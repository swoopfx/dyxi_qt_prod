#include "TracingEngine.hpp"
#include <QtMath>
#include <QDateTime>

TracingEngine::TracingEngine(QObject *parent)
    : QObject(parent)
{
    m_config = AppDifficulty::getConfig(m_difficulty);
}

void TracingEngine::setDifficulty(AppDifficulty::Level level) {
    if (m_difficulty != level) {
        m_difficulty = level;
        m_config = AppDifficulty::getConfig(m_difficulty);
        emit difficultyChanged();
    }
}

void TracingEngine::setShapeId(const QString &id) {
    if (m_shapeId != id) {
        m_shapeId = id;
        emit shapeIdChanged();
    }
}

void TracingEngine::startTracingSession(const QString &shapeId, qreal canvasWidth, qreal canvasHeight) {
    m_shapeId = shapeId;
    m_canvasWidth = canvasWidth;
    m_canvasHeight = canvasHeight;
    m_sessionStartTime = QDateTime::currentMSecsSinceEpoch();

    resetSession();
}

void TracingEngine::resetSession() {
    m_recordedPoints.clear();
    m_timestamps.clear();
    m_accuracy = 100.0;
    m_streak = 0;
    m_maxStreak = 0;
    m_totalScore = 0;
    m_stars = 0;
    m_isCompleted = false;
    m_pointsInCorridor = 0;
    m_totalSamplePoints = 0;
    m_deviationsCount = 0;
    m_totalDeviationDistance = 0.0;
    m_rankTitle = "Precision Scribbler";

    emit accuracyChanged();
    emit streakChanged();
    emit totalScoreChanged();
    emit starsChanged();
    emit isCompletedChanged();
    emit scoreResultChanged();
}

void TracingEngine::setGuideWaypoints(const QVector<QPointF> &waypoints) {
    m_guideWaypoints = waypoints;
}

qreal TracingEngine::calculateDistanceToGuideRail(const QPointF &point) const {
    if (m_guideWaypoints.isEmpty()) {
        return 0.0;
    }

    qreal minDistance = std::numeric_limits<qreal>::max();

    // Iterate through guide segments to find shortest Euclidean projection
    for (int i = 0; i < m_guideWaypoints.size() - 1; ++i) {
        QPointF p1 = m_guideWaypoints[i];
        QPointF p2 = m_guideWaypoints[i + 1];

        qreal dx = p2.x() - p1.x();
        qreal dy = p2.y() - p1.y();
        qreal lengthSquared = dx * dx + dy * dy;

        qreal t = 0.0;
        if (lengthSquared > 0.0001) {
            t = ((point.x() - p1.x()) * dx + (point.y() - p1.y()) * dy) / lengthSquared;
            t = qBound(0.0, t, 1.0);
        }

        QPointF projection(p1.x() + t * dx, p1.y() + t * dy);
        qreal dist = QLineF(point, projection).length();

        if (dist < minDistance) {
            minDistance = dist;
        }
    }

    return minDistance;
}

bool TracingEngine::processRawTouchPoint(qreal x, qreal y, qint64 timestampMs) {
    QPointF current(x, y);

    // Distance decimation: avoid processing redundant micro-jitter
    if (!m_recordedPoints.isEmpty()) {
        qreal delta = QLineF(m_recordedPoints.last(), current).length();
        if (delta < 3.0) {
            return true;
        }
    }

    m_recordedPoints.append(current);
    m_timestamps.append(timestampMs);
    m_totalSamplePoints++;

    // Heavy-duty calculation of distance from ideal corridor rail
    qreal distance = calculateDistanceToGuideRail(current);
    bool withinCorridor = distance <= m_config.corridorTolerancePx;

    updateScoringState(withinCorridor, distance);

    return withinCorridor;
}

void TracingEngine::updateScoringState(bool withinCorridor, qreal distance) {
    if (withinCorridor) {
        m_pointsInCorridor++;
        m_streak++;
        if (m_streak > m_maxStreak) {
            m_maxStreak = m_streak;
        }

        // Scoring scales with streak & difficulty multiplier
        int pointBase = 10;
        int streakBonus = qMin(m_streak / 5, 20);
        int addedScore = static_cast<int>((pointBase + streakBonus) * m_config.scoringMultiplier);
        m_totalScore += addedScore;
    } else {
        m_streak = 0;
        m_deviationsCount++;
        m_totalDeviationDistance += (distance - m_config.corridorTolerancePx);
        emit strokeDeviationOccurred(distance);
    }

    if (m_totalSamplePoints > 0) {
        m_accuracy = (static_cast<qreal>(m_pointsInCorridor) / m_totalSamplePoints) * 100.0;
        m_accuracy = qBound(0.0, m_accuracy, 100.0);
    }

    // Dynamic Star Thresholds based on developer difficulty
    int newStars = 1;
    if (m_accuracy >= m_config.minAccuracyThreshold) {
        newStars = 3;
    } else if (m_accuracy >= (m_config.minAccuracyThreshold * 0.75)) {
        newStars = 2;
    }

    if (newStars > m_stars) {
        m_stars = newStars;
        emit starEarned(m_stars);
        emit starsChanged();
    }

    emit accuracyChanged();
    emit streakChanged();
    emit totalScoreChanged();
}

void TracingEngine::finalizeStroke() {
    // Check completion threshold
    if (m_totalSamplePoints >= 15 && m_accuracy >= 40.0) {
        m_isCompleted = true;

        if (m_accuracy >= 85.0) {
            m_rankTitle = "Bullseye Master!";
        } else if (m_accuracy >= 70.0) {
            m_rankTitle = "Super Neat Tracer!";
        } else {
            m_rankTitle = "Great Effort!";
        }

        emit isCompletedChanged();
        emit scoreResultChanged();
    }
}

QJsonObject TracingEngine::evaluateSessionResult() {
    QJsonObject result;
    result["accuracy"] = m_accuracy;
    result["totalScore"] = m_totalScore;
    result["maxStreak"] = m_maxStreak;
    result["stars"] = m_stars;
    result["pointsRecorded"] = m_totalSamplePoints;
    result["deviationsCount"] = m_deviationsCount;
    result["avgDeviationPx"] = m_deviationsCount > 0 ? (m_totalDeviationDistance / m_deviationsCount) : 0.0;
    result["rankTitle"] = m_rankTitle;
    result["difficultyLevel"] = m_config.label;
    result["corridorWidthPx"] = m_config.corridorTolerancePx;

    return result;
}
