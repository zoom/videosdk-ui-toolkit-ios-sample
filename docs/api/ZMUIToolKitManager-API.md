# ZMUIToolKitManager API Documentation

## Module Information
- **Module**: ZMUIToolKitManager
- **Platform**: iOS
- **Language**: Swift (with Objective-C interop via `@objc`)
- **Version**: 1.0
- **Framework**: ZMUIToolkit
- **Superclass**: `NSObject`
- **Objective-C name**: `ZMUIToolKitManager`
- **Concurrency**: The lifecycle APIs (`initialize`, `cleanup`) are `@MainActor`. Access `ZMUIToolKitManager` from the main thread.

## Overview

`ZMUIToolKitManager` is the central, **all-static** setup and management class for the ZMUIToolkit framework. It has no instances you need to create; everything is accessed through type-level (`static`) members. Its responsibilities are:

1. **Hold the host's `ZoomVideoSDK` instance** (`videoSDK`) so toolkit components can read session/user state and obtain helpers.
2. **Own the SDK delegate slot.** On `initialize`, it installs a single fan-out delegate (`videoSDKDelegate`, a `CompositeZoomVideoSDKDelegate`) onto `ZoomVideoSDK.delegate`. Toolkit components and integrators register their own `ZoomVideoSDKDelegate` observers via `videoSDKDelegate.add(_:)` rather than reassigning `ZoomVideoSDK.delegate`.
3. **Manage the connected/cleaned-up lifecycle** via `initialize(videoSDK:bundleId:)` and `cleanup()`.
4. **Expose configuration flags** that tune toolkit behavior, notably `cloudRecordingConsentDialogEnabled`.

Because the SDK delegate slot only accepts one delegate, the toolkit centralizes ownership here and fans events out to many weakly-held observers. This is the mechanism every self-managing toolkit component (e.g. `ZMUIToolkitAudioBtn`) relies on.

## Lifecycle

### Prerequisites
- The host application should pass the same `ZoomVideoSDK` instance it initialized successfully (the toolkit does not call `ZoomVideoSDK.initialize`, and it does not infer SDK readiness from `getVersion()` / `getSDKVersion()`).
- `initialize(videoSDK:bundleId:)` must be called on the **main thread** (`@MainActor`) and **before** constructing any toolkit component that reads `ZMUIToolKitManager.videoSDK`.

### States
- **Uninitialized / cleaned-up**: `videoSDK == nil`. This is the state at process start and after `cleanup()`. Toolkit components constructed in this state construct in a default state and tap actions take no action.
- **Initialized**: `videoSDK != nil` and `ZoomVideoSDK.delegate === videoSDKDelegate`. Set by a successful `initialize(...)` call (returns `true`).

### Entry — `initialize(videoSDK:bundleId:)`
Connects the toolkit to the provided SDK and takes ownership of its `delegate` slot:
1. If the manager was still attached to a different SDK instance, it first clears that old SDK's delegate slot when the toolkit still owns it.
2. Stores `bundleId` (empty string if `nil`).
3. Stores the `videoSDK` reference.
4. Assigns `videoSDK.delegate = videoSDKDelegate`.
5. Verifies the delegate slot is now owned by the toolkit. If ownership does not stick, `initialize` returns `false`, clears the stored references, and leaves the toolkit `Uninitialized`.

### Exit — `cleanup()`
Tears the toolkit's hold on the SDK back down, in this exact order:
1. Stops Picture-in-Picture: `ZMUIToolkitPiPController.shared.stop()`. PiP subscriptions are released here so they do not outlive the session.
2. Clears the delegate slot when `videoSDK` is non-nil **and** `videoSDK.delegate === videoSDKDelegate` (the toolkit still owns the slot). If the host has set a different delegate, the toolkit leaves it as-is.
3. Clears the SDK reference and bundle id.

> **Note:** `cleanup()` does **not** clear the registered child observers on `videoSDKDelegate`. The `CompositeZoomVideoSDKDelegate` and its weak observer set survive across `cleanup()` / re-`initialize()`, so listeners do not need to re-register after re-initialization. (Observers are held weakly, so any that have been deallocated are dropped automatically.)

### Delegate Registration Model
There is exactly one fan-out delegate (`videoSDKDelegate`), created once as a `static let` and reused for the lifetime of the process. It is installed onto `ZoomVideoSDK.delegate` by `initialize`. Integrators and toolkit components add/remove their own observers with `videoSDKDelegate.add(_:)` / `videoSDKDelegate.remove(_:)`. Child delegates are held **weakly**, so callers must retain their own observers.

## State Machine

### States

| State | Condition | Meaning |
|-------|-----------|---------|
| `Uninitialized` | `videoSDK == nil`, delegate slot not owned | Initial state; also the state after `cleanup()`. Toolkit components construct in a default state and tap actions take no action. |
| `Initialized` | `videoSDK != nil`, `ZoomVideoSDK.delegate === videoSDKDelegate` | Active state after a successful `initialize(...)` (returned `true`). Toolkit components are fully functional. |

### Transitions

| From | Trigger | Result | To |
|------|---------|--------|----|
| `Uninitialized` | `initialize(videoSDK:bundleId:)` returns `true` | installs `videoSDKDelegate` | `Initialized` |
| `Uninitialized` | `initialize(videoSDK:bundleId:)` returns `false` | clears refs, delegate NOT installed | `Uninitialized` (unchanged) |
| `Initialized` | `cleanup()` | stops PiP, clears owned delegate slot, releases SDK | `Uninitialized` |
| `Initialized` | `initialize(...)` again | if switching SDKs, clears the old owned delegate slot, then replaces the SDK ref and re-installs the delegate. Existing observers retained. | `Initialized` (re-pointed) |

### Query Methods

| State | Query |
|-------|-------|
| `Uninitialized` | `ZMUIToolKitManager.videoSDK == nil` |
| `Initialized` | `ZMUIToolKitManager.videoSDK != nil` (and SDK delegate is `videoSDKDelegate`) |

## APIs

### `initialize(videoSDK:bundleId:)`
```swift
@MainActor
@objc static public func initialize(videoSDK: ZoomVideoSDK, bundleId: String?) -> Bool
```
Connects the toolkit to the host's `ZoomVideoSDK` instance and installs the toolkit's fan-out delegate onto `ZoomVideoSDK.delegate`.

- **Concurrency**: `@MainActor` — must be called on the main thread.
- **Parameters**:
  - `videoSDK` — the `ZoomVideoSDK` instance the host initialized and wants the toolkit to observe.
  - `bundleId` — Broadcast Upload Extension bundle identifier (used by PiP/broadcast paths); `nil` is normalized to `""`.
- **Return value**: `Bool`. `true` = the delegate was installed and the toolkit now owns the SDK delegate slot (now `Initialized`). `false` = the toolkit could not verify delegate ownership after assignment. On `false` the stored references are cleared and the delegate is **not** installed (remains `Uninitialized`). SDK initialization success itself should be validated by the host through `ZoomVideoSDK.initialize(...)`.
- **Side effects**:
  - If re-pointing from another SDK instance, clears that old delegate slot when still owned by the toolkit.
  - Stores `videoSDK` and `bundleId`.
  - On success: `videoSDK.delegate = videoSDKDelegate` (the toolkit takes ownership of the SDK delegate slot).
- **Preconditions**: the host should initialize `videoSDK` first. Call before constructing toolkit components.

### `cleanup()`
```swift
@MainActor
@objc static public func cleanup()
```
Releases the toolkit's hold on the SDK before the host tears the SDK down.

- **Concurrency**: `@MainActor` — must be called on the main thread.
- **Return value**: none.
- **Side effects** (in order):
  1. `ZMUIToolkitPiPController.shared.stop()` — unsubscribes active PiP video/share sources, retires receivers, flushes the display layer, and tears down the `AVPictureInPictureController`.
  2. Clears `ZoomVideoSDK.delegate` when the toolkit still owns it (`sdk.delegate === videoSDKDelegate`).
  3. Sets `videoSDK` to `nil` and `bundleId` to `""`.
- **Does not**: clear registered child observers on `videoSDKDelegate` (they survive, held weakly).
- **Preconditions**: none; safe to call when already `Uninitialized` (PiP stop is idempotent and the delegate-clear is conditional on ownership).

### `videoSDK` (property)
```swift
@objc public static var videoSDK: ZoomVideoSDK? { get }
```
Read-only accessor for the connected SDK instance. `nil` before `initialize` and after `cleanup`. Toolkit components use this to obtain helpers and read session/user state. Access from the main thread.

### `videoSDKDelegate` (property)
```swift
@objc public static let videoSDKDelegate: CompositeZoomVideoSDKDelegate
```
The single, process-lifetime fan-out delegate. Register observers via `videoSDKDelegate.add(_:)` / remove via `videoSDKDelegate.remove(_:)`. This is the **only** supported way to observe SDK events through the toolkit. Created once; not replaced by `cleanup()`.

### `cloudRecordingConsentDialogEnabled` (property)
```swift
@objc public static var cloudRecordingConsentDialogEnabled: Bool { get set }
```
Controls whether participants are shown the **blocking** cloud-recording consent dialog when a recording starts.
- `true` (default): the consent dialog is shown and blocks until the user accepts/declines.
- `false`: the dialog is suppressed, consent is **auto-accepted**, and a non-blocking "Recording started" toast is shown instead.
- **Concurrency**: set on the main thread.

### `bundleId` (property, internal)
```swift
@objc static var bundleId: String { get }
```
Internal read-only accessor returning the bundle id supplied to `initialize` (or `""`). Not part of the public API surface; documented for completeness. Consumed by PiP / broadcast-extension code paths.

## Owned Type — `CompositeZoomVideoSDKDelegate`

`videoSDKDelegate` is an instance of `CompositeZoomVideoSDKDelegate` (Objective-C name `ZMUICompositeZoomVideoSDKDelegate`), a `final` `ZoomVideoSDKDelegate` that forwards every SDK callback to a set of weakly-held child delegates.

### Registration APIs
```swift
@objc(addDelegate:)    public func add(_ delegate: ZoomVideoSDKDelegate)
@objc(removeDelegate:)  public func remove(_ delegate: ZoomVideoSDKDelegate)
```
- **Storage**: observers are held **weakly**. Retain them yourself.
- **Forwarding**: every `ZoomVideoSDKDelegate` method is implemented and forwards to each live child via the optional-method form (`$0.method?(...)`), so observers only implement the callbacks they care about.
- **Threading**: callbacks are delivered on whatever thread the SDK invokes them (in practice the main thread for UI-relevant events); the composite does not hop threads.

### Coverage
The composite forwards the full `ZoomVideoSDKDelegate` surface, grouped by area: session lifecycle (`onSessionJoin`, `onSessionLeave`, `onError`, password), user changes (join/leave/audio/video/host/manager/name), share, live stream / streaming / broadcast, chat, audio raw data, phone / call-out, command channel, camera control, recording (`onCloudRecordingStatus`, `onUserRecordAgreementNotification`), mute / mic test, multi-camera, system / permissions / SSL / proxy, live transcription, network status, annotation, subscribe failures, file transfer, video alpha / spotlight, UVC camera, sub-session, in-app screen share, whiteboard / canvas, and real-time media streams.

## Error Handling

`ZMUIToolKitManager` does not return SDK error codes. The only fallible operation is `initialize`, which signals failure with a `Bool`.

| API | Result | Meaning | Recommended action |
|-----|--------|---------|--------------------|
| `initialize` | `true` | Delegate installed; toolkit owns the SDK delegate slot; state is `Initialized`. | Proceed to construct toolkit components. |
| `initialize` | `false` | The toolkit could not verify delegate ownership after assignment. References cleared; delegate NOT installed. | Verify you passed the live host SDK instance, do not construct toolkit components, and retry `initialize` after correcting the host lifecycle/setup issue. |
| `cleanup` | (void) | Always succeeds; idempotent. | Safe to call multiple times. |

**Recovery strategy**: If `initialize` returns `false`, the toolkit is left in the `Uninitialized` state with no references retained. Re-check the host SDK instance/lifecycle, then call `initialize` again once the host setup issue is resolved.

## Rules

- **REQUIRED**: Call `initialize(videoSDK:bundleId:)` on the main thread, with an already-initialized SDK, before constructing toolkit components.
- **REQUIRED**: Check the `Bool` return value of `initialize`. A `false` means the delegate was not installed and toolkit components will not receive events.
- **REQUIRED**: Call `cleanup()` on the main thread before tearing the SDK down (it releases PiP, which holds an SDK delegate/pipe that must not outlive the session).
- **REQUIRED**: Observe SDK events via `videoSDKDelegate.add(_:)`. Retain your observer — the composite holds it weakly.
- **FORBIDDEN**: Assigning anything directly to `ZoomVideoSDK.delegate` after `initialize`. The toolkit owns that slot; register observers via `videoSDKDelegate.add(_:)`.
- **CHECK** the `Bool` return of `initialize` before constructing toolkit components, but use the host's `ZoomVideoSDK.initialize(...)` result to determine whether the SDK itself initialized successfully.
- **NOTE**: Do not use `getVersion()` / `getSDKVersion()` as your own SDK-initialization check; that probe is no longer the toolkit's validation gate.
- **NOTE**: `cleanup()` does not clear your observers; they persist across re-`initialize()`. Remove them explicitly if you need them gone.

## Examples

### Example 1: Initialize the toolkit (happy path)
```swift
// On the main thread, with an already-initialized ZoomVideoSDK.
let ok = ZMUIToolKitManager.initialize(videoSDK: sdk, bundleId: Bundle.main.bundleIdentifier)
guard ok else {
    // Toolkit did not claim the SDK delegate slot. Do not build toolkit UI.
    return
}
// Now safe to construct toolkit components (they read ZMUIToolKitManager.videoSDK).
let audioButton = ZMUIToolkitAudioBtn()
```

### Example 2: Register a custom SDK event observer
```swift
final class SessionLogger: NSObject, ZoomVideoSDKDelegate {
    func onSessionJoin() { print("joined") }
    func onError(_ errorType: ZoomVideoSDKError, detail details: Int) { print("error \(errorType)") }
}
let logger = SessionLogger()
ZMUIToolKitManager.videoSDKDelegate.add(logger) // held weakly — retain `logger` yourself
// ... later, optionally:
// ZMUIToolKitManager.videoSDKDelegate.remove(logger)
```

### Example 3: Suppress the blocking cloud-recording consent dialog
```swift
// Auto-accept consent and show a non-blocking toast instead of the blocking dialog.
ZMUIToolKitManager.cloudRecordingConsentDialogEnabled = false
```

### Example 4: Teardown before the SDK is destroyed
```swift
// On the main thread, before the host calls ZoomVideoSDK cleanup/uninitialize.
ZMUIToolKitManager.cleanup()
// PiP is stopped, the SDK delegate slot is cleared (if still owned),
// and ZMUIToolKitManager.videoSDK is now nil.
// Registered observers on videoSDKDelegate are NOT removed and survive
// across a subsequent initialize(...).
```

### Example 5: Re-initialize for a new session
```swift
ZMUIToolKitManager.cleanup()              // end previous session
// ... host re-inits ZoomVideoSDK / joins a new session ...
let ok = ZMUIToolKitManager.initialize(videoSDK: sdk, bundleId: bundleId)
// Previously-registered observers (still alive) automatically resume receiving events.
```

## Code Snippets (Reusable Patterns)

### Guarded initialize
```swift
@MainActor
func setupToolkit(_ sdk: ZoomVideoSDK) -> Bool {
    let ok = ZMUIToolKitManager.initialize(videoSDK: sdk, bundleId: Bundle.main.bundleIdentifier)
    if !ok {
        assertionFailure("ZMUIToolKitManager.initialize failed: toolkit could not claim the SDK delegate slot")
    }
    return ok
}
```

### Safe access to the connected SDK
```swift
guard let sdk = ZMUIToolKitManager.videoSDK else {
    // Toolkit not initialized (or already cleaned up). Return early.
    return
}
let mySelf = sdk.getSession()?.getMySelf()
```

### Observer lifecycle (retain + remove)
```swift
final class MyObserver: NSObject, ZoomVideoSDKDelegate { /* implement what you need */ }
private let observer = MyObserver()   // strong property keeps it alive (composite is weak)
func start() { ZMUIToolKitManager.videoSDKDelegate.add(observer) }
func stop()  { ZMUIToolKitManager.videoSDKDelegate.remove(observer) }
```

## Type Definitions

| Type | Kind | Notes |
|------|------|-------|
| `ZMUIToolKitManager` | `@objc` class (`NSObject`) | All-static manager. No instances; access via type-level members. |
| `CompositeZoomVideoSDKDelegate` | `final` class (`ZMUICompositeZoomVideoSDKDelegate`) | Weak fan-out `ZoomVideoSDKDelegate`; `add(_:)` / `remove(_:)`. |
| `ZoomVideoSDK` | SDK class | The host's initialized Video SDK instance; provides session, helpers, and the `delegate` slot. |
| `ZoomVideoSDKDelegate` | SDK protocol | Observer protocol; register implementations via `videoSDKDelegate.add(_:)`. |
| `ZMUIToolkitPiPController` | internal `@MainActor` class | Singleton (`.shared`) owning the AVKit Picture-in-Picture machinery; `stop()` is invoked by `cleanup()`. |
| `Bool` | Swift type | `initialize` return value: `true` = installed, `false` = validation failed. |

## Metadata
- **Source file**: `ZMUIToolkit/ZMUIToolkit/Sources/ZMUIToolKitManager.swift`
- **Related sources**: `ZMUIToolkit/ZMUIToolkit/Sources/CompositeZoomVideoSDKDelegate.swift`, `ZMUIToolkit/ZMUIToolkit/Sources/Video/ZMUIToolkitPiPController.swift`
- **Generated for**: AI-assisted code generation and integration
- **Verification**: Behaviors documented above (initialize return/failure path, conditional delegate clearing in cleanup, weak observer storage, main-thread access, consent-flag semantics, PiP stop ordering) were verified directly against the implementation source.
