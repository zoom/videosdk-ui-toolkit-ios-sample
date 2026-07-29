# ZMUIToolkitAudioBtn API Documentation

## Module Information
- **Module**: ZMUIToolkitAudioBtn
- **Platform**: iOS
- **Language**: Swift (with Objective-C interop via `@objc`)
- **Version**: 1.0
- **Framework**: ZMUIToolkit
- **Superclass**: `ZMUIToolkitIconBtn` (which subclasses `UIButton`)
- **Conforms to**: `ZoomVideoSDKDelegate`
- **Concurrency**: `@MainActor` — all members must be accessed on the main thread.

## Overview

`ZMUIToolkitAudioBtn` is a drop-in, self-managing audio control button for sessions built on the Zoom Video SDK. Once added to a view, it:

1. Reads the local participant's current audio status from the SDK and renders the matching icon.
2. On tap, performs the correct audio action for the current state (join audio, mute, or unmute).
3. Registers itself as a `ZoomVideoSDKDelegate` observer and **automatically keeps its icon in sync** with the authoritative SDK audio state (e.g. when the host mutes the user, or audio status changes for any reason).

The button is icon-only and has three visual/behavioral states represented by the private `AudioButtonType` enum.

## Lifecycle

### Prerequisites
- The Zoom Video SDK must be initialized **and** registered with the toolkit via `ZMUIToolKitManager.initialize(videoSDK:bundleId:)` **before** creating the button. The button reads `ZMUIToolKitManager.videoSDK` during `init()` to determine its initial state and obtains the audio helper from it on tap.
- If `ZMUIToolKitManager.videoSDK` is `nil` (toolkit not initialized or already cleaned up), the button is constructed in the `join` state and tap actions have no effect (the callback guards on the audio helper / self being non-nil).

### Entry / Exit
- **Construction** (`init()`): determines initial state, builds the underlying `ZMUIToolkitIconBtn` descriptor, calls `super.init`, **adds itself** to `ZMUIToolKitManager.videoSDKDelegate`, then calls `syncCurrentState()`.
- **Attachment** (`didMoveToWindow()`): re-syncs the icon to the current SDK audio state, ensuring correctness even if state changed between construction and display.
- **Destruction** (`deinit`): **removes itself** from `ZMUIToolKitManager.videoSDKDelegate`. Delegates are held weakly by `CompositeZoomVideoSDKDelegate`, so this removal is automatic cleanup.

### Delegate Registration Model
The button does **not** assign itself to `ZoomVideoSDK.delegate`. Instead it registers with the toolkit's fan-out delegate (`ZMUIToolKitManager.videoSDKDelegate`, a `CompositeZoomVideoSDKDelegate`). This allows multiple toolkit components to observe SDK events simultaneously. Child delegates are stored in a weak hash table.

## State Machine

### States (`AudioButtonType`, internal enum, raw `Int`)

| State | Raw value | Meaning | Icon (default asset) |
|-------|-----------|---------|----------------------|
| `join` | 1 | Local user has not joined session audio (`audioType == .none`) | `JoinAudio` |
| `mute` | 2 | Joined audio and **currently muted** | `MicMuted` |
| `unmute` | 3 | Joined audio and **currently unmuted** (live mic) | `Mic` |

> Note: `.unmute` is the "unmuted/live" state (mic on); tapping in this state will **mute**. `.mute` is the "muted" state; tapping will **unmute**. The enum case names describe the *current* state, not the action the tap performs.

### Transitions

Two transition drivers exist: **optimistic** (immediately after a successful API call in the tap handler) and **authoritative** (via SDK delegate callbacks / lifecycle, which is the source of truth).

| From | Trigger | Action attempted | To (on success) |
|------|---------|------------------|-----------------|
| `join` | tap | `audioHelper.startAudio()` | `unmute` |
| `mute` | tap | `unmuteAudio(myself)` → fallback `unmuteAudio(nil)` → fallback `startAudio()` | `unmute` |
| `unmute` | tap | `audioHelper.muteAudio(myself)` | `mute` |
| any | `onSessionJoin` callback | resync from SDK | matches SDK status |
| any | `onUserAudioStatusChanged` (local user only) | resync from SDK | matches SDK status |
| any | `didMoveToWindow` | resync from SDK | matches SDK status |

On any tap action, if the SDK call returns a non-`.Errors_Success` error, **the icon is not changed optimistically**. The authoritative `onUserAudioStatusChanged` callback (when/if it fires) reconciles the displayed state.

### Resync rule (`syncCurrentState`)
```
audioType == .none        -> switchTo(.join)
audioStatus.isMuted == true -> switchTo(.mute)
otherwise                 -> switchTo(.unmute)
```

## APIs

### `init()`
```swift
@objc public init()
```
Designated public initializer. Reads the local participant's audio status to choose the initial state/icon, constructs the button (40×40, clear background, `secondaryLabel` icon color, zero icon padding), registers as a delegate observer, and syncs state.

- **Preconditions**: Toolkit initialized via `ZMUIToolKitManager.initialize` (recommended). Must be called on the main thread (`@MainActor`).
- **Side effects**: Adds `self` to `ZMUIToolKitManager.videoSDKDelegate`.
- **Returns**: A configured button instance.

### `init?(coder:)`
```swift
@MainActor required public init?(coder: NSCoder)
```
- **Not supported.** Calls `fatalError`. Do **not** instantiate this button from a Storyboard/XIB.

### `setMuteAudioIcon(image:)`
```swift
public func setMuteAudioIcon(image: UIImage)
```
Overrides the icon used for the `mute` (muted) state. If the button is currently in the `mute` state, the new icon is applied immediately.
- **Parameters**: `image` — the replacement muted-state icon.

### `setUnmuteAudioIcon(image:)`
```swift
public func setUnmuteAudioIcon(image: UIImage)
```
Overrides the icon used for the `unmute` (live mic) state. Applied immediately if currently in the `unmute` state.

### `setJoinAudioIcon(image:)`
```swift
public func setJoinAudioIcon(image: UIImage)
```
Overrides the icon used for the `join` state. Applied immediately if currently in the `join` state.

### `switchTo(type:)` *(internal)*
```swift
internal func switchTo(type: AudioButtonType)
```
Sets `currentType` and updates the displayed image to match. Internal — not part of the public API surface; documented for completeness.

### `syncCurrentState()` *(private)*
Recomputes state from the SDK's current audio status and calls `switchTo(type:)`. Invoked from `init()`, `didMoveToWindow()`, `onSessionJoin()`, and `onUserAudioStatusChanged(...)`.

### Inherited configuration (from `ZMUIToolkitIconBtn`)
The following commonly-used setters are inherited and may be used to style the button: `setBackgroundColor(color:)`, `setRadius(radius:)`, `setSize(size:)`, `setPosition(x:y:)`, `setIconColor(color:)`, `setIcon(icon:)`, `setIconSize(_:)`, `setIconPadding(padding:)`, `setAccessibility(label:hint:)`. (Note: `setIcon`/`setImage` set the raw image directly and will be overwritten on the next state resync — prefer the per-state `set*AudioIcon` methods above.)

## Callbacks (`ZoomVideoSDKDelegate`)

The button implements two delegate methods. Both are invoked on the main thread via the toolkit's `CompositeZoomVideoSDKDelegate` fan-out.

### `onSessionJoin()`
```swift
public func onSessionJoin()
```
Fired when the local user joins the session. Calls `syncCurrentState()` to render the correct initial audio icon.
- **State change**: resyncs to match SDK audio status.

### `onUserAudioStatusChanged(_:user:)`
```swift
public func onUserAudioStatusChanged(_ helper: ZoomVideoSDKAudioHelper?, user users: [ZoomVideoSDKUser]?)
```
Fired whenever any user's audio status changes. The button **filters to the local user only**: it resyncs only if the changed user list contains the local user's ID. This is the primary mechanism keeping the icon authoritative (e.g. host-initiated mute, external mute/unmute).
- **State change**: resyncs to match SDK audio status, but only for local-user changes.
- Takes no action when `users` is nil, the local user ID can't be resolved, or the local user is not in the changed list.

## Error Handling

The tap callback inspects the `ZoomVideoSDKError` returned by each audio-helper call. Behavior:

| Action | Calls | On `.Errors_Success` | On other error |
|--------|-------|----------------------|----------------|
| Join audio | `startAudio()` | optimistic `switchTo(.unmute)` | no UI change; rely on delegate resync |
| Unmute | `unmuteAudio(myself)`, then `unmuteAudio(nil)`, then `startAudio()` (each only if prior failed) | optimistic `switchTo(.unmute)` | if all three fail, no UI change |
| Mute | `muteAudio(myself)` | optimistic `switchTo(.mute)` | no UI change |

**Rationale for the unmute fallback chain**: self-unmute permissions vary by SDK state and participant role. The button first attempts an explicit self-targeted unmute, then a `nil` (local) unmute, then a full `startAudio()` as a last resort. The first call returning `.Errors_Success` wins.

**Recovery strategy**: The icon updates on success and reconciles via the delegate: the authoritative state is reconciled by `onUserAudioStatusChanged`. To respond to action outcomes, observe SDK events via `ZMUIToolKitManager.videoSDKDelegate.add(_:)`.

## Rules

- **REQUIRED**: Initialize the toolkit (`ZMUIToolKitManager.initialize`) before constructing the button for correct state and functional taps.
- **REQUIRED**: Construct and use the button on the main thread (`@MainActor`).
- **FORBIDDEN**: Instantiating via `init?(coder:)` / Interface Builder (traps with `fatalError`).
- **DO NOT** assign the button (or anything) directly to `ZoomVideoSDK.delegate`; the toolkit owns that slot. Use `ZMUIToolKitManager.videoSDKDelegate.add(_:)`.
- **AVOID** calling inherited `setIcon`/`setImage` to represent audio state — it is transient and overwritten on the next resync. Use `setMuteAudioIcon` / `setUnmuteAudioIcon` / `setJoinAudioIcon`.

## Examples

### Example 1: Add the button to a toolbar (happy path)
```swift
// Assumes ZMUIToolKitManager.initialize(videoSDK:bundleId:) was already called.
let audioButton = ZMUIToolkitAudioBtn()
audioButton.setSize(size: CGSize(width: 44, height: 44))
toolbarStackView.addArrangedSubview(audioButton)
// No further wiring needed: the button self-registers for audio status updates
// and toggles join/mute/unmute on tap.
```

### Example 2: Customize the per-state icons
```swift
let audioButton = ZMUIToolkitAudioBtn()
audioButton.setJoinAudioIcon(image: UIImage(named: "custom_join")!)
audioButton.setMuteAudioIcon(image: UIImage(named: "custom_muted")!)
audioButton.setUnmuteAudioIcon(image: UIImage(named: "custom_live")!)
// If the current state matches one of these, its icon updates immediately.
```

### Example 3: Respond to action outcomes via the delegate
```swift
final class AudioErrorObserver: NSObject, ZoomVideoSDKDelegate {
    func onError(_ errorType: ZoomVideoSDKError, detail details: Int) {
        // Show retry UI, logging, etc.
    }
}
let observer = AudioErrorObserver()
ZMUIToolKitManager.videoSDKDelegate.add(observer) // held weakly — retain `observer` yourself
```

### Example 4: Lifecycle / teardown
```swift
// The button removes itself from the delegate fan-out in deinit.
// Just release your reference (e.g. remove from view hierarchy) when done.
audioButton.removeFromSuperview()
// audioButton = nil  // deinit -> ZMUIToolKitManager.videoSDKDelegate.remove(self)
```

## Code Snippets (Reusable Patterns)

### Determine current audio state from the SDK (mirrors `syncCurrentState`)
```swift
let status = ZMUIToolKitManager.videoSDK?.getSession()?.getMySelf()?.audioStatus()
if status?.audioType == .none {
    // not joined to audio -> "join" state
} else if status?.isMuted ?? false {
    // muted -> "mute" state
} else {
    // live mic -> "unmute" state
}
```

## Type Definitions

| Type | Kind | Notes |
|------|------|-------|
| `AudioButtonType` | internal enum (`Int`) | `join = 1`, `mute`, `unmute` |
| `ZMUIToolkitIconBtn` | open class (superclass) | UIButton subclass providing icon/badge/layout config |
| `ZMUIToolkitIconBtnDescriptor` | struct | Initializer configuration (frame, colors, icon name, padding, callback) |
| `ZMUIToolKitManager` | class | Holds the `ZoomVideoSDK` instance and `videoSDKDelegate` fan-out |
| `CompositeZoomVideoSDKDelegate` | class | Weak fan-out delegate; register via `add(_:)` / `remove(_:)` |
| `ZoomVideoSDKAudioType` | SDK enum | `.none` indicates audio not joined |
| `ZoomVideoSDKError` | SDK enum | `.Errors_Success` indicates success |
| `ZoomVideoSDKAudioHelper` | SDK class | Provides `startAudio()`, `muteAudio(_:)`, `unmuteAudio(_:)` |
| `ZoomVideoSDKUser` | SDK class | Local user via `getSession()?.getMySelf()` |

## Metadata
- **Source file**: `ZMUIToolkit/ZMUIToolkit/Sources/Buttons/ZMUIToolkitAudioBtn.swift`
- **Generated for**: AI-assisted code generation and integration
- **Verification**: Behaviors documented above were verified directly against the implementation source, not assumed.
