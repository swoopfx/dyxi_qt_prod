#pragma once

#include <QObject>
#include <QEvent>
#include <QMouseEvent>
#include <QPointF>

class InputEventFilter : public QObject {
    Q_OBJECT

public:
    explicit InputEventFilter(QObject *parent = nullptr);

signals:
    void inputPressed(qint64 timestamp, QPointF pos);
    void inputReleased(qint64 timestamp, QPointF pos);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};