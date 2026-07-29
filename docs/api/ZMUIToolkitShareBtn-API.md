# ZMUIToolkitShareBtn API Documentation

## Module Information
- **Module**: ZMUIToolkitShareBtn
- **Platform**: iOS
- **Language**: Swift (with Objective-C interop via `@objc`)
- **Version**: 1.0
- **Framework**: ZMUIToolkit
- **Superclass**: `ZMUIToolkitIconBtn` (which subclasses `UIButton`)
- **Conforms to**: `ZoomVideoSDKDelegate`
- **Concurrency**: `@MainActor` — all members must be accessed on the main thread.

## Overview

`ZMUIToolkitShareBtn` is a drop-in, self-managing screen-share control button for sessions built on the Zoom Video SDK. It is built specifically for iOS **in-app / ReplayKit broadcast** screen sharing. Once added to a view, it:

1. Reads whether the local participant is currently sharing out from the SDK's share helper and renders the matching icon (start vs. stop).
2. On tap, performs the correct share action for the current state: when **not** sharing it presents Apple's `RPSystemBroadcastPickerView` (the ReplayKit system broadcast picker) so the user can begin a broadcast; when **already** sharing it calls `stopShare()`.
3. Registers itself as a `ZoomVideoSDKDelegate` observer and **automatically keeps its icon and enabled state in sync** with the authoritative SDK share state via `onUserShareStatusChanged`.
4. Continuously gates whether sharing is even allowed (host share lock, multi-share setting, another user already sharing, in-app screen-share support) and disables itself when it is not.

The button is icon-only and has two visual/behavioral states represented by the private `ShareButtonType` enum (`start`, `stop`).

The button reflects the authoritative share state reported by the SDK; its icon updates to the sharing state once a broadcast begins (via `onUserShareStatusChanged`).

## Lifecycle

### Prerequisites
- The Zoom Video SDK must be initialized **and** registered with the toolkit via `ZMUIToolKitManager.initialize(videoSDK:bundleId:)` **before** creating the button. The button reads `ZMUIToolKitManager.videoSDK` during `init()` to determine its initial state, and obtains the share helper from it on tap.
- The `bundleId` passed to `ZMUIToolKitManager.initialize(videoSDK:bundleId:)` should be the **App Extension bundle identifier of your ReplayKit Broadcast Upload Extension**. The button assigns it to `RPSystemBroadcastPickerView.preferredExtension` so the picker pre-selects your extension. If `ZMUIToolKitManager.bundleId` is empty, `preferredExtension` is left unset and the user must pick the extension manually.
- If `ZMUIToolKitManager.videoSDK` is `nil` (toolkit not initialized or already cleaned up), the button is constructed in the `start` state, is **disabled** (because `canCurrentUserStartScreenShare()` returns `false` when the share helper is nil), and tap actions take no effect (the callback guards on the share helper being non-nil).

### Entry / Exit
- **Construction** (`init()`): determines initial state via `isScreenSharingOut()`, builds the underlying `ZMUIToolkitIconBtn` descriptor (40×40, clear background, `secondaryLabel` icon color, zero icon padding), calls `super.init`, sets `isEnabled = canCurrentUserStartScreenShare()`, then **adds itself** to `ZMUIToolKitManager.videoSDKDelegate`.
- **Destruction** (`deinit`): **removes itself** from `ZMUIToolKitManager.videoSDKDelegate`. Delegates are held weakly by `CompositeZoomVideoSDKDelegate`.

> This button does not override `didMoveToWindow()`. Initial state is set at construction and thereafter updated by `onUserShareStatusChanged`.

### Delegate Registration Model
The button does **not** assign itself to `ZoomVideoSDK.delegate`. Instead it registers with the toolkit's fan-out delegate (`ZMUIToolKitManager.videoSDKDelegate`, a `CompositeZoomVideoSDKDelegate`). This allows multiple toolkit components to observe SDK events simultaneously. Child delegates are stored in a weak hash table (`NSHashTable.weakObjects()`).

## State Machine

### States (`ShareButtonType`, internal enum, raw `Int`)

| State | Raw value | Meaning | Icon (default asset) | Tap action |
|-------|-----------|---------|----------------------|-----------|
| `start` | 1 | Local user is **not** sharing out; tapping starts a share | `StartShare` | Present `RPSystemBroadcastPickerView` |
| `stop` | 2 | Local user **is** sharing out; tapping stops the share | `StopShare` | `shareHelper.stopShare()` |

> The enum case names describe the **action the tap performs**, not the current activity. In `start`, the user is idle and the next action starts sharing. In `stop`, the user is sharing and the next action stops it. `currentType` defaults to `.stop` at the property level but is overwritten in `init()` before display.

### Transitions

Two transition drivers exist: **optimistic** (the stop branch immediately switches to `.start` on a successful `stopShare()`), and **authoritative** (via the `onUserShareStatusChanged` SDK delegate callback, which is the source of truth).

| From | Trigger | Action attempted | To |
|------|---------|------------------|----|
| `start` | tap | guard `canCurrentUserStartScreenShare()`; if `!isSharingOut()`, present `RPSystemBroadcastPickerView` and programmatically trigger its button | unchanged (the actual transition to `.stop` happens via `onUserShareStatusChanged(.start/.resume)`) |
| `stop` | tap | `shareHelper.stopShare()` | `.start` immediately **if** `stopShare()` returns `.Errors_Success` (optimistic) |
| any | `onUserShareStatusChanged(.start / .resume)` (local user only) | `switchTo(.stop)` + `dismissBroadcastPicker()` | `stop` |
| any | `onUserShareStatusChanged(.none / .stop)` (local user only) | `switchTo(.start)` + `dismissBroadcastPicker()` | `start` |

After every `onUserShareStatusChanged` for the local user, `isEnabled` is recomputed via `canCurrentUserStartScreenShare()`.

> Note: tapping in the `start` state does not change the icon immediately. The share begins after the user completes the ReplayKit system broadcast flow and the SDK fires `onUserShareStatusChanged(.start)`, at which point the icon updates to the sharing state. There is no `.start`-branch return value to inspect because starting is driven by the system picker.

### Enablement rule (`canCurrentUserStartScreenShare`)
Computed from the share helper. The button is enabled only when ALL of the following hold (defaults shown are the fallback used when the share helper is `nil`):

```
isSupportInAppScreenShare == true   (default false -> disabled)
isShareLocked            == false   (default true  -> disabled)
NOT (isMultiShareEnabled == false AND isOtherSharing == true)
                                    (defaults: multiShare false, otherSharing true -> disabled)
```

Logic:
```
if !isSupportInAppScreenShare           -> return false
if isShareLocked                        -> return false
if !isMultiShareEnabled && isOtherSharing -> return false
otherwise                               -> return true
```

This gate is applied at construction (`isEnabled`) and re-applied after each local-user `onUserShareStatusChanged`. It is also re-checked at the top of the tap callback and short-circuits the action if `false`.

## APIs

### `init()`
```swift
@objc public init()
```
Designated public initializer. Reads `shareHelper.isScreenSharingOut()` to choose the initial state/icon, constructs the button (40×40, clear background, `secondaryLabel` icon color, zero icon padding), sets `isEnabled` from `canCurrentUserStartScreenShare()`, and registers as a delegate observer.

- **Preconditions**: Toolkit initialized via `ZMUIToolKitManager.initialize` (recommended). Must be called on the main thread (`@MainActor`).
- **Side effects**: Adds `self` to `ZMUIToolKitManager.videoSDKDelegate`; sets `isEnabled`.
- **Returns**: A configured button instance.

### `init?(coder:)`
```swift
@MainActor required public init?(coder: NSCoder)
```
- **Not supported.** Calls `fatalError`. Do **not** instantiate this button from a Storyboard/XIB.

### `setStartShareIcon(image:)`
```swift
public func setStartShareIcon(image: UIImage)
```
Overrides the icon used for the `start` state. If the button is currently in the `start` state, the new icon is applied immediately.
- **Parameters**: `image` — the replacement start-state icon.

### `setStopShareIcon(image:)`
```swift
public func setStopShareIcon(image: UIImage)
```
Overrides the icon used for the `stop` state. If the button is currently in the `stop` state, the new icon is applied immediately.
- **Parameters**: `image` — the replacement stop-state icon.

### `canCurrentUserStartScreenShare()` *(internal)*
```swift
func canCurrentUserStartScreenShare() -> Bool
```
Returns whether the local user is currently permitted to start an in-app screen share, based on in-app-share support, host share lock, multi-share setting, and whether another user is sharing. Used to drive `isEnabled` and to gate the tap action. Returns `false` defensively when the share helper is `nil`.

### `switchTo(type:)` *(internal)*
```swift
internal func switchTo(type: ShareButtonType)
```
Sets `currentType` and updates the displayed image to match (`startIcon` for `.start`, `stopIcon` for `.stop`). Internal — documented for completeness.

### `dismissBroadcastPicker()` *(internal, `@objc`)*
```swift
@objc internal func dismissBroadcastPicker()
```
Removes the `RPSystemBroadcastPickerView` (located by tag `kBroadcastPickerTag` = `10001`) from the window if present. Called whenever the local user's share status changes (start, resume, none, or stop) to clean up the transient picker view.

### Inherited configuration (from `ZMUIToolkitIconBtn`)
The following commonly-used setters are inherited and may be used to style the button: `setBackgroundColor(color:)`, `setRadius(radius:)`, `setSize(size:)`, `setPosition(x:y:)`, `setIconColor(color:)`, `setIcon(icon:)`, `setIconSize(_:)`, `setIconPadding(padding:)`, `setAccessibility(label:hint:)`. (Note: `setIcon`/`setImage` set the raw image directly and will be overwritten on the next state change — prefer the per-state `setStartShareIcon` / `setStopShareIcon` methods above.)

## Callbacks (`ZoomVideoSDKDelegate`)

The button implements one delegate method. It is invoked on the main thread via the toolkit's `CompositeZoomVideoSDKDelegate` fan-out.

### `onUserShareStatusChanged(_:user:shareAction:)`
```swift
public func onUserShareStatusChanged(_ helper: ZoomVideoSDKShareHelper?, user: ZoomVideoSDKUser?, shareAction: ZoomVideoSDKShareAction?)
```
Fired whenever any user's share status changes. The button **filters to the local user only**: it acts only when the changed `user` equals `getSession()?.getMySelf()`. It reads the status via `shareAction?.getShareStatus()` (defaulting to `.none` when `shareAction` is nil) and:

- On `.start` or `.resume`: `switchTo(.stop)` and `dismissBroadcastPicker()`.
- On `.none` or `.stop`: `switchTo(.start)` and `dismissBroadcastPicker()`.
- On any other status: no icon change (falls through `default`); however `isEnabled` is still recomputed below.

After the switch, `isEnabled` is recomputed from `canCurrentUserStartScreenShare()`.

- **State change**: resyncs the icon to match SDK share status for local-user changes only; always re-evaluates `isEnabled`.
- No icon or enablement change when the local user can't be resolved or the changed `user` is not the local user.
- **Threading**: main thread (via the toolkit fan-out delegate).

## Error Handling

Share starting is delegated to the operating system's ReplayKit broadcast picker.

| Action | Calls | On `.Errors_Success` | On other result |
|--------|-------|----------------------|----------------|
| Start share | (none — presents `RPSystemBroadcastPickerView`) | n/a — no SDK return value to inspect; transition awaits `onUserShareStatusChanged(.start)` | n/a |
| Stop share | `shareHelper.stopShare()` | `switchTo(.start)` | icon unchanged; reconciled by `onUserShareStatusChanged` |

**Notes**
- The start branch has no direct return-code path. The button reflects the authoritative share state reported by the SDK; its icon updates to the sharing state once a broadcast begins via `onUserShareStatusChanged(.start)`. If your app wants additional handling around share start, observe SDK events via `ZMUIToolKitManager.videoSDKDelegate.add(_:)`.
- `stopShare()` returns a `ZoomVideoSDKError`; `.Errors_Success` triggers the icon update to `.start`. Otherwise the icon is reconciled by the authoritative callback.

**Integration guidance**: Register your own `ZoomVideoSDKDelegate` if the host app wants to react to additional share-related SDK events in parallel with the button.

## Rules

- **REQUIRED**: Initialize the toolkit (`ZMUIToolKitManager.initialize(videoSDK:bundleId:)`) before constructing the button for correct state and functional taps.
- **REQUIRED**: Pass your ReplayKit Broadcast Upload Extension's bundle identifier as `bundleId` to `ZMUIToolKitManager.initialize` so the broadcast picker preselects your extension. Without it the user must choose the extension manually.
- **REQUIRED**: Provide a configured ReplayKit Broadcast Upload Extension in the host app and the appropriate entitlements; this button only presents the system picker, it does not implement the broadcast extension.
- **REQUIRED**: Construct and use the button on the main thread (`@MainActor`).
- **FORBIDDEN**: Instantiating via `init?(coder:)` / Interface Builder (traps with `fatalError`).
- **DO NOT** assign the button (or anything) directly to `ZoomVideoSDK.delegate`; the toolkit owns that slot. Use `ZMUIToolKitManager.videoSDKDelegate.add(_:)`.
- **DO NOT** add another view with tag `10001` (`kBroadcastPickerTag`) to the button's window; the button uses this tag to locate and dismiss the broadcast picker.
- **AVOID** calling inherited `setIcon`/`setImage` to represent share state — it is transient and overwritten on the next state change. Use `setStartShareIcon` / `setStopShareIcon`.

## Examples

### Example 1: Add the button to a toolbar (happy path)
```swift
// Assumes ZMUIToolKitManager.initialize(videoSDK:bundleId: "com.yourapp.BroadcastExtension")
// was already called with the Broadcast Upload Extension's bundle id.
let shareButton = ZMUIToolkitShareBtn()
shareButton.setSize(size: CGSize(width: 44, height: 44))
toolbarStackView.addArrangedSubview(shareButton)
// No further wiring needed: the button self-registers for share status updates,
// presents the ReplayKit broadcast picker on tap when idle, and stops sharing on tap when active.
```

### Example 2: Customize the per-state icons
```swift
let shareButton = ZMUIToolkitShareBtn()
shareButton.setStartShareIcon(image: UIImage(named: "custom_start_share")!)
shareButton.setStopShareIcon(image: UIImage(named: "custom_stop_share")!)
// If the current state matches one of these, its icon updates immediately.
```

### Example 3: Observe share-related SDK events alongside the button
```swift
final class ShareOutcomeObserver: NSObject, ZoomVideoSDKDelegate {
    func onFailed(toStartShare helper: ZoomVideoSDKShareHelper, user: ZoomVideoSDKUser?) {
        // Handle share-start callbacks in the host app.
    }
    func on(inAppScreenShare code: RPRecordingErrorCode) {
        // Handle ReplayKit broadcast callbacks.
    }
}
let observer = ShareOutcomeObserver()
ZMUIToolKitManager.videoSDKDelegate.add(observer) // held weakly — retain `observer` yourself
```

### Example 4: Lifecycle / teardown
```swift
// The button removes itself from the delegate fan-out in deinit.
// Just release your reference (e.g. remove from view hierarchy) when done.
shareButton.removeFromSuperview()
// shareButton = nil  // deinit -> ZMUIToolKitManager.videoSDKDelegate.remove(self)
```

## Code Snippets (Reusable Patterns)

### Determine current share state from the SDK (mirrors `init()`)
```swift
let shareHelper = ZMUIToolKitManager.videoSDK?.getShareHelper()
let isSharingOut = shareHelper?.isScreenSharingOut() ?? false
if isSharingOut {
    // currently sharing -> "stop" state (tap stops the share)
} else {
    // idle -> "start" state (tap presents the broadcast picker)
}
```

### Reproduce the start-eligibility gate (mirrors `canCurrentUserStartScreenShare`)
```swift
let helper = ZMUIToolKitManager.videoSDK?.getShareHelper()
let canStart =
    (helper?.isSupportInAppScreenShare() ?? false) &&
    !(helper?.isShareLocked() ?? true) &&
    !((helper?.isMultiShareEnabled() ?? false) == false && (helper?.isOtherSharing() ?? true))
// canStart drives the button's isEnabled
```

## Type Definitions

| Type | Kind | Notes |
|------|------|-------|
| `ShareButtonType` | internal enum (`Int`) | `start = 1`, `stop` (= 2). Case names describe the tap action. |
| `kBroadcastPickerTag` | file-level `let Int` | `10001`. Tag used to locate/dismiss the `RPSystemBroadcastPickerView`. |
| `ZMUIToolkitIconBtn` | open class (superclass) | `UIButton` subclass providing icon/badge/layout config and click-callback dispatch |
| `ZMUIToolkitIconBtnDescriptor` | struct | Initializer configuration (frame, backgroundColor, accessibilityLabel, iconname, iconPadding, iconColor, callback) |
| `ZMUIToolKitManager` | class | Holds the `ZoomVideoSDK` instance, `bundleId`, and `videoSDKDelegate` fan-out |
| `CompositeZoomVideoSDKDelegate` | class | Weak fan-out delegate; register via `add(_:)` / `remove(_:)` |
| `RPSystemBroadcastPickerView` | ReplayKit class | Apple's system broadcast picker; `preferredExtension` set from `ZMUIToolKitManager.bundleId` |
| `ZoomVideoSDKShareHelper` | SDK class | Provides `isScreenSharingOut()`, `isSharingOut()`, `stopShare()`, `isSupportInAppScreenShare()`, `isShareLocked()`, `isMultiShareEnabled()`, `isOtherSharing()` |
| `ZoomVideoSDKShareAction` | SDK class | `getShareStatus()` returns the `ZoomVideoSDKReceiveSharingStatus` for the change |
| `ZoomVideoSDKReceiveSharingStatus` | SDK enum | Relevant cases: `.start`, `.resume`, `.stop`, `.none` |
| `ZoomVideoSDKError` | SDK enum | `.Errors_Success` indicates success (from `stopShare()`) |
| `ZoomVideoSDKUser` | SDK class | Local user via `getSession()?.getMySelf()` |

## Metadata
- **Source file**: `ZMUIToolkit/ZMUIToolkit/Sources/Buttons/ZMUIToolkitShareBtn.swift`
- **Generated for**: AI-assisted code generation and integration
- **Verification**: Behaviors documented above were verified directly against the implementation source, not assumed.
