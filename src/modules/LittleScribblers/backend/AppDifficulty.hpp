#pragma once

#include <QObject>
#include <QString>
#include <QtQml/qqmlregistration.h>

class AppDifficulty : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("AppDifficulty provides difficulty enums and developer configurations")

public:
    enum class Level {
        Easy,
        Medium,
        Hard
    };
    Q_ENUM(Level)

    struct Config {
        Level level;
        QString label;
        qreal corridorTolerancePx;
        qreal minAccuracyThreshold;
        qreal speedPenaltyFactor;
        qreal snapTolerancePx;
        bool guideAssistVisible;
        qreal scoringMultiplier;
        QString description;
    };

    static Config getConfig(Level level);

    Q_INVOKABLE static QString getLabel(Level level);
    Q_INVOKABLE static qreal getCorridorTolerance(Level level);
    Q_INVOKABLE static qreal getMinAccuracy(Level level);
    Q_INVOKABLE static bool getGuideAssist(Level level);
    Q_INVOKABLE static qreal getMultiplier(Level level);
};
