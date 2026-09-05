#pragma once

#include <QObject>
#include <QPointF>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include <QtQml/qqmlregistration.h>
#include "AppDifficulty.hpp"

class TracingEngine : public QObject {
    Q_OBJECT
    QML_ELEMENT

    // Properties exposed to QML
    Q_PROPERTY(AppDifficulty::Level difficulty READ difficulty WRITE setDifficulty NOTIFY difficultyChanged)
    Q_PROPERTY(qreal accuracy READ accuracy NOTIFY accuracyChanged)
    Q_PROPERTY(int streak READ streak NOTIFY streakChanged)
    Q_PROPERTY(int totalScore READ totalScore NOTIFY totalScoreChanged)
    Q_PROPERTY(int stars READ stars NOTIFY starsChanged)
    Q_PROPERTY(bool isCompleted READ isCompleted NOTIFY isCompletedChanged)
    Q_PROPERTY(QString shapeId READ shapeId WRITE setShapeId NOTIFY shapeIdChanged)
    Q_PROPERTY(QString rankTitle READ rankTitle NOTIFY scoreResultChanged)

public:
    explicit TracingEngine(QObject *parent = nullptr);

    AppDifficulty::Level difficulty() const { return m_difficulty; }
    void setDifficulty(AppDifficulty::Level level);

    qreal accuracy() const { return m_accuracy; }
    int streak() const { return m_streak; }
    int totalScore() const { return m_totalScore; }
    int stars() const { return m_stars; }
    bool isCompleted() const { return m_isCompleted; }
    QString shapeId() const { return m_shapeId; }
    void setShapeId(const QString &id);
    QString rankTitle() const { return m_rankTitle; }

    // Heavy-duty C++ data processing invokable from QML touch events
    Q_INVOKABLE void startTracingSession(const QString &shapeId, qreal canvasWidth, qreal canvasHeight);
    Q_INVOKABLE bool processRawTouchPoint(qreal x, qreal y, qint64 timestampMs);
    Q_INVOKABLE void finalizeStroke();
    Q_INVOKABLE QJsonObject evaluateSessionResult();
    Q_INVOKABLE void resetSession();

    // Geometric calculations
    qreal calculateDistanceToGuideRail(const QPointF &point) const;
    void setGuideWaypoints(const QVector<QPointF> &waypoints);

signals:
    void difficultyChanged();
    void accuracyChanged();
    void streakChanged();
    void totalScoreChanged();
    void starsChanged();
    void isCompletedChanged();
    void shapeIdChanged();
    void scoreResultChanged();
    void pointSnapped(qreal x, qreal y);
    void starEarned(int starNumber);
    void strokeDeviationOccurred(qreal deviationDistance);

private:
    void updateScoringState(bool withinCorridor, qreal distance);

    AppDifficulty::Level m_difficulty = AppDifficulty::Level::Medium;
    AppDifficulty::Config m_config;

    QString m_shapeId = "circle";
    qreal m_canvasWidth = 800.0;
    qreal m_canvasHeight = 600.0;

    QVector<QPointF> m_guideWaypoints;
    QVector<QPointF> m_recordedPoints;
    QVector<qint64> m_timestamps;

    qreal m_accuracy = 100.0;
    int m_streak = 0;
    int m_maxStreak = 0;
    int m_totalScore = 0;
    int m_stars = 0;
    bool m_isCompleted = false;
    QString m_rankTitle = "Precision Scribbler";

    int m_pointsInCorridor = 0;
    int m_totalSamplePoints = 0;
    int m_deviationsCount = 0;
    qreal m_totalDeviationDistance = 0.0;
    qint64 m_sessionStartTime = 0;
};
