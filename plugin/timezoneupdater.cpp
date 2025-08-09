// SPDX-License-Identifier: LGPL-2.1-only

#include "timezoneupdater.h"
#include <QQmlEngine>
#include <QTimeZone>

TimezoneUpdater::TimezoneUpdater(QQmlEngine *engine, QObject *parent)
    : QObject(parent)
    , m_engine(engine)
    , m_lastTimezone(QTimeZone::systemTimeZoneId())
{
    // Note: In a real implementation, this would connect to Maemo::Timed::WallClock
    // For now, we'll use a simplified approach that monitors system timezone changes
    // via QTimeZone::systemTimeZoneId()
}

TimezoneUpdater::~TimezoneUpdater()
{
}

void TimezoneUpdater::onTimezoneChanged()
{
    QString currentTimezone = QTimeZone::systemTimeZoneId();

    if (m_lastTimezone != currentTimezone) {
        m_lastTimezone = currentTimezone;

        // Trigger QML engine re-evaluation for locale-dependent bindings
        if (m_engine) {
            // This would typically trigger retranslation or locale-dependent updates
            // For now, we'll emit a signal that QML can listen to
            emit m_engine->retranslate();
        }
    }
}
