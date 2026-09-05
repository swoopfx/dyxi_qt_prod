# Little Scribblers - Qt 6.11 / QML Edition

An interactive, kid-friendly shape tracing and scribbling application converted to **Qt 6.11 / QML** with a dedicated C++ backend for heavy-duty processing, media synthesis, and JSON telemetry transmission.

## Architecture

1. **C++ Heavy-Duty Backend (`/backend`)**:
   - `TracingEngine`: Handles point decimation, spline interpolation, raycasting Euclidean distance from ideal path rails, and real-time precision scoring.
   - `MediaController`: Coordinates audio playback (`QMediaPlayer`, `QAudioOutput`) and speech synthesis.
   - `TelemetryManager`: Packages tracing and session metrics into a standardized JSON payload and dispatches it via `QNetworkAccessManager` to a dummy endpoint upon clicking **Submit**.
   - `AppDifficulty`: Defines developer difficulty parameters (`Easy`, `Medium`, `Hard`) including corridor tolerance in pixels, minimum accuracy thresholds, speed factors, and magnetic snapping.

2. **QML Frontend Pages (`/qml`)**:
   - Each App Type has its own dedicated page accepting `difficulty` as a parameter entered by the developer:
     - `ShapeTracingPage.qml` (`property int difficulty: AppDifficulty.Level.Medium`)
     - `FreeScribblePage.qml` (`property int difficulty: AppDifficulty.Level.Easy`)
     - `ShapeSelectorPage.qml` (`property int difficulty: AppDifficulty.Level.Medium`)
     - `ArtGalleryPage.qml` (`property int difficulty: AppDifficulty.Level.Easy`)
     - `CelebrationDialog.qml` featuring the **Submit** button for telemetry dispatch.
     - `YardstickRuler.qml` providing a real-time accuracy meter.

## Building and Running

### Prerequisites
- Qt 6.11 or newer (with Qt Quick, QML, Network, and Multimedia modules)
- CMake 3.22+
- C++20 compliant compiler (GCC 11+, Clang 13+, or MSVC 2022)

```bash
# Configure the build
cmake -B build -S . -DCMAKE_PREFIX_PATH=/path/to/Qt/6.11.0/gcc_64

# Compile the application
cmake --build build --config Release

# Run
./build/appLittleScribblers
```

## Dummy Telemetry Endpoint

When the user finishes tracing and clicks the **Submit** button in `CelebrationDialog.qml`, `TelemetryManager` gathers the session telemetry into JSON and dispatches it via HTTP POST to the configured endpoint (default: `/api/telemetry` or `https://httpbin.org/post`).
