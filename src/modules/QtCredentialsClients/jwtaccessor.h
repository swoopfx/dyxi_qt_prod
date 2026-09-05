#ifndef JWTACCESSOR_H
#define JWTACCESSOR_H

#include <QObject>
#include <QString>
#include <qtkeychain/keychain.h>
#include <QQmlEngine>

class JwtAccessor : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit JwtAccessor(QObject *parent = nullptr);

    void retrieveToken();
    void retriveRefreshToken();

    QString token() const;
    void setToken(const QString &newToken);

    QString refreshToken() const;
    void setRefreshToken(const QString &newRefreshToken);

signals:

    void tokenChanged();

    void refreshTokenChanged();

private:
    QString m_token;
    QString m_refreshToken;


    Q_PROPERTY(QString token READ token WRITE setToken NOTIFY tokenChanged FINAL)
    Q_PROPERTY(QString refreshToken READ refreshToken WRITE setRefreshToken NOTIFY refreshTokenChanged FINAL)
};

#endif // JWTACCESSOR_H
