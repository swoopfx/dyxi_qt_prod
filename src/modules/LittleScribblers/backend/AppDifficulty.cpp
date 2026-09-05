#include "AppDifficulty.hpp"

AppDifficulty::Config AppDifficulty::getConfig(AppDifficulty::Level level) {
    switch (level) {
    case Level::Easy:
        return {
            Level::Easy,
            "Easy (Explorer)",
            48.0, // generous 48px corridor
            60.0, // 60% min accuracy
            0.0,  // no speed penalty
            16.0, // 16px magnetic snap
            true, // guide assist visible
            1.0,  // multiplier
            "Generous 48px corridor with magnetic snapping for beginners."
        };
    case Level::Hard:
        return {
            Level::Hard,
            "Hard (Precision Master)",
            20.0, // narrow 20px corridor
            88.0, // 88% min accuracy
            0.5,  // speed penalty
            5.0,  // tight snap
            false, // minimal guide
            2.0,  // multiplier
            "Strict 20px precision corridor with deviation penalties."
        };
    case Level::Medium:
    default:
        return {
            Level::Medium,
            "Medium (Standard)",
            32.0, // 32px corridor
            75.0, // 75% min accuracy
            0.2,  // minor speed penalty
            10.0, // snap
            true, // guide assist visible
            1.5,  // multiplier
            "Standard 32px corridor for developing shape precision."
        };
    }
}

QString AppDifficulty::getLabel(Level level) {
    return getConfig(level).label;
}

qreal AppDifficulty::getCorridorTolerance(Level level) {
    return getConfig(level).corridorTolerancePx;
}

qreal AppDifficulty::getMinAccuracy(Level level) {
    return getConfig(level).minAccuracyThreshold;
}

bool AppDifficulty::getGuideAssist(Level level) {
    return getConfig(level).guideAssistVisible;
}

qreal AppDifficulty::getMultiplier(Level level) {
    return getConfig(level).scoringMultiplier;
}
