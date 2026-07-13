#include "inputeventfilter.h"
#include <QDateTime>
#include <QMouseEvent>

InputEventFilter::InputEventFilter(QObject *parent)
    : QObject(parent) {}

bool InputEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type()) {

    case QEvent::MouseButtonPress: {
        auto *me = static_cast<QMouseEvent*>(event);

        emit inputPressed(
            QDateTime::currentMSecsSinceEpoch(),
            me->position()
            );
        break;
    }

    case QEvent::MouseButtonRelease: {
        auto *me = static_cast<QMouseEvent*>(event);

        emit inputReleased(
            QDateTime::currentMSecsSinceEpoch(),
            me->position()
            );
        break;
    }

    default:
        break;
    }

    return QObject::eventFilter(obj, event);
}