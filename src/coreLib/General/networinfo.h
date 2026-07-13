#ifndef NETWORINFO_H
#define NETWORINFO_H

#include <QObject>
#include <QQmlEngine>

class NetworInfo : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit NetworInfo(QObject *parent = nullptr);

signals:
    void networkChanged();

private :
    void checkNetworkStatus();
};

#endif // NETWORINFO_H
