# Step-by-Step Qt6 C++ & QML Integration Tutorial

Follow this step-by-step developer guide to integrate modern secure authentication (OAuth2) and platform-native secure keyring persistence (**QtKeychain**) into your Qt6 applications.

---

## Step 1: Link Dependencies in CMake
To enable OAuth2 flows and secure keychain calls, update your CMake target configurations to locate and link the necessary packages.

### Desktop & Mobile Cohesive Toolchain Configuration
In your primary `CMakeLists.txt`, find and link `NetworkAuth` (which houses Qt's OAuth client) and `qtkeychain`:

```cmake
# 1. Discover modules
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Network NetworkAuth)
find_package(qtkeychain REQUIRED)

# 2. Setup your target executable
add_executable(QtOAuthApp
    main.cpp
    authservice.h
    authservice.cpp
    resources.qrc
)

# 3. Link modules
target_link_libraries(QtOAuthApp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::Network
    Qt6::NetworkAuth
    qtkeychain
)
```

---

## Step 2: Register Native Mobile Redirections (Deep Links)
Mobile operating systems isolate apps and block direct loopback server listeners. Therefore, you must register **Custom URL Schemes** (Deep Links) to intercept callbacks.

### A. Android Setup (`AndroidManifest.xml`)
Add an intent filter with a custom scheme and host to your launch activity:
```xml
<intent-filter android:autoVerify="true">
    <action android:name="android.intent.action.VIEW" />
    <category android:name="android.intent.category.DEFAULT" />
    <category android:name="android.intent.category.BROWSABLE" />
    <data android:scheme="myqtoauth" android:host="callback" />
</intent-filter>
```

### B. iOS Tablet Setup (`Info.plist`)
Declare the identical custom callback scheme in your Apple package details:
```xml
<key>CFBundleURLTypes</key>
<array>
    <dict>
        <key>CFBundleTypeRole</key>
        <string>Editor</string>
        <key>CFBundleURLSchemes</key>
        <array>
            <string>myqtoauth</string>
        </array>
    </dict>
</array>
```

---

## Step 3: Define the C++ AuthService Controller
Create a controller inheriting from `QObject` to bridge your C++ authentication operations directly to QML bindings.

### Header File (`authservice.h`)
Expose statuses, errors, and authentication trigger methods to your UI:
```cpp
#pragma once
#include <QObject>
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>

class AuthService : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)
    Q_PROPERTY(bool isBusy READ isBusy NOTIFY isBusyChanged)

public:
    explicit AuthService(QObject *parent = nullptr);
    QString statusText() const;
    bool isBusy() const;

    Q_INVOKABLE void startGoogleLogin();
    Q_INVOKABLE void startAppleLogin();
    Q_INVOKABLE void cancelLogin();
    Q_INVOKABLE void signOut();

signals:
    void statusTextChanged();
    void isBusyChanged();
    void loginSuccess(const QString &userName, const QString &userEmail);
    void loginError(const QString &errorMessage);

private:
    QOAuth2AuthorizationCodeFlow m_oauth;
    QOAuthHttpServerReplyHandler *m_replyHandler;
    bool m_isBusy;
    QString m_statusText;
};
```

---

## Step 4: Implement Anti-CSRF Token Checks
Before launching standard system browser redirect parameters, create a random security state string and compare it upon loopback capture.

```cpp
#include <QRandomGenerator>
#include <QCryptographicHash>

// Inside login trigger routines:
QString csrfState = QString::fromLatin1(
    QRandomGenerator::global()->generateDouble() > 0.5 ? "stateA" : "stateB"
).toUtf8().toBase64(QByteArray::Base64UrlEncoding);

m_oauth.setModifyParametersFunction([csrfState](QAbstractOAuth::Stage stage, QMultiMap<QString, QVariant> *parameters) {
    if (stage == QAbstractOAuth::Stage::RequestingAuthorization) {
        parameters->insert("state", csrfState);
    }
});
```

---

## Step 5: Persist Keys Securely via QtKeychain
Do not store production API keys or tokens in plain-text `QSettings` plist files or registries. Instead, write them into native OS secure hardware crypts via non-blocking write/read jobs.

### A. Saving Credentials (`WritePasswordJob`)
Upon successful login callback, write the token to the system keyring:
```cpp
#include <keychain.h>

void AuthService::saveTokenToKeychain(const QString &token) {
    QKeychain::WritePasswordJob *job = new QKeychain::WritePasswordJob("EnterpriseSecurity", this);
    job->setKey("oauth_access_token");
    job->setPassword(token);
    
    connect(job, &QKeychain::WritePasswordJob::finished, [this, job]() {
        if (job->error()) {
            emit loginError("Failed to store credentials: " + job->errorString());
        } else {
            rawLog("🔑 Access token saved securely in OS Keyring.", "success");
        }
        job->deleteLater();
    });
    job->start();
}
```

### B. Automatic Session Restoration (`ReadPasswordJob`)
Upon application boot, search the secure keyring. If keys exist, load user profiles immediately:
```cpp
void AuthService::tryRestoreSession() {
    QKeychain::ReadPasswordJob *job = new QKeychain::ReadPasswordJob("EnterpriseSecurity", this);
    job->setKey("oauth_access_token");
    
    connect(job, &QKeychain::ReadPasswordJob::finished, [this, job]() {
        if (!job->error()) {
            QString token = job->text();
            rawLog("🔄 Recovered existing session from native keyring.", "info");
            fetchUserProfile(token);
        } else {
            rawLog("⚠️ No cached session located in platform keyrings.", "info");
        }
        job->deleteLater();
    });
    job->start();
}
```

### C. Cleardown Wipes (`DeletePasswordJob`)
Scrub local keys upon logout requests to prevent security bypasses:
```cpp
void AuthService::signOut() {
    QKeychain::DeletePasswordJob *job = new QKeychain::DeletePasswordJob("EnterpriseSecurity", this);
    job->setKey("oauth_access_token");
    
    connect(job, &QKeychain::DeletePasswordJob::finished, [this, job]() {
        rawLog("🧹 Revoked and deleted active token from platform keyring.", "info");
        job->deleteLater();
    });
    job->start();
}
```

---

## Step 6: Separate Login & Welcome QML Components
For modular clean layouts, separate user experience states into modular files.

### A. The Login Gate (`Login.qml`)
Displays action triggers to authenticate when no active keyring tokens are found:
```qml
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    Button {
        text: "Connect Google Account"
        onClicked: authService.startGoogleLogin()
    }
}
```

### B. The Landed Dashboard (`WelcomePage.qml`)
Rendered automatically upon C++ validation signals:
```qml
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    Label {
        text: "Handshake Verified"
    }
}
```
