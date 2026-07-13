# Qt6.10 Desktop Credentials & Keyring Integration Guide

This application simulates a production-ready enterprise authentication flow designed inside Qt 6.10 C++ and QML. To safeguard sensitive credentials such as access tokens, master passcodes, and user secrets in a fully platform-independent layout, it incorporates standard Qt Network APIs and modern cryptographic security models instead of vulnerable plain-text configurations.

---

## 1. Credentials Authentication Architecture

A secure desktop authentication flow consists of the following progressive states:

### A. Credentials Verification (QNetworkAccessManager)
- **Central REST Authentication**: Instead of spawning vulnerable in-app WebViews or complex local loopback HTTP sockets, the C++ service utilizes standard `QNetworkAccessManager` to submit login credentials (username and master password) over a secure TLS/SSL socket.
- **Payload Serialization**: The input credentials are serialized into high-performance, strictly structured JSON strings using the `QJsonDocument` and `QJsonObject` APIs.
- **Asynchronous Handshake**: All network requests execute asynchronously via signals and slots. This ensures that network latency never blocks or freezes the main operating system window or parent user interface loops.

### B. Multi-Provider OAuth 2.0 (Google & Apple ID)
- **QtNetworkAuth Module**: To integrate modern secure authorization protocols, the application incorporates `QOAuth2AuthorizationCodeFlow` to query standard open-ID providers such as Google and Apple ID.
- **Local Loopback Callback**: A local lightweight `QOAuthHttpServerReplyHandler` server listener is spawned dynamically on port 8080 to cleanly capture the safe browser redirection redirect code query.
- **Token Exchange & Ring Vaults**: Once the authorization callback authorization code is captured, the service exchanges it securely for user credentials. These parameters are then stored inside the native operating system keyring safe with standard Write Password Jobs.

### C. OS hardware-backed Keyrings (QtKeychain)
- **The Plain-text Vulnerability**: Traditional configurations save settings using simple text files (like standard `QSettings` INI configurations). This stores sensitive access tokens in plain-text inside macOS plists, Windows registry entries, or Linux home directories, creating massive security holes.
- **The Solution**: The client integrates **QtKeychain**, a secure, cross-platform wrapper that communicates directly with native hardware credential safes:
  - **macOS / iOS**: Interacts directly with the secure **Apple Keychain Services** API.
  - **Windows**: Stores credentials securely in the native **Windows Credential Manager**.
  - **Linux**: Commits fields using **GNOME Keyring** or **KWallet** storage providers.

---

## 2. Non-Blocking Keyring Operations

To ensure the desktop GUI never locks up during device I/O, QtKeychain jobs run asynchronously on background threads:

### A. Writing Credentials (WritePasswordJob)
When credentials verify successfully against the REST API, an asynchronous `QKeychain::WritePasswordJob` is spawned to write the session token securely:
```cpp
QKeychain::WritePasswordJob *job = new QKeychain::WritePasswordJob("EnterpriseSecurity", this);
job->setKey("session_auth_token");
job->setPassword(token);
connect(job, &QKeychain::WritePasswordJob::finished, [this, job]() {
    if (!job->error()) {
        emit loginSuccess();
    }
});
job->start();
```

### B. Session Restorations (ReadPasswordJob)
When the application reboots, the client automatically triggers a non-blocking `ReadPasswordJob` query. If a credentials token matches, the user is instantly logged back in without any manual passcode prompt:
```cpp
QKeychain::ReadPasswordJob *job = new QKeychain::ReadPasswordJob("EnterpriseSecurity", this);
job->setKey("session_auth_token");
connect(job, &QKeychain::ReadPasswordJob::finished, [this, job]() {
    if (!job->error()) {
        restoreSession(job->text());
    }
});
job->start();
```

### C. Cleardown Wipes (DeletePasswordJob)
Upon logging out, a clean `DeletePasswordJob` is executed to physically scrub the active session key, preventing unauthorized token recovery.

---

## 3. Qt 6.10 Declarative Standards

This template strictly conforms to **Qt 6.10** standards:
- **Declarative QML Registration**: Bypasses legacy C++ registration helpers (like `qmlRegisterType`). Instead, types use the modern `QML_ELEMENT` macro directly inside header definitions.
- **CMake QML Modules**: Uses `qt_add_qml_module` inside CMake to automatically generate and register the required QML metadata, aligning with modern Qt 6 compiler targets.
- **OAuth Integration**: Integrates the native `QtNetworkAuth` library components to orchestrate state validation and secure browser integration.

---

## 4. Secure Platform-Independent Environment Configuration Mapping

To secure production parameters (like authorization URL endpoints, cryptographic master keys, and timeouts) without hardcoding them in the C++ binaries or writing plain text files in local folders, our template incorporates the **EnvConfig** mapping module:
- **Unified QProcessEnvironment**: Resolves native system environment variables using standard cross-platform Qt containers (`QProcessEnvironment`).
- **Standard Storage Paths (QStandardPaths)**: Dynamically resolves platform-specific secure sandboxes for localized configuration files (e.g. `.env.security` overrides):
  - **Windows**: `C:/Users/<User>/AppData/Local/EnterpriseCompany/HeroGateSecureClient`
  - **macOS / iOS**: `/Users/<User>/Library/Application Support/EnterpriseCompany/HeroGateSecureClient`
  - **Linux**: `/home/<User>/.config/EnterpriseCompany/HeroGateSecureClient`
- **Credentials Masking**: Ensures sensitive parameters (e.g. `SECRET_KEY`, `TOKEN`, `PASSWORD`) are never displayed in application log feeds, replacing them with dynamic secure mask structures (`[ENCRYPTED_AT_REST]`).

---

## 5. Step-by-Step QtKeychain Setup Guide

To compile and link the secure credentials client with QtKeychain across all target desktop platforms, follow this comprehensive installation guide:

### Step 1: Install QtKeychain Packages
- **macOS (via Homebrew)**:
  ```bash
  brew install qtkeychain
  ```
- **Linux (Debian/Ubuntu)**:
  ```bash
  sudo apt-get install qtkeychain-qt6-dev
  ```
- **Windows (via vcpkg)**:
  ```bash
  vcpkg install qtkeychain:x64-windows
  ```

### Step 2: Configure CMakeLists.txt
Add the package finder and link dependencies to your project target:
```cmake
find_package(qtkeychain REQUIRED)
target_link_libraries(QtCredentialsClient PRIVATE qtkeychain)
```

### Step 3: Set Application Namespaces
Inside your C++ `main.cpp`, configure application and organization names. This is mandatory, as QtKeychain uses this metadata to isolate sandbox folders in native platforms (e.g., Windows Registry, macOS Keychain Access):
```cpp
QGuiApplication app(argc, argv);
app.setOrganizationName("EnterpriseCompany");
app.setOrganizationDomain("enterprise-security.internal");
app.setApplicationName("HeroGateSecureClient");
```
