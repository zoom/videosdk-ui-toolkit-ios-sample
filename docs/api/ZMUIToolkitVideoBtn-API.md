# ZMUIToolkitVideoBtn API Documentation

## Module Information
- **Module**: ZMUIToolkitVideoBtn
- **Platform**: iOS
- **Language**: Swift (with Objective-C interop via `@objc`)
- **Version**: 1.0
- **Framework**: ZMUIToolkit
- **Superclass**: `ZMUIToolkitIconBtn` (which subclasses `UIButton`)
- **Conforms to**: `ZoomVideoSDKDelegate`
- **Concurrency**: `@MainActor` — all members must be accessed on the main thread.

## Overview

`ZMUIToolkitVideoBtn` is a drop-in, self-managing camera/video control button for sessions built on the Zoom Video SDK. Once added to a view, it:

1. Reads the local participant's current video status from the SDK and renders the matching icon.
2. On tap, starts or stops the local participant's video based on the current state.
3. Registers itself as a `ZoomVideoSDKDelegate` observer and **automatically keeps its icon in sync** with the authoritative SDK video state (e.g. when video status changes for any reason).

The button is icon-only and has two visual/behavioral states represented by the private `VideoButtonType` enum. Unlike `ZMUIToolkitAudioBtn`, there is **no separate "join" state** — video is simply on (`unmute`) or off (`mute`).

## Lifecycle

### Prerequisites
- The Zoom Video SDK must be initialized **and** registered with the toolkit via `ZMUIToolKitManager.initialize(videoSDK:bundleId:)` **before** creating the button. The button reads `ZMUIToolKitManager.videoSDK` during `init()` to determine its initial state and obtains the video helper from it on tap.
- If `ZMUIToolKitManager.videoSDK` is `nil` (toolkit not initialized or already cleaned up), the button is constructed in the `mute` state (because `videoStatus()?.on` resolves to `false`) and tap actions take no effect (the callback proceeds only when the video helper, video status, and self are non-nil).

### Entry / Exit
- **Construction** (`init()`): determines initial state from `getMySelf()?.getVideoCanvas()?.videoStatus()?.on`, builds the underlying `ZMUIToolkitIconBtn` descriptor, calls `super.init`, **adds itself** to `ZMUIToolKitManager.videoSDKDelegate`, then calls `syncCurrentState()`.
- **Attachment** (`didMoveToWindow()`): re-syncs the icon to the current SDK video state, ensuring correctness even if state changed between construction and display.
- **Destruction** (`deinit`): **removes itself** from `ZMUIToolKitManager.videoSDKDelegate`. Delegates are held weakly by `CompositeZoomVideoSDKDelegate`, so this is defensive cleanup rather than a strict requirement.

### Delegate Registration Model
The button does **not** assign itself to `ZoomVideoSDK.delegate`. Instead it registers with the toolkit's fan-out delegate (`ZMUIToolKitManager.videoSDKDelegate`, a `CompositeZoomVideoSDKDelegate`). This allows multiple toolkit components to observe SDK events simultaneously. Child delegates are stored in a weak hash table.

## State Machine

### States (`VideoButtonType`, internal enum, raw `Int`)

| State | Raw value | Meaning | Icon (default asset) |
|-------|-----------|---------|----------------------|
| `mute` | 1 | Local video is **off** (`videoStatus().on == false`) | `VideoMuted` |
| `unmute` | 2 | Local video is **on** (live camera) | `Video` |

> Note: `.unmute` is the "video on/live" state; tapping in this state will **stop video** (turn it off). `.mute` is the "video off" state; tapping will **start video** (turn it on). The enum case names describe the *current* state, not the action the tap performs.

### Transitions

Two transition drivers exist: **optimistic** (immediately after a successful API call in the tap handler) and **authoritative** (via SDK delegate callbacks / lifecycle, which is the source of truth).

| From | Trigger | Action attempted | To (on success) |
|------|---------|------------------|-----------------|
| `unmute` (video on) | tap | `videoHelper.stopVideo()` (updates immediately) | `mute` |
| `mute` (video off) | tap | `videoHelper.startVideo()` | `unmute` |
| any | `onSessionJoin` callback | resync from SDK | matches SDK status |
| any | `onUserVideoStatusChanged` (local user only) | resync from SDK | matches SDK status |
| any | `didMoveToWindow` | resync from SDK | matches SDK status |

Tapping toggles the camera: both stop and start update the icon immediately on success. In all cases the displayed state is reconciled by `onUserVideoStatusChanged`. On any tap action, the icon updates only when the SDK call returns `.Errors_Success`.

> Threading note: The current state is decided in the tap callback by reading `videoStatus()?.on`. Both `stopVideo()` and `startVideo()` are invoked directly from the button's `@MainActor` tap path, and the optimistic `switchTo(...)` updates run on the main thread.

### Resync rule (`syncCurrentState`)
```
videoStatus().on == true  -> switchTo(.unmute)
otherwise (false or nil)  -> switchTo(.mute)
```

## APIs

### `init()`
```swift
@objc public init()
```
Designated public initializer. Reads the local participant's video status (`getMySelf()?.getVideoCanvas()?.videoStatus()?.on`) to choose the initial state/icon, constructs the button (40×40, clear background, `secondaryLabel` icon color, zero icon padding), registers as a delegate observer, and syncs state.

- **Preconditions**: Toolkit initialized via `ZMUIToolKitManager.initialize` (recommended). Must be called on the main thread (`@MainActor`).
- **Side effects**: Adds `self` to `ZMUIToolKitManager.videoSDKDelegate`.
- **Returns**: A configured button instance.

### `init?(coder:)`
```swift
@MainActor required public init?(coder: NSCoder)
```
- **Not supported.** Calls `fatalError`. Do **not** instantiate this button from a Storyboard/XIB.

### `setMuteVideoIcon(image:)`
```swift
public func setMuteVideoIcon(image: UIImage)
```
Overrides the icon used for the `mute` (video off) state. If the button is currently in the `mute` state, the new icon is applied immediately.
- **Parameters**: `image` — the replacement video-off-state icon.

### `setUnmuteVideoIcon(image:)`
```swift
public func setUnmuteVideoIcon(image: UIImage)
```
Overrides the icon used for the `unmute` (video on / live camera) state. Applied immediately if currently in the `unmute` state.
- **Parameters**: `image` — the replacement video-on-state icon.

### `switchTo(type:)` *(internal)*
```swift
internal func switchTo(type: VideoButtonType)
```
Sets `currentType` and updates the displayed image to match. Internal — not part of the public API surface; documented for completeness.

### `syncCurrentState()` *(private)*
Recomputes state from the SDK's current video status and calls `switchTo(type:)`. Invoked from `init()`, `didMoveToWindow()`, `onSessionJoin()`, and `onUserVideoStatusChanged(...)`.

### `didMoveToWindow()`
```swift
public override func didMoveToWindow()
```
UIView override. Calls `super.didMoveToWindow()` then `syncCurrentState()` to refresh the icon to the current SDK video state when the button is attached to the view hierarchy.

### Inherited configuration (from `ZMUIToolkitIconBtn`)
The following commonly-used setters are inherited and may be used to style the button: `setBackgroundColor(color:)`, `setRadius(radius:)`, `setSize(size:)`, `setPosition(x:y:)`, `setIconColor(color:)`, `setIcon(icon:)`, `setIconSize(_:)`, `setIconPadding(padding:)`, `setAccessibility(label:hint:)`. (Note: `setIcon`/`setImage` set the raw image directly and will be overwritten on the next state resync — prefer the per-state `set*VideoIcon` methods above.)

## Callbacks (`ZoomVideoSDKDelegate`)

The button implements two delegate methods. Both are invoked on the main thread via the toolkit's `CompositeZoomVideoSDKDelegate` fan-out.

### `onSessionJoin()`
```swift
public func onSessionJoin()
```
Fired when the local user joins the session. Calls `syncCurrentState()` to render the correct initial video icon.
- **State change**: resyncs to match SDK video status.

### `onUserVideoStatusChanged(_:user:)`
```swift
public func onUserVideoStatusChanged(_ helper: ZoomVideoSDKVideoHelper?, user users: [ZoomVideoSDKUser]?)
```
Fired whenever any user's video status changes. The button **filters to the local user only**: it resyncs only if the changed user list contains the local user's ID. This is the primary mechanism keeping the icon authoritative (e.g. video started/stopped externally, by the host, or by the system).
- **State change**: resyncs to match SDK video status, but only for local-user changes.
- Takes no effect when `users` is nil, the local user ID can't be resolved, or the local user is not in the changed list.

## Error Handling

The tap callback inspects the `ZoomVideoSDKError` returned by each video-helper call. Behavior:

| Action | Calls | On `.Errors_Success` | On other result |
|--------|-------|----------------------|----------------|
| Stop video (from `unmute`) | `stopVideo()` (updates immediately) | `switchTo(.mute)` | icon unchanged; reconciled by delegate resync |
| Start video (from `mute`) | `startVideo()` | `switchTo(.unmute)` | icon unchanged; reconciled by delegate resync |

**Reconciliation**: The displayed state is reconciled by `onUserVideoStatusChanged`. To respond to outcomes, observe SDK events via your own delegate registered with `ZMUIToolKitManager.videoSDKDelegate.add(_:)`.

## Rules

- **REQUIRED**: Initialize the toolkit (`ZMUIToolKitManager.initialize`) before constructing the button for correct state and functional taps.
- **REQUIRED**: Construct and use the button on the main thread (`@MainActor`).
- **FORBIDDEN**: Instantiating via `init?(coder:)` / Interface Builder (traps with `fatalError`).
- **DO NOT** assign the button (or anything) directly to `ZoomVideoSDK.delegate`; the toolkit owns that slot. Use `ZMUIToolKitManager.videoSDKDelegate.add(_:)`.
- **AVOID** calling inherited `setIcon`/`setImage` to represent video state — it is transient and overwritten on the next resync. Use `setMuteVideoIcon` / `setUnmuteVideoIcon`.

## Examples

### Example 1: Add the button to a toolbar (happy path)
```swift
// Assumes ZMUIToolKitManager.initialize(videoSDK:bundleId:) was already called.
let videoButton = ZMUIToolkitVideoBtn()
videoButton.setSize(size: CGSize(width: 44, height: 44))
toolbarStackView.addArrangedSubview(videoButton)
// No further wiring needed: the button self-registers for video status updates
// and toggles start/stop video on tap.
```

### Example 2: Customize the per-state icons
```swift
let videoButton = ZMUIToolkitVideoBtn()
videoButton.setMuteVideoIcon(image: UIImage(named: "custom_video_off")!)
videoButton.setUnmuteVideoIcon(image: UIImage(named: "custom_video_on")!)
// If the current state matches one of these, its icon updates immediately.
```

### Example 3: Observe SDK outcomes yourself
```swift
final class VideoErrorObserver: NSObject, ZoomVideoSDKDelegate {
    func onError(_ errorType: ZoomVideoSDKError, detail details: Int) {
        // Show retry UI, logging, etc.
    }
}
let observer = VideoErrorObserver()
ZMUIToolKitManager.videoSDKDelegate.add(observer) // held weakly — retain `observer` yourself
```

### Example 4: Lifecycle / teardown
```swift
// The button removes itself from the delegate fan-out in deinit.
// Just release your reference (e.g. remove from view hierarchy) when done.
videoButton.removeFromSuperview()
// videoButton = nil  // deinit -> ZMUIToolKitManager.videoSDKDelegate.remove(self)
```

## Code Snippets (Reusable Patterns)

### Determine current video state from the SDK (mirrors `syncCurrentState`)
```swift
let isVideoOn = ZMUIToolKitManager.videoSDK?
    .getSession()?
    .getMySelf()?
    .getVideoCanvas()?
    .videoStatus()?
    .on ?? false
if isVideoOn {
    // live camera -> "unmute" state
} else {
    // video off -> "mute" state
}
```

## Type Definitions

| Type | Kind | Notes |
|------|------|-------|
| `VideoButtonType` | internal enum (`Int`) | `mute = 1`, `unmute` |
| `ZMUIToolkitIconBtn` | open class (superclass) | UIButton subclass providing icon/badge/layout config |
| `ZMUIToolkitIconBtnDescriptor` | struct | Initializer configuration (frame, colors, icon name, padding, callback) |
| `ZMUIToolKitManager` | class | Holds the `ZoomVideoSDK` instance and `videoSDKDelegate` fan-out |
| `CompositeZoomVideoSDKDelegate` | class | Weak fan-out delegate; register via `add(_:)` / `remove(_:)` |
| `ZoomVideoSDKError` | SDK enum | `.Errors_Success` indicates success |
| `ZoomVideoSDKVideoHelper` | SDK class | Provides `startVideo()`, `stopVideo()` |
| `ZoomVideoSDKVideoStatus` | SDK class | Returned by `videoStatus()`; `on` boolean indicates whether video is live |
| `ZoomVideoSDKUser` | SDK class | Local user via `getSession()?.getMySelf()` |

## Metadata
- **Source file**: `ZMUIToolkit/ZMUIToolkit/Sources/Buttons/ZMUIToolkitVideoBtn.swift`
- **Generated for**: AI-assisted code generation and integration
- **Verification**: Behaviors documented above were verified directly against the implementation source, not assumed.
