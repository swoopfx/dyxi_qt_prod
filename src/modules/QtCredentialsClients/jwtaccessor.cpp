#include "jwtaccessor.h"
#include <qtkeychain/keychain.h>

JwtAccessor::JwtAccessor(QObject *parent)
    : QObject{parent}
{
    retrieveToken();
    retriveRefreshToken();
}

void JwtAccessor::retrieveToken()
{
    auto *job =
        new QKeychain::ReadPasswordJob(
            QStringLiteral(
                "QtCredentialsClient"),
            this);

    job->setKey(
        QString::fromUtf8(
            "session_access_token"));

    connect(
        job,
        &QKeychain::ReadPasswordJob::finished,
        this,
        [this, job]() {

            const bool failed = job->error();

            const QString token =job->textData();



            if (!failed && !token.isEmpty()) {

                setToken(token);

            }



            job->deleteLater();
        });

}

void JwtAccessor::retriveRefreshToken()
{
    auto *job =
        new QKeychain::ReadPasswordJob(
            QStringLiteral(
                "QtCredentialsClient"),
            this);

    job->setKey(
        QString::fromUtf8(
            "session_refresh_token"));

    connect(
        job,
        &QKeychain::ReadPasswordJob::finished,
        this,
        [this, job]() {

            const bool failed = job->error();

            const QString token =job->textData();



            if (!failed && !token.isEmpty()) {

                setToken(token);

            }

            job->deleteLater();
        });
}

QString JwtAccessor::token() const
{
    return m_token;
}

void JwtAccessor::setToken(const QString &newToken)
{
    if (m_token == newToken)
        return;
    m_token = newToken;
    emit tokenChanged();
}

QString JwtAccessor::refreshToken() const
{
    return m_refreshToken;
}

void JwtAccessor::setRefreshToken(const QString &newRefreshToken)
{
    if (m_refreshToken == newRefreshToken)
        return;
    m_refreshToken = newRefreshToken;
    emit refreshTokenChanged();
}
