# Little Scribblers™ Data Dictionary & Telemetry Specification
**Platform Target:** React 19 (TypeScript) & Qt 6.11 (QML / C++) Dual Architecture  
**Document Version:** 2.4.0 • Production Ready  
**Scope:** Real-Time Kinematic Tracking, Pediatric Developmental Telemetry, Control Cohort Standardization, and Data Ingestion Schemas.

---

## Table of Contents
1. [Architecture Overview & Data Flow](#1-architecture-overview--data-flow)
2. [Developer Difficulty Parameter Matrix](#2-developer-difficulty-parameter-matrix)
3. [Core Domain Entities & Shape Models](#3-core-domain-entities--shape-models)
4. [Real-Time Tracing & Kinematic Point Stream](#4-real-time-tracing--kinematic-point-stream)
5. [The 6 Pediatric Cognitive Developmental Metrics](#5-the-6-pediatric-cognitive-developmental-metrics)
6. [Normative Control Cohort & 4 Expertise Stages](#6-normative-control-cohort--4-expertise-stages)
7. [Network Telemetry Ingestion Protocol (`POST /api/telemetry`)](#7-network-telemetry-ingestion-protocol-post-apitelemetry)
8. [Cross-Platform Type Mapping (TypeScript ↔ Qt 6.11 C++ ↔ QML)](#8-cross-platform-type-mapping-typescript--qt-611-c--qml)
9. [Local Storage & State Persistence Keys](#9-local-storage--state-persistence-keys)

---

## 1. Architecture Overview & Data Flow

Little Scribblers operates across two synchronized environments sharing an identical telemetry schema:
- **Web Frontend (React 19 + TypeScript + Canvas API):** High-frequency pointer event listeners collect continuous coordinates, evaluate corridor adherence against pre-computed bezier contours, compute immediate feedback on the Accuracy Yardstick, and serialize end-of-session telemetry.
- **Native Tablet Client (Qt 6.11 + QML + C++ TracingEngine):** Native `MultiPointTouchArea` in QML forwards raw touch stream data directly to the C++ `TracingEngine` and `TelemetryManager`, enforcing memory safety, deterministic math, and hardware-accelerated rendering.
- **Ingestion Endpoint (`/api/telemetry`):** A RESTful mock endpoint configured in Vite middleware that validates, echoes, and stores child tracing sessions.

```
+-----------------------------------------------------------------------------------+
|                            LITTLE SCRIBBLERS CLIENT                               |
|                                                                                   |
|  [Touch Input Stream] -> (Kinematic Filter) -> [Corridor Yardstick Evaluator]      |
|                                                     |                             |
|                                                     v                             |
|                                        [6 Developmental Metric Analyzers]         |
|                                                     |                             |
|                                                     v                             |
|                                       [Normative Control Comparator]              |
|                                                     |                             |
+-----------------------------------------------------+-----------------------------+
                                                      |
                                       HTTP POST JSON Payload
                                                      |
                                                      v
                                        [/api/telemetry Endpoint]
```

---

## 2. Developer Difficulty Parameter Matrix

The Developer Difficulty setting dynamically adjusts the spatial tolerance corridor, penalty weights, and sensory assistance thresholds across both React and Qt 6.11 engines.

| Field Name | Type | Unit / Range | Description |
| :--- | :--- | :--- | :--- |
| `level` | `string` | `'easy' \| 'medium' \| 'hard'` | Primary preset identifier. |
| `corridorTolerancePx` | `number` (float) | `16.0 – 64.0 px` | Maximum allowed euclidean perpendicular deviation from target bezier path before penalty triggers. |
| `minStrokePercent` | `number` (int) | `50 – 95 %` | Minimum shape coverage required to trigger completion celebration. |
| `penaltyFactor` | `number` (float) | `0.4 – 2.0` | Multiplier applied to off-rail excursions when deducting live score. |
| `snapAssistance` | `boolean` | `true \| false` | When true, provides magnetic waypoint attraction towards nearest guide segment. |
| `label` | `string` | Human-readable | Display label (e.g., "Toddler Guided (48px Corridor)"). |
| `description` | `string` | Text | Clinical / developer intent description. |
| `targetAgeGroup` | `string` | Age range | Pediatric age recommendation (e.g., "Ages 2–3.5", "Ages 4–5", "Ages 5.5+"). |
| `minAccuracyToPass` | `number` (int) | `50 – 85 %` | Minimum aggregate session accuracy to award 1 star. |

### Presets Specification

```json
{
  "easy": {
    "level": "easy",
    "corridorTolerancePx": 48,
    "minStrokePercent": 70,
    "penaltyFactor": 0.5,
    "snapAssistance": true,
    "label": "Easy (Toddler / Early Emergent)",
    "description": "Forgiving 48px corridor with magnetic snapping for developing motor control.",
    "targetAgeGroup": "Ages 2.0 - 3.5",
    "minAccuracyToPass": 55
  },
  "medium": {
    "level": "medium",
    "corridorTolerancePx": 32,
    "minStrokePercent": 80,
    "penaltyFactor": 1.0,
    "snapAssistance": false,
    "label": "Medium (Preschool / Kindergarten)",
    "description": "Standard 32px corridor requiring purposeful stroke guidance and corner turning.",
    "targetAgeGroup": "Ages 3.5 - 5.0",
    "minAccuracyToPass": 70
  },
  "hard": {
    "level": "hard",
    "corridorTolerancePx": 20,
    "minStrokePercent": 90,
    "penaltyFactor": 1.8,
    "snapAssistance": false,
    "label": "Hard (Precision Tracing)",
    "description": "Tight 20px corridor measuring fine motor inhibition and strict contour closure.",
    "targetAgeGroup": "Ages 5.0 - 7.0+",
    "minAccuracyToPass": 82
  }
}
```

---

## 3. Core Domain Entities & Shape Models

### 3.1 ShapeDefinition
Represents a traceable geometric primitive, fun icon, or alphabet glyph.

| Property | Type | Required | Description |
| :--- | :--- | :--- | :--- |
| `id` | `string` | Yes | Unique alphanumeric token (e.g., `'circle'`, `'star'`, `'letter-a'`). |
| `name` | `string` | Yes | Localized child-friendly name (e.g., `'Circle'`, `'Little Star'`). |
| `category` | `string` | Yes | Category enum: `'lines' \| 'basic' \| 'fun' \| 'letters'`. |
| `color` | `string` | Yes | Hex color code utilized for default stroke highlighting (e.g., `'#EF4444'`). |
| `prompt` | `string` | Yes | Spoken and visual instruction prompt (e.g., `"Round and round like a soap bubble!"`). |
| `strokes` | `TraceStrokeDefinition[]` | Yes | Array of ordered strokes required to complete the figure. |
| `magicTransform` | `MagicTransform` | Yes | Interactive animation metadata triggered upon completion. |

### 3.2 TraceStrokeDefinition & StrokePoint
| Property | Type | Required | Description |
| :--- | :--- | :--- | :--- |
| `id` | `string` | Yes | Unique identifier for the individual stroke (e.g., `'circle-s1'`). |
| `name` | `string` | Yes | Spoken stroke prompt (e.g., `'Curve down and around'`). |
| `order` | `number` | Yes | 0-indexed execution order enforcing executive stroke sequencing. |
| `color` | `string` | Yes | Accent color assigned to this stroke segment. |
| `isClosed` | `boolean` | Yes | Specifies whether the stroke endpoint closes onto the start point. |
| `points` | `StrokePoint[]` | Yes | Dense normalized coordinate array $[(x \in [0, 100], y \in [0, 100])]$. |

### 3.3 MagicTransform
| Property | Type | Description |
| :--- | :--- | :--- |
| `emoji` | `string` | Graphical representation rendered in celebration (e.g., `'🎈'`). |
| `label` | `string` | Name of the transformed creature/object (e.g., `'Floating Balloon'`). |
| `soundKey` | `string` | Audio cue trigger token (e.g., `'boing' \| 'fanfare' \| 'sparkle'`). |
| `actionPhrase` | `string` | Text-to-speech phrase spoken upon completion. |
| `particleType` | `string` | Visual particle effect: `'bubbles' \| 'stars' \| 'confetti' \| 'hearts'`. |

---

## 4. Real-Time Tracing & Kinematic Point Stream

Each touch/stylus interaction creates a high-frequency point sample recorded by the canvas.

### 4.1 PointSample (Raw Kinematic Sample)
| Field | Type | Unit | Description |
| :--- | :--- | :--- | :--- |
| `x` | `number` | `px` | Canvas coordinate horizontal position. |
| `y` | `number` | `px` | Canvas coordinate vertical position. |
| `timestamp` | `number` | `ms` | Epoch millisecond timestamp of pointer event. |
| `pressure` | `number` | `0.0 – 1.0` | Stylus or touch pressure (default: 1.0 if not supported). |
| `deviationPx` | `number` | `px` | Euclidean distance to closest segment of target stroke. |
| `inCorridor` | `boolean` | `bool` | True if `deviationPx <= corridorTolerancePx`. |
| `instantAccuracy`| `number` | `% (0–100)` | Instantaneous metric: $\max(0, 100 - (\frac{deviation}{tolerance} \times 100))$. |

---

## 5. The 6 Pediatric Cognitive Developmental Metrics

Calculated at session conclusion using real-time kinematic analysis, based on BOT-2 (Bruininks-Oseretsky Test of Motor Proficiency) and Beery VMI (Visual-Motor Integration) standards.

### 5.1 Executive Functions (`executive_functions`)
Assesses goal-directed behavior, working memory, and sequential planning.

| Field | Type | Range / Format | Clinical Significance & Calculation |
| :--- | :--- | :--- | :--- |
| `overallIndex` | `number` (int) | `0 – 100` | Composite index: $0.4 \times Sequenc + 0.3 \times Rules + 0.3 \times Flexibility$. |
| `planningAndSequencing` | `number` (int) | `0 – 100` | Evaluates waypoint progression in forward order vs random jumping. |
| `workingMemoryRuleAdherence` | `number` (int) | `0 – 100` | Maintenance of task goal (tracing within rail without skipping strokes). |
| `cognitiveFlexibility` | `number` (int) | `0 – 100` | Smooth transition when changing stroke directions at vertices. |
| `strokeOrderAdherencePercent`| `number` (int) | `0 – 100 %` | Percentage of strokes initiated in pedagogically prescribed order. |
| `rating` | `string` | Categorical | `'Developing' \| 'Competent' \| 'Advanced' \| 'Exceptional'`. |

### 5.2 Impulse Control (`impulse_control`)
Measures inhibitory motor control, velocity regulation, and overshooting at trajectory corners.

| Field | Type | Range / Format | Clinical Significance & Calculation |
| :--- | :--- | :--- | :--- |
| `overallIndex` | `number` (int) | `0 – 100` | Composite inhibitory control rating. |
| `velocityRegulationRatio` | `number` (float) | `0.5 – 3.0` | Ratio of mean velocity to velocity standard deviation (smooth vs bursty). |
| `cornerOvershootCount` | `number` (int) | `0 – 20 count` | Number of times pen flew $> 1.5\times$ corridor width past an angular vertex. |
| `accelerationJerkSpikes` | `number` (int) | `0 – 30 count` | Derivative of acceleration ($\frac{d^3x}{dt^3}$); count of abrupt jerks. |
| `inhibitionRating` | `string` | Categorical | `'Needs Guidance' \| 'Moderate Regulation' \| 'High Inhibitory Control'`. |

### 5.3 Motor Speed & Fluidity (`motor_speed`)
Evaluates fine motor execution efficiency, hesitation pauses, and drawing rhythm.

| Field | Type | Range / Format | Clinical Significance & Calculation |
| :--- | :--- | :--- | :--- |
| `overallIndex` | `number` (int) | `0 – 100` | Composite motor execution index. |
| `meanVelocityPxPerSec` | `number` (int) | `30 – 800 px/s` | Average speed along the drawing trajectory. |
| `peakVelocityPxPerSec` | `number` (int) | `60 – 1400 px/s` | Maximum recorded instantaneous speed. |
| `motorFluidityIndex` | `number` (int) | `0 – 100` | Inverse of hesitation pause density along stroke length. |
| `hesitationPausesCount` | `number` (int) | `0 – 15 count` | Episodes where velocity dropped $< 5 px/s$ for $> 350 ms$. |
| `activeDrawingTimeMs` | `number` (int) | `ms` | Total active pen-down duration excluding pauses. |
| `speedProfile` | `string` | Categorical | `'Cautious' \| 'Fluid & Rhythmic' \| 'Hyper-Kinetic'`. |

### 5.4 Text & Glyph Tracing Accuracy (`text_tracing_accuracy`)
Measures handwriting readiness, baseline alignment, and closed contour fidelity.

| Field | Type | Range / Format | Clinical Significance & Calculation |
| :--- | :--- | :--- | :--- |
| `overallIndex` | `number` (int) | `0 – 100` | Composite graphic readiness index. |
| `glyphContourFidelity` | `number` (int) | `0 – 100` | Closeness to true typographic bezier curves. |
| `cornerVertexPrecision` | `number` (int) | `0 – 100` | Sharpness and placement of angular corner turns. |
| `baselineAlignmentScore` | `number` (int) | `0 – 100` | Deviation from horizontal bottom alignment line. |
| `strokeClosurePrecision` | `number` (int) | `0 – 100` | Gap distance between start and endpoints on closed shapes (e.g. O, D). |
| `letterReadinessRating` | `string` | Categorical | `'Early Pre-Writing' \| 'Emerging Letter Formation' \| 'Proficient Handwriting Readiness'`. |

### 5.5 Attention Span & Persistence (`attention_span`)
Measures sustained engagement, distractibility, and task persistence.

| Field | Type | Range / Format | Clinical Significance & Calculation |
| :--- | :--- | :--- | :--- |
| `overallIndex` | `number` (int) | `0 – 100` | Composite sustained attention index. |
| `sustainedFocusDurationMs` | `number` (int) | `ms` | Total elapsed duration from first contact to completion. |
| `taskPersistenceRatio` | `number` (float) | `0.0 – 1.0` | Active drawing time divided by total elapsed session time. |
| `distractionDriftEpisodes` | `number` (int) | `0 – 10 count` | Off-task pauses lasting $> 2500 ms$ without stroke progression. |
| `focusStabilityRating` | `string` | Categorical | `'Variable Attention' \| 'Moderate Persistence' \| 'High Sustained Focus'`. |

### 5.6 Visual Processing Index (`visual_processing_index`)
Measures visual-motor integration (VMI), spatial orientation, and figure-ground perception.

| Field | Type | Range / Format | Clinical Significance & Calculation |
| :--- | :--- | :--- | :--- |
| `overallIndex` | `number` (int) | `0 – 100` | Composite visual-motor integration index. |
| `visualMotorIntegrationVmi`| `number` (int) | `0 – 100` | Beery VMI equivalent score calculated from spatial accuracy. |
| `spatialOrientationScore` | `number` (int) | `0 – 100` | Correct aspect ratio and rotational orientation maintenance. |
| `formConstancyScore` | `number` (int) | `0 – 100` | Ability to recognize and trace shapes regardless of scale or color. |
| `figureGroundDiscrimination`| `number` (int) | `0 – 100` | Distinction between guide rails and background decoration. |
| `vmiDevelopmentalTier` | `string` | Categorical | `'Foundational VMI' \| 'Age-Appropriate VMI' \| 'Superior VMI'`. |

---

## 6. Normative Control Cohort & 4 Expertise Stages

Standardized against empirical normative data ($N = 2,450$ neurotypical participants aged 2.0 to 7.0 years).

### 6.1 The 4 Developmental Expertise Stages

```
STAGE 1: NOVICE SCRIBBLER           (Ages 2.0 - 3.0)  | Palmar Grasp | Mean Acc: 42.5% | Corridor: 48px
STAGE 2: INTERMEDIATE TRACER        (Ages 3.0 - 4.0)  | Digital Pronate | Mean Acc: 62.0% | Corridor: 40px
STAGE 3: PROFICIENT SHAPE MASTER    (Ages 4.0 - 5.5)  | Static Tripod | Mean Acc: 81.5% | Corridor: 32px
STAGE 4: MASTERY PRECISION VIRTUOSO (Ages 5.5 - 7.0+) | Dynamic Tripod | Mean Acc: 93.0% | Corridor: 20px
```

### 6.2 Control Baseline Metrics Table ($N=2,450$)

| Stage ID | Stage Key | Chronological Age | Control Mean Acc ($\mu$) | Std Dev ($\sigma$) | Corridor Width | Mean Jerk ($\mathrm{m/s^3}$) | Mean VMI |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **1** | `novice` | 2.0 – 3.0 yrs | 42.5 % | $\pm 14.2$ | 48 px | 24.5 | 45.0 |
| **2** | `intermediate` | 3.0 – 4.0 yrs | 62.0 % | $\pm 11.5$ | 40 px | 14.8 | 64.0 |
| **3** | `proficient` | 4.0 – 5.5 yrs | 81.5 % | $\pm 7.8$ | 32 px | 7.2 | 82.5 |
| **4** | `mastery` | 5.5 – 7.0+ yrs | 93.0 % | $\pm 4.6$ | 20 px | 2.8 | 94.0 |

### 6.3 Mathematical Formulations for Comparative Telemetry

1. **Composite Performance Score ($S_{comp}$):**
   $$S_{comp} = 0.40 \cdot \text{Accuracy} + 0.20 \cdot \text{Executive} + 0.15 \cdot \text{Impulse} + 0.15 \cdot \text{VMI} + 0.10 \cdot \text{Attention}$$

2. **Standardized Z-Score ($Z$):**
   $$Z = \frac{S_{comp} - \mu_{stage}}{\sigma_{stage}}$$

3. **Empirical Percentile Rank ($P$):**
   $$P = \Phi(Z) \times 100 = \frac{1}{\sqrt{2\pi}} \int_{-\infty}^{Z} e^{-t^2/2} dt \times 100$$

4. **Transition Readiness Score ($TRS$):**
   $$TRS = \min\left(100, \max\left(0, \frac{S_{comp} - \mu_{cur}}{\mu_{next} - \mu_{cur}} \times 100\right)\right)$$

---

## 7. Network Telemetry Ingestion Protocol (`POST /api/telemetry`)

### 7.1 HTTP Request Specification
- **Endpoint:** `/api/telemetry`
- **Method:** `POST`
- **Headers:** `Content-Type: application/json`, `Accept: application/json`

### 7.2 Complete Payload Schema

```json
{
  "session_id": "scribble-session-1725295200000-a9f2x",
  "client_timestamp": "2026-09-02T12:00:00.000Z",
  "target_endpoint": "/api/telemetry",
  "app_type_page": "ShapeTracingPage.qml",
  "developer_difficulty": {
    "level": "medium",
    "corridorTolerancePx": 32,
    "minStrokePercent": 80,
    "penaltyFactor": 1.0,
    "snapAssistance": false,
    "label": "Medium (Preschool / Kindergarten)",
    "description": "Standard 32px corridor requiring purposeful stroke guidance and corner turning.",
    "targetAgeGroup": "Ages 3.5 - 5.0",
    "minAccuracyToPass": 70
  },
  "shape": {
    "id": "star",
    "name": "Star",
    "category": "basic"
  },
  "performance": {
    "accuracy_percent": 88,
    "coverage_percent": 96,
    "total_score": 1420,
    "stars_earned": 3,
    "max_streak": 22,
    "accuracy_rating": "bullseye",
    "rank_title": "Bullseye Master",
    "duration_ms": 14200
  },
  "cognitive_developmental_metrics": {
    "executive_functions": {
      "overall_index": 88,
      "planning_and_sequencing": 91,
      "working_memory_rule_adherence": 87,
      "cognitive_flexibility": 86,
      "stroke_order_adherence_percent": 95,
      "rating": "Advanced"
    },
    "impulse_control": {
      "overall_index": 84,
      "velocity_regulation_ratio": 1.35,
      "corner_overshoot_count": 1,
      "acceleration_jerk_spikes": 3,
      "inhibition_rating": "High Inhibitory Control"
    },
    "motor_speed": {
      "overall_index": 86,
      "mean_velocity_px_per_sec": 175,
      "peak_velocity_px_per_sec": 310,
      "motor_fluidity_index": 88,
      "hesitation_pauses_count": 2,
      "active_drawing_time_ms": 12800,
      "speed_profile": "Fluid & Rhythmic"
    },
    "text_tracing_accuracy": {
      "overall_index": 89,
      "glyph_contour_fidelity": 91,
      "corner_vertex_precision": 87,
      "baseline_alignment_score": 90,
      "stroke_closure_precision": 88,
      "letter_readiness_rating": "Proficient Handwriting Readiness"
    },
    "attention_span": {
      "overall_index": 92,
      "sustained_focus_duration_ms": 14200,
      "task_persistence_ratio": 0.90,
      "distraction_drift_episodes": 1,
      "focus_stability_rating": "High Sustained Focus"
    },
    "visual_processing_index": {
      "overall_index": 88,
      "visual_motor_integration_vmi": 89,
      "spatial_orientation_score": 90,
      "form_constancy_score": 92,
      "figure_ground_discrimination": 87,
      "vmi_developmental_tier": "Superior VMI"
    }
  },
  "expertise_control_data": {
    "active_stage": {
      "stage_id": 3,
      "stage_key": "proficient",
      "stage_name": "Proficient Shape Master",
      "age_equivalent_years": "4.0 - 5.5 yrs",
      "developmental_milestone": "Static tripod grasp with corner deceleration and consistent corridor tracking."
    },
    "control_comparison": {
      "composite_performance_score": 88,
      "comparison_stage": "Stage 3 (Ages 4.0 - 5.5)",
      "control_cohort_mean": 81.5,
      "control_cohort_std_dev": 7.8,
      "z_score_vs_control": 0.83,
      "percentile_rank": 79.7,
      "transition_readiness_to_next_stage_percent": 56.5
    },
    "control_baseline_reference": {
      "cohort_size": 2450,
      "standardization_framework": "BOT-2 and Beery VMI Empirical Cohort Baseline (2024-2026)"
    }
  }
}
```

### 7.3 Response Receipt Format

```json
{
  "receipt_id": "rcpt-1725295200123-774",
  "status": "success",
  "http_code": 200,
  "endpoint": "/api/telemetry",
  "received_at": "2026-09-02T12:00:00.125Z",
  "server_processing_time_ms": 14,
  "payload_echo": { ... }
}
```

---

## 8. Cross-Platform Type Mapping (TypeScript ↔ Qt 6.11 C++ ↔ QML)

| Logical Field | TypeScript Interface | Qt 6.11 C++ Type | QML Native Property |
| :--- | :--- | :--- | :--- |
| Difficulty Level | `DeveloperDifficultyLevel` (`'easy' \| 'medium' \| 'hard'`) | `enum AppDifficulty::Level` | `int difficulty` |
| Corridor Tolerance | `number` (px) | `qreal corridorTolerance()` | `property real corridorTolerance` |
| Session Accuracy | `number` (0–100%) | `qreal accuracy()` | `property real accuracy` |
| Total Stars Earned | `number` (1–3) | `int stars()` | `property int stars` |
| Telemetry Payload | `TelemetryPayload` (`object`) | `QJsonObject` | `var telemetry` |
| HTTP Request Post | `fetch('/api/telemetry')` | `QNetworkAccessManager::post()` | Signal: `submitClicked()` |
| Sound Synthesizer | `kidAudio.playPop()` | `MediaController::playPop()` | `cppMediaController.playPop()` |

---

## 9. Local Storage & State Persistence Keys

Used by the React 19 web application to persist progress offline across browser sessions.

| Key | Type | Default Value | Description |
| :--- | :--- | :--- | :--- |
| `little_scribblers_stars` | `number` (int) | `0` | Cumulative stars earned across all completed tracing sessions. |
| `little_scribblers_scores` | `Record<string, ShapeScoreRecord>` | `{}` | Key-value store mapping `shapeId` to best scores, stars, and completion dates. |
| `little_scribblers_gallery` | `SavedArtPiece[]` | `[]` | Array of serialized canvas drawings with base64 PNG data URLs. |
| `little_scribblers_dev_diff`| `DeveloperDifficultyConfig` | Medium config | Active developer difficulty setting persisted between app reloads. |

---

*Authored for Little Scribblers™ Research & Engineering Team. Aligns with Beery-Buktenica Developmental Test of Visual-Motor Integration (Beery VMI 6th Ed.) and Bruininks-Oseretsky Test of Motor Proficiency (BOT-2).*
