# Qt6 Desktop OAuth2 & Keyring Integration Guide

This application simulates a production-ready enterprise authentication flow designed inside Qt6 C++ and QML. To safeguard sensitive credentials such as access tokens, state values, and user secrets in a fully platform-independent layout, it incorporates standard Qt APIs and modern cryptographic security models instead of vulnerable plain-text configurations.

---

## 1. The OAuth 2.0 Authorization Code Loop

A secure desktop authentication flow consists of the following progressive states:

### A. Authorize Request Trigger
- **System Browser Redirect**: The C++ service constructs a customized HTTPS authorization URL with required scopes (`openid`, `profile`, `email`), `client_id`, `redirect_uri`, and a dynamically generated anti-CSRF `state` parameter. It safely opens the user's default desktop browser via `QDesktopServices::openUrl()` so credentials are typed directly into Google or Apple secure portals without app visibility.
- **Why WebViews are Discouraged**: Embedding in-app WebViews (like QWebEngineView) is considered an anti-pattern. Both Google and Apple block in-app browsers due to potential credential-hijacking risks. Spawning the native system web browser is the strict security requirement.

### B. Dynamic Loopback Server
- **QOAuthHttpServerReplyHandler**: Instead of writing standard TCP sockets, we configure Qt's native C++ `QOAuthHttpServerReplyHandler` as part of the `QtNetworkAuth` package. It listens on an active loopback interface (e.g., `http://localhost:8080`) to securely capture incoming auth code redirections.
- **Handling Redirect Handshake**: Once a user permits account authorization, the remote server initiates a callback browser request returning `code` and `state` parameters. The loopback server processes the request, pushes a success HTML completion confirmation straight to the browser, and parses the fields.

---

## 2. Dynamic Cryptographic Anti-CSRF Protection

To deny Session Hijacking or Cross-Site Request Forgery (CSRF) exploits, the client manages state assertion:

- **Generation**: Before triggering browser redirects, the C++ engine computes high-entropy cryptographic words using `QRandomGenerator::global()` and packages them into clean base64url encoded variables (`m_csrfState`).
- **Assertion**: When receiving callbacks on the local loophole socket, the server pulls the `state` key parameter and strictly checks it against the cached `m_csrfState`.
- **Enforcement**: Any mismatch or missing parameter immediately kills the TCP loop, closes connection ports, and dispatches a critical error trigger (`AuthError::StateCsrfMismatch`), isolating the platform from malicious triggers.

---

## 3. QOAuthHttpServerReplyHandler Integration

Qt6 provides the elegant **Qt Network Authorization** module which removes low-level socket handling:

- Setup is handled with `QOAuth2AuthorizationCodeFlow` and `QOAuthHttpServerReplyHandler`.
- The reply handler automatically listens to incoming local interface callbacks and coordinates the HTTP headers elegantly.
- No need to parse raw HTTP POST strings or TCP bytes manually!

---

## 4. Secure Keyring Storage via QtKeychain

Plain parent settings registries (like QSettings INI configurations) store persistent keys in non-encrypted plain-text within the Windows Registry, macOS plist files, or standard Linux flat-files, creating high-risk credential-exposure vectors.

To bypass this vulnerability, the client integrates **QtKeychain**, a cross-platform library that interacts directly with secure native operating system keyrings:
- **macOS / iOS**: Interacts directly with the secure **Apple Keychain Services** API.
- **Windows**: Stores credentials securely in the native **Windows Credential Manager**.
- **Linux**: Commits fields using **GNOME Keyring** or **KWallet** storage providers.

### A. Non-Blocking Keyring Writes (Writing Credentials)
When registration finishes successfully, an asynchronous `QKeychain::WritePasswordJob` is prepared. It isolates private client keys and records them onto the device's hardware keychain wrapper:
```cpp
QKeychain::WritePasswordJob *job = new QKeychain::WritePasswordJob("EnterpriseSecurity", this);
job->setKey("oauth_access_token");
job->setPassword(accessToken);
job->start();
```

### B. Dynamic Session Restorations (ReadPasswordJob)
When the application reboots, the client automatically triggers a non-blocking `ReadPasswordJob` query. If a keychain token matches, the user is instantly logged back in without any manual password prompt:
```cpp
QKeychain::ReadPasswordJob *job = new QKeychain::ReadPasswordJob("EnterpriseSecurity", this);
job->setKey("oauth_access_token");
connect(job, &QKeychain::ReadPasswordJob::finished, [this, job]() {
    if (!job->error()) {
        restoreSession(job->text());
    }
});
job->start();
```

### C. Cleardown Wipes (DeletePasswordJob)
Upon hitting standard log outs, a clean `DeletePasswordJob` is executed to physically scrub active keys, preventing any subsequent unauthorized recovery.

---

## 5. Comprehensive Error Classification Matrix

The client models all edge conditions via the `AuthError` enumerator to maintain a bulletproof system:
- **NetworkOffline**: Device reports zero internet connectivity or socket handshakes fail immediately.
- **LocalServerPortInUse**: The configured loopback TCP port is locked by another running system service.
- **StateCsrfMismatch**: Returned security tokens fail the dynamic cryptographically random state match.
- **UserCancelled**: Player cancels authorization screens or closes tab windows before returning keys.
- **TokenExchangeFailed**: Remote security servers return 400 Bad Request or 401 Unauthorized status keys.
- **SslHandshakeFailed**: Discovered server certificates cannot pass platform authority standards.
- **KeychainAccessFailed**: Platform system keyrings are locked or reject storing active tokens.

---

## 6. Multi-Platform CMake Configuration Requirements

To establish cross-platform parity and maintain secure desktop/mobile deployments, the applet supports specific CMake compilation configurations for Desktop, Android, and iOS/iPadOS Tablet targets.

### A. Universal Multi-Platform C++ Build Toolchains
The core `CMakeLists.txt` handles conditional macro injection and package linkage dynamically:
- Discovering modern Qt 6 packages via `find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Network NetworkAuth)`.
- Discovering the secure platform keyring wrapper package via `find_package(qtkeychain REQUIRED)`.
- Mapping native sources: `main.cpp`, `authservice.cpp`, `authservice.h`, and resource compiler bundles `resources.qrc` into a single, unified compilation sequence.

### B. Android Native Compilation Setup (`CMakeLists.android.txt`)
When target compilers direct compilation towards Android targets (`if(ANDROID)` is triggered):
- **Android Package Source Directories**: Defines the directory containing native assets, branding, and permissions via `QT_ANDROID_PACKAGE_SOURCE_DIR`.
- **Preprocessors**: Configures `ANDROID_MOBILE_PLATFORM=1` and `QT_OAUTH_MOBILE_BUILD=1` to bypass standard static desktop localhost loopback ports and route tokens via intent-based mobile URL schemes.
- **Library Linkage**: Links prebuilt or local QtKeychain shared objects built specifically for mobile target ABIs (such as `armeabi-v7a` or `arm64-v8a`).

### C. iOS / iPadOS Tablet Specifications (`CMakeLists.ios.txt`)
For iOS/iPadOS tablet deployments, specific Bundle properties are enforced to conform to Apple security sandboxing:
- **Apple Bundle Payload Generation**: Sets `MACOSX_BUNDLE` to True to package raw executable binaries into an acceptable iOS `.app` container.
- **Info.plist Mappings**: Binds custom parameters such as URL Schemes into Info.plist dynamically via `MACOSX_BUNDLE_INFO_PLIST`.
- **Targeted Device Family (iPad/Tablet Support)**: The property `XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "2"` (or `1,2` for universal scaling) is explicitly integrated. This guarantees the application scales across premium tablet layouts instead of defaulting to a narrow mobile window.
- **Keychain Isolation Bounds**: Because iOS isolates local sandbox keyrings, CMake explicitly configures Xcode signature codes and Entitlements mappings:
  ```cmake
  set_target_properties(QtOAuthClient PROPERTIES
      XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_CURRENT_SOURCE_DIR}/ios/Entitlements.plist"
      XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
      XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "ABC123XYZ4"
  )
  ```
  This enables keychain access group permissions needed to call QtKeychain read/write routines on active iOS devices.
