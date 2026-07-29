# ZMUIToolkitPauseShareBtn API Documentation

## Module Information
- **Module**: ZMUIToolkitPauseShareBtn
- **Platform**: iOS
- **Language**: Swift (with Objective-C interop via `@objc`)
- **Version**: 1.0
- **Framework**: ZMUIToolkit
- **Superclass**: `ZMUIToolkitIconBtn` (which subclasses `UIButton`)
- **Conforms to**: `ZoomVideoSDKDelegate`
- **Concurrency**: `@MainActor` — all members must be accessed on the main thread.

## Overview

`ZMUIToolkitPauseShareBtn` is a drop-in, self-managing button that pauses and resumes the **local user's active screen share** in sessions built on the Zoom Video SDK. Once added to a view, it:

1. Reads the local user's current share state from `ZMShareManager.shared` and renders the matching icon (pause icon while sharing/live, play/resume icon while paused).
2. On tap, performs the correct action for the current state — `pauseShare()` when live, `resumeShare()` when paused.
3. Enables/disables itself based on whether the local user is actively sharing. It is **greyed out (disabled) unless a share is active**.
4. Registers itself as a `ZoomVideoSDKDelegate` observer and **automatically keeps its icon and enabled state in sync** with the authoritative SDK share state when the local user's share status changes.

The button is icon-only and has two visual/behavioral states represented by the private `PauseShareButtonType` enum. All pause/resume control is delegated to the `ZMShareManager` singleton, which is the single source of truth for "am I sharing" and "is my share paused".

## Lifecycle

### Prerequisites
- The Zoom Video SDK must be initialized **and** registered with the toolkit via `ZMUIToolKitManager.initialize(videoSDK:bundleId:)` **before** creating the button. `ZMShareManager` reads `ZMUIToolKitManager.videoSDK` to obtain the share helper and the local user's share-action list.
- A share is only meaningful while the local user is actively sharing out (`shareHelper.isScreenSharingOut() == true`). The button is **disabled** when no share is active, so taps are not delivered.
- If `ZMUIToolKitManager.videoSDK` is `nil` (toolkit not initialized or already cleaned up), `ZMShareManager.shared.isSharing` returns `false`, so the button is constructed in the `playing` state and **disabled**. `pauseShare()` / `resumeShare()` return `.Errors_Wrong_Usage` when the share helper is `nil`.

### Entry / Exit
- **Construction** (`init()`): reads `ZMShareManager.shared.isPaused` to pick the initial state/icon, builds the underlying `ZMUIToolkitIconBtn` descriptor, calls `super.init`, sets `isEnabled = ZMShareManager.shared.isSharing`, then **adds itself** to `ZMUIToolKitManager.videoSDKDelegate`. The initial icon and enabled state are set inline at construction.
- **Attachment** (`didMoveToWindow()`): calls `syncCurrentState()` to reconcile both the icon and the enabled state with the current SDK share state when the button is attached to the view hierarchy.
- **Destruction** (`deinit`): **removes itself** from `ZMUIToolKitManager.videoSDKDelegate`. Delegates are held weakly by `CompositeZoomVideoSDKDelegate`.

### Delegate Registration Model
The button does **not** assign itself to `ZoomVideoSDK.delegate`. Instead it registers with the toolkit's fan-out delegate (`ZMUIToolKitManager.videoSDKDelegate`, a `CompositeZoomVideoSDKDelegate`). This allows multiple toolkit components to observe SDK events simultaneously. Child delegates are stored in a weak hash table.

## State Machine

### States (`PauseShareButtonType`, internal enum, raw `Int`)

| State | Raw value | Meaning | Icon (default asset) | Tap action |
|-------|-----------|---------|----------------------|------------|
| `playing` | 1 | Share is active and **live** (not paused) | `PauseShare` | Pauses the share |
| `paused` | 2 | Share is active but **paused** | `ResumeShare` | Resumes the share |

> Note: The enum case names describe the *current* state, not the action the tap performs. In `.playing` (live) a tap **pauses**; in `.paused` a tap **resumes**.

### Enabled / Disabled
The button is a separate axis from the two states above. `isEnabled` reflects `ZMShareManager.shared.isSharing`:
- **Enabled** only while a share is actively being sent out.
- **Disabled (greyed out)** whenever no share is active. Disabled state is rendered by the superclass with a `systemGray4` background, and taps are not delivered while disabled.

### Transitions

Two transition drivers exist: **optimistic** (immediately after a successful API call in the tap handler) and **authoritative** (via SDK delegate callbacks / lifecycle, which is the source of truth).

| From | Trigger | Action attempted | To (on success) |
|------|---------|------------------|-----------------|
| `playing` | tap | `ZMShareManager.shared.pauseShare()` | `paused` |
| `paused` | tap | `ZMShareManager.shared.resumeShare()` | `playing` |
| any | `onUserShareStatusChanged` (local user only) | `syncCurrentState()` | matches SDK status; refreshes enabled state |
| any | `didMoveToWindow` | `syncCurrentState()` | matches SDK status; refreshes enabled state |

On any tap action, if `pauseShare()` / `resumeShare()` returns a non-`.Errors_Success` error, **the icon is not changed optimistically**. The authoritative `onUserShareStatusChanged` callback (when/if it fires for the local user) reconciles the displayed state and enabled flag.

### Resync rule (`syncCurrentState`)
```
isEnabled = ZMShareManager.shared.isSharing
ZMShareManager.shared.isPaused == true -> switchTo(.paused)
otherwise                              -> switchTo(.playing)
```

Where `ZMShareManager.shared.isPaused` is `true` only when the local user is sharing **and** the local user's share-action list contains an action whose `getShareStatus()` is `.pause` (`ZoomVideoSDKReceiveSharingStatus_Pause`).

## APIs

### `init()`
```swift
@objc public init()
```
Designated public initializer. Reads `ZMShareManager.shared.isPaused` to choose the initial state/icon, constructs the button (40×40, clear background, `secondaryLabel` icon color, zero icon padding), sets `isEnabled` from `ZMShareManager.shared.isSharing`, and registers as a delegate observer.

- **Preconditions**: Toolkit initialized via `ZMUIToolKitManager.initialize` (recommended). Must be called on the main thread (`@MainActor`).
- **Side effects**: Adds `self` to `ZMUIToolKitManager.videoSDKDelegate`; sets initial enabled state from the share manager.
- **Returns**: A configured button instance.

### `init?(coder:)`
```swift
@MainActor required public init?(coder: NSCoder)
```
- **Not supported.** Calls `fatalError`. Do **not** instantiate this button from a Storyboard/XIB.

### `setPauseShareIcon(image:)`
```swift
public func setPauseShareIcon(image: UIImage)
```
Overrides the icon used for the `playing` state (the icon shown while the share is live — i.e. the "pause" affordance). If the button is currently in the `playing` state, the new icon is applied immediately.
- **Parameters**: `image` — the replacement live-state (pause-affordance) icon.

### `setResumeShareIcon(image:)`
```swift
public func setResumeShareIcon(image: UIImage)
```
Overrides the icon used for the `paused` state (the icon shown while the share is paused — i.e. the "resume/play" affordance). If the button is currently in the `paused` state, the new icon is applied immediately.
- **Parameters**: `image` — the replacement paused-state (resume-affordance) icon.

### `switchTo(type:)` *(internal)*
```swift
internal func switchTo(type: PauseShareButtonType)
```
Sets `currentType` and updates the displayed image to match (`pauseIcon` for `.playing`, `playIcon` for `.paused`). Internal — not part of the public API surface; documented for completeness.

### `syncCurrentState()` *(private)*
Refreshes `isEnabled` from `ZMShareManager.shared.isSharing` and recomputes the icon state from `ZMShareManager.shared.isPaused`, then calls `switchTo(type:)`. Invoked from `didMoveToWindow()` and `onUserShareStatusChanged(...)`.

### `didMoveToWindow()`
```swift
public override func didMoveToWindow()
```
UIView override. Calls `super.didMoveToWindow()` then `syncCurrentState()` to refresh the icon and enabled state to the current SDK share state when the button is attached to the view hierarchy.

### Inherited configuration (from `ZMUIToolkitIconBtn`)
The following commonly-used setters are inherited and may be used to style the button: `setBackgroundColor(color:)`, `setRadius(radius:)`, `setSize(size:)`, `setPosition(x:y:)`, `setIconColor(color:)`, `setIcon(icon:)`, `setIconSize(_:)`, `setIconPadding(padding:)`, `setAccessibility(label:hint:)`. (Note: `setIcon`/`setImage` set the raw image directly and are replaced on the next state resync — prefer the per-state `setPauseShareIcon` / `setResumeShareIcon` methods above.)

## Callbacks (`ZoomVideoSDKDelegate`)

The button implements one delegate method, invoked on the main thread via the toolkit's `CompositeZoomVideoSDKDelegate` fan-out.

### `onUserShareStatusChanged(_:user:shareAction:)`
```swift
public func onUserShareStatusChanged(_ helper: ZoomVideoSDKShareHelper?, user: ZoomVideoSDKUser?, shareAction: ZoomVideoSDKShareAction?)
```
Fired whenever any user's share status changes (start, pause, resume, stop). The button **filters to the local user only**: it resyncs only if the changed `user` equals the local user (`ZMUIToolKitManager.videoSDK?.getSession()?.getMySelf()`). This is the primary mechanism keeping the icon and enabled state authoritative (e.g. share started/stopped, or pause/resume initiated elsewhere).
- **State change**: calls `syncCurrentState()`, which refreshes `isEnabled` from `isSharing` and resyncs the icon to `isPaused`.
- Takes no action when the local user cannot be resolved, `user` is nil, or `user` is not the local user.
- **Note**: The button derives all state by re-querying `ZMShareManager.shared`.

## Error Handling

The tap callback inspects the `ZoomVideoSDKError` returned by `ZMShareManager.shared.pauseShare()` / `resumeShare()`. Behavior:

| State at tap | Calls | On `.Errors_Success` | On other error |
|--------------|-------|----------------------|----------------|
| `paused` | `ZMShareManager.shared.resumeShare()` | optimistic `switchTo(.playing)` | no UI change; rely on delegate resync |
| `playing` | `ZMShareManager.shared.pauseShare()` | optimistic `switchTo(.playing)`… see note | no UI change; rely on delegate resync |

> Implementation note: the tap handler branches on `ZMShareManager.shared.isPaused` (re-queried at tap time). When paused, a successful `resumeShare()` optimistically switches to `.playing`; when live, a successful `pauseShare()` optimistically switches to `.paused`. The tap handler also guards on `ZMShareManager.shared.isSharing` and takes no action if no share is active; the button is disabled in that case.

**`pauseShare()` / `resumeShare()` failure modes** (from `ZMShareManager`):
- Returns `.Errors_Wrong_Usage` immediately if the share helper is `nil` (SDK not available / not registered).
- Otherwise returns the `ZoomVideoSDKError` from the SDK's `shareHelper.pauseShare()` / `resumeShare()` call directly.

**Recovery strategy**: No explicit retry or error surfacing is performed by the button. The authoritative state is reconciled by `onUserShareStatusChanged`. Hosts/integrators wanting to show error UI should observe SDK errors via their own delegate registered with `ZMUIToolKitManager.videoSDKDelegate.add(_:)`.

## Rules

- **REQUIRED**: Initialize the toolkit (`ZMUIToolKitManager.initialize`) before constructing the button for correct state and functional taps.
- **REQUIRED**: Construct and use the button on the main thread (`@MainActor`).
- **FORBIDDEN**: Instantiating via `init?(coder:)` / Interface Builder (traps with `fatalError`).
- **DO NOT** assign the button (or anything) directly to `ZoomVideoSDK.delegate`; the toolkit owns that slot. Use `ZMUIToolKitManager.videoSDKDelegate.add(_:)`.
- **AVOID** calling inherited `setIcon`/`setImage` to represent pause/resume state — it is replaced on the next resync. Use `setPauseShareIcon` / `setResumeShareIcon`.
- **NOTE**: Pause/resume only applies to the local user's own share. The button does not control or reflect remote participants' shares.

## Examples

### Example 1: Add the button to a share toolbar (happy path)
```swift
// Assumes ZMUIToolKitManager.initialize(videoSDK:bundleId:) was already called.
let pauseShareButton = ZMUIToolkitPauseShareBtn()
pauseShareButton.setSize(size: CGSize(width: 44, height: 44))
toolbarStackView.addArrangedSubview(pauseShareButton)
// No further wiring needed: the button self-registers for share-status updates,
// auto-enables only while a share is active, and toggles pause/resume on tap.
```

### Example 2: Customize the per-state icons
```swift
let pauseShareButton = ZMUIToolkitPauseShareBtn()
pauseShareButton.setPauseShareIcon(image: UIImage(named: "custom_pause")!)   // shown while live
pauseShareButton.setResumeShareIcon(image: UIImage(named: "custom_resume")!) // shown while paused
// If the current state matches one of these, its icon updates immediately.
```

### Example 3: Observe errors yourself (the button does not surface them)
```swift
final class ShareErrorObserver: NSObject, ZoomVideoSDKDelegate {
    func onError(_ errorType: ZoomVideoSDKError, detail details: Int) {
        // Show retry UI, logging, etc.
    }
}
let observer = ShareErrorObserver()
ZMUIToolKitManager.videoSDKDelegate.add(observer) // held weakly — retain `observer` yourself
```

### Example 4: Lifecycle / teardown
```swift
// The button removes itself from the delegate fan-out in deinit.
// Just release your reference (e.g. remove from view hierarchy) when done.
pauseShareButton.removeFromSuperview()
// pauseShareButton = nil  // deinit -> ZMUIToolKitManager.videoSDKDelegate.remove(self)
```

## Code Snippets (Reusable Patterns)

### Determine current share / pause state from the toolkit (mirrors `syncCurrentState`)
```swift
let isSharing = ZMShareManager.shared.isSharing
let isPaused  = ZMShareManager.shared.isPaused
if !isSharing {
    // No active share -> button should be disabled
} else if isPaused {
    // Share paused -> "paused" state (show resume icon)
} else {
    // Share live -> "playing" state (show pause icon)
}
```

### Pause / resume the local share directly (what the button does on tap)
```swift
if ZMShareManager.shared.isPaused {
    let err = ZMShareManager.shared.resumeShare()
    // err == .Errors_Success means the request was accepted
} else {
    let err = ZMShareManager.shared.pauseShare()
}
```

## Type Definitions

| Type | Kind | Notes |
|------|------|-------|
| `PauseShareButtonType` | internal enum (`Int`) | `playing = 1` (live, tap pauses), `paused = 2` (paused, tap resumes) |
| `ZMUIToolkitIconBtn` | open class (superclass) | UIButton subclass providing icon/badge/layout config and disabled-state rendering |
| `ZMUIToolkitIconBtnDescriptor` | struct | Initializer configuration (frame, colors, icon name, padding, callback) |
| `ZMUIToolKitManager` | class | Holds the `ZoomVideoSDK` instance and `videoSDKDelegate` fan-out |
| `CompositeZoomVideoSDKDelegate` | class | Weak fan-out delegate; register via `add(_:)` / `remove(_:)` |
| `ZMShareManager` | internal `@MainActor` class (singleton) | Single source of truth for `isSharing`, `isPaused`, and `pauseShare()` / `resumeShare()` |
| `ZoomVideoSDKShareHelper` | SDK class | Provides `pauseShare()`, `resumeShare()`, `isScreenSharingOut()` |
| `ZoomVideoSDKShareAction` | SDK class | Per-share action; `getShareStatus()` returns `ZoomVideoSDKReceiveSharingStatus` |
| `ZoomVideoSDKReceiveSharingStatus` | SDK enum | `.none`, `.start`, `.pause`, `.resume`, `.stop`; `.pause` indicates a paused share |
| `ZoomVideoSDKError` | SDK enum | `.Errors_Success` indicates success; `.Errors_Wrong_Usage` returned when share helper unavailable |
| `ZoomVideoSDKUser` | SDK class | Local user via `getSession()?.getMySelf()` |

## Metadata
- **Source file**: `ZMUIToolkit/ZMUIToolkit/Sources/Buttons/ZMUIToolkitPauseShareBtn.swift`
- **Generated for**: AI-assisted code generation and integration
- **Verification**: Behaviors documented above were verified directly against the implementation source (`ZMUIToolkitPauseShareBtn.swift`, `ZMUIToolkitIconBtn.swift`, `ZMUIToolkitDescriptor.swift`, `ZMUIToolKitManager.swift`, `CompositeZoomVideoSDKDelegate.swift`, `ZMShareManager.swift`) and the Zoom Video SDK headers, not assumed.
