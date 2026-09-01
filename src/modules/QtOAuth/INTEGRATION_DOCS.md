# Qt 6.11 Architecture Guide: QtKeychain Credential Storage & Conventional Google OAuth Integration

## 1. Overview & Architectural Philosophy

This Qt 6.11 C++/QML architecture provides a clean, robust, cross-platform enterprise authentication system based on two core foundations:

1. **Native OS Credential Storage via `qtkeychain`**:
   - Instead of storing bearer tokens or credentials in plaintext `QSettings`, files, or unencrypted local databases, credentials are saved in the host operating system's native encrypted credential store:
     - **Windows**: Windows Credential Manager (Data Protection API - DPAPI)
     - **macOS**: Apple Keychain Services (Security Framework)
     - **Linux**: Secret Service API / `libsecret` (GNOME Keyring / KDE KWallet)
     - **Android**: Android KeyStore via QtKeychain JNI bridge
     - **iOS**: iOS Keychain Services (`kSecClassGenericPassword`)
2. **Conventional Google OAuth 2.0 with PKCE (No Client Secrets)**:
   - Modern public client applications (desktop and mobile) **never store or transmit client secrets**.
   - The app uses **RFC 7636 PKCE (Proof Key for Code Exchange with S256)**:
     - The client generates a high-entropy `code_verifier` and calculates its SHA-256 hash `code_challenge`.
     - The user authenticates in the system browser against Google's official authorization endpoint (`https://accounts.google.com/o/oauth2/v2/auth`) with your Public Client ID:
       `952310262713-4b1f3os6uqmhorsjmp9tmritbph1m6sg.apps.googleusercontent.com`
     - Google redirects to the local loopback server (`http://localhost:8080/` via `QOAuthHttpServerReplyHandler`).
     - The client exchanges the authorization `code` + `code_verifier` directly at `https://oauth2.googleapis.com/token` for tokens **without any client secret**.
     - The resulting session token is committed immediately to the OS Keychain via `KeychainHelper`.
3. **Flexible Environment Configuration (`.env.security`)**:
   - Environment parameters are managed via `EnvConfig` with cascading priority: OS Environment variables -> `.env.security` configuration file -> compiled fallback defaults.

---

## 2. Component Structure & Data Flow

```
+----------------------------------------------------------------------------------------------------+
|                                    QT 6.11 ARCHITECTURE PIPELINE                                   |
+----------------------------------------------------------------------------------------------------+
|                                                                                                    |
|  [QML User Interface] (Login.qml / WelcomePage.qml)                                                |
|            │                                                                                       |
|            ▼ (Q_INVOKABLE)                                                                         |
|  [AuthService] (authservice.cpp / authservice.h)                                                   |
|       ├──> [EnvConfig] (envconfig.cpp) ──> Reads .env.security / QProcessEnvironment               |
|       │                                    (Google Client ID: 952310262713-...apps.google...)      |
|       │                                                                                            |
|       ├──> [OAuth Flow] (QOAuth2AuthorizationCodeFlow)                                             |
|       │       ├── 1. Generate PKCE S256 Code Verifier & Challenge (RFC 7636)                       |
|       │       ├── 2. Launch System Browser -> accounts.google.com/o/oauth2/v2/auth                 |
|       │       ├── 3. Listen on http://localhost:8080/ via QOAuthHttpServerReplyHandler             |
|       │       └── 4. Exchange Code for Access Token at oauth2.googleapis.com (NO Client Secret)    |
|       │                                                                                            |
|       ├──> [REST Auth] (QNetworkAccessManager) ──> POST authEndpoint (Email / Password)            |
|       │                                                                                            |
|       └──> [KeychainHelper] (keychainhelper.cpp)                                                   |
|               └──> QKeychain::WritePasswordJob / ReadPasswordJob / DeletePasswordJob               |
|                       ├── Windows Credential Manager                                               |
|                       ├── macOS Keychain                                                           |
|                       ├── Linux Secret Service (libsecret)                                         |
|                       └── Android / iOS KeyStore                                                   |
|                                                                                                    |
+----------------------------------------------------------------------------------------------------+
```

---

## 3. Project Directory Structure in Qt Creator

```text
HeroGateClient/
├── CMakeLists.txt              <-- CMake build configuration (Qt 6.11, NetworkAuth, QtKeychain)
├── main.cpp                   <-- C++ application bootstrap & QML type registration
├── .env.security              <-- Environment configuration file (Endpoints, Client ID, Timeouts)
├── src/
│   ├── authservice.h          <-- AuthService declarations (OAuth PKCE, REST, Keychain integration)
│   ├── authservice.cpp        <-- AuthService implementation
│   ├── keychainhelper.h       <-- QKeychain wrapper header (Cross-platform OS Keyring)
│   ├── keychainhelper.cpp     <-- QKeychain wrapper implementation
│   ├── envconfig.h            <-- Environment configuration parser header
│   └── envconfig.cpp          <-- Environment configuration parser implementation
├── qml/
│   ├── main.qml               <-- Root window & navigation loader
│   ├── Login.qml              <-- Centered 1/3 light-themed Login & OAuth interface
│   └── WelcomePage.qml        <-- Authenticated dashboard & Keychain session status
└── android/
    └── AndroidManifest.xml    <-- Android manifest with loopback / intent filter permissions
```

---

## 4. Step-by-Step Compilation Guide in Qt Creator 13+

### Step 1: Toolchain & Dependencies Setup
1. Open **Qt Creator 13+**.
2. Ensure you have installed **Qt 6.11** with the **Network Authorization (QtNetworkAuth)** component.
3. Install or clone `qtkeychain` (available via vcpkg `vcpkg install qtkeychain`, Conan, or system packages `sudo apt install qtkeychain-qt6-dev` on Ubuntu/Debian).

### Step 2: Open and Configure Project
1. Select **File -> Open File or Project...** (`Ctrl+O`).
2. Select `CMakeLists.txt` in the project directory.
3. Choose your desired Qt 6.11 kit:
   - **Desktop Qt 6.11 (MSVC 2022 / GCC / Clang 64-bit)**
   - **Android Qt 6.11 Clang arm64-v8a**
4. Click **Configure Project**.

### Step 3: Environment Setup
1. Verify that `.env.security` exists in the build or executable directory.
2. In Qt Creator, navigate to **Projects Mode (Ctrl+5) -> Run Settings -> Run Environment**.
3. (Optional) You can override any variable directly in the environment:
   - `GOOGLE_CLIENT_ID=952310262713-4b1f3os6uqmhorsjmp9tmritbph1m6sg.apps.googleusercontent.com`
   - `ENTERPRISE_AUTH_ENDPOINT=https://api.enterprise-security.internal/v1/auth/login`
   - `ENTERPRISE_AUTH_TIMEOUT_MS=1200`

### Step 4: Build and Run
1. Press `Ctrl+B` to compile with CMake and Ninja.
2. Press `Ctrl+R` to run the application.
3. On launch, `AuthService` will query the OS Keychain via `KeychainHelper` to restore any existing authenticated session.
