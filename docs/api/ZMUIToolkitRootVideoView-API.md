# ZMUIToolkitRootVideoView API Documentation

## Module Information
- **Module**: ZMUIToolkitRootVideoView
- **Platform**: iOS
- **Language**: Swift (with Objective-C interop via `@objc`)
- **Version**: 1.0
- **Framework**: ZMUIToolkit
- **Superclass**: `UIView`
- **Conforms to**: `ZoomVideoSDKDelegate`, `UIScrollViewDelegate`
- **Objective-C name**: `ZMUIToolkitRootVideoView`
- **Concurrency**: `@MainActor` — all members must be accessed on the main thread.

## Overview

`ZMUIToolkitRootVideoView` is the toolkit's top-level, self-managing video surface for sessions built on the Zoom Video SDK. Drop it into a view hierarchy and it renders the complete in-session video experience with **no manual wiring**:

1. A **horizontally paged scroll view** (`UIScrollView`, paging enabled) whose pages are, in order: an optional **local share panel** (shown while *you* are screen sharing), zero or more **remote share receive views** (one per active remote share), then one or more **video grid pages** of participant cameras.
2. A **page control** (dots) reflecting the current page; hidden for a single page and non-interactive.
3. A **self-video overlay** (picture-in-picture-style floating cell of the local camera) shown on top of grid pages whenever there are remote users. In solo mode (no remote users) the local user is shown full-size in the primary grid instead.
4. A **cloud-recording indicator** badge ("● REC" / paused) in the top-left safe area while a recording is active or paused.
5. **System Picture-in-Picture (PiP)** integration via `ZMUIToolkitPiPController.shared`: when the app backgrounds, an active source (remote share > visible remote camera > local camera) drives the floating system PiP window.

The view registers itself with the toolkit's fan-out delegate (`ZMUIToolKitManager.videoSDKDelegate`) and reacts to SDK callbacks to keep the grid, shares, recording badge, and PiP source authoritative. It also observes device-orientation, app active/resign, and PiP-enabled notifications.

> Important: This view does **not** assign itself to `ZoomVideoSDK.delegate`. The toolkit owns that slot via `ZMUIToolKitManager.initialize`. Observers register through `ZMUIToolKitManager.videoSDKDelegate.add(_:)` (delegates held weakly).

## Lifecycle

### Prerequisites
- The Zoom Video SDK must be initialized **and** registered with the toolkit via `ZMUIToolKitManager.initialize(videoSDK:bundleId:)` **before** the view becomes useful. The view reads `ZMUIToolKitManager.videoSDK?.getSession()` at construction; if there is no session yet, it skips populating users/shares and waits for `onSessionJoin`/`onUserJoin` callbacks.
- Must be constructed and used on the main thread (`@MainActor`).

### Entry / Exit
- **Construction** (`init()`): builds the scroll view, page control, self-overlay layer, and recording indicator; touches `ZMRecordingManager.shared` to ensure the recording manager is registered as an SDK delegate (so the consent dialog and "recording stopped" toast are delivered); syncs the recording indicator; creates the first (empty) video page; registers for orientation / app-state / PiP-enabled notifications. If a session already exists, it immediately populates all users, active remote shares, and the PiP active source.
- **Attachment** (`didMoveToWindow()` with `window != nil`): **adds** itself to `ZMUIToolKitManager.videoSDKDelegate`, re-syncs the recording indicator, resumes rendering if the app is active, **attaches** itself to `ZMUIToolkitPiPController.shared` (dedupes internally), and recomputes the PiP active source.
- **Detachment** (`didMoveToWindow()` with `window == nil`): **removes** itself from the delegate fan-out, removes any tracked share views from its scroll view (without destroying them — the share-view singleton owns them), **detaches** PiP, and turns off rendering.
- **Destruction** (`deinit`): removes itself from `ZMUIToolKitManager.videoSDKDelegate` and removes all `NotificationCenter` observers.

> Note: Add the view to a window (place it in your view hierarchy) to begin receiving live session updates; it starts observing SDK events when it appears on screen and stops when removed. Delegate registration is tied to window attachment: the first `didMoveToWindow` with a non-nil window adds the delegate, and removal happens on detachment.

### Delegate Registration Model
The view registers with the toolkit's `CompositeZoomVideoSDKDelegate` fan-out (`ZMUIToolKitManager.videoSDKDelegate`), allowing many toolkit components to observe SDK events simultaneously. Child delegates are stored in a weak hash table; callbacks are forwarded on whatever thread the SDK fires them (documented here as main thread per UIKit assumptions).

### `init?(coder:)`
**Not supported.** Calls `fatalError`. Do **not** instantiate this view from a Storyboard/XIB.

## Page / Layout Model

The ordered page list (`allPages`) is computed dynamically:

```
[ sharePanelView? ] + [ remote share receive views (per activeShareSourceIds) ] + [ video grid pages ]
```

- **Local share panel** (`ZMUIToolkitSharePanelView`): present only while the local user is screen sharing. Always page index 0 when present. Contains a "Stop Sharing" button and a "Share device audio" toggle.
- **Remote share receive views** (`ZMUIToolkitShareReceiveView`): one per active remote share source, ordered by `activeShareSourceIds`. The first remote share starts at index `remoteShareStartIndex` (0 if no local panel, else 1).
- **Video grid pages** (`ZMUIToolkitVideoPage`): displays up to **4** participant video cells per page. 1 participant → primary (full-size) layout; 2–4 → grid layout. Overflow spills onto additional pages.

`currPgIdx` tracks the currently visible page across the whole `allPages` list. Page width = view bounds width; `scrollView.contentSize` is `pageWidth * pageCount`. The visible page renders its video; off-screen pages call `disappearFromScreen()` (unsubscribe) to save resources, gated by `shouldRenderPageContent` (`window != nil && app state == .active`).

### Self-overlay vs. solo presentation
- **`shouldUseSelfOverlay`** is true iff there is at least one remote user (`addedUsers` non-empty).
- **Multi-user**: the local camera is removed from grid pages and shown by the floating `selfOverlayLayerView` overlay cell on top of grid pages. The overlay is hidden on share pages or when the current page is invalid.
- **Solo**: the local user is inserted as a full-size cell in the first video page; the overlay is hidden.

## State / Behavior Drivers

There is no explicit finite state machine; behavior is driven by SDK delegate callbacks, scroll events, and lifecycle notifications. Key derived flags:

| Flag | Meaning |
|------|---------|
| `shouldRenderPageContent` | `window != nil && UIApplication.shared.applicationState == .active`. Gates whether the visible page and self-overlay actually subscribe/render video. |
| `shouldUseSelfOverlay` | `!addedUsers.isEmpty` (any remote users present). Drives overlay-vs-solo self presentation. |
| `remoteShareStartIndex` | `0` if no local share panel, else `1`. First remote-share page index. |
| `currPgIdx` | Index into `allPages` of the visible page. Adjusted on share add/remove, user leave, rebalance, and scroll. |

## APIs

### `init()`
```swift
@objc public init()
```
Designated public initializer. Builds all subviews, registers notification observers, ensures the recording manager exists, and—if a session is already active—populates users, remote shares, and the PiP source.
- **Preconditions**: Toolkit initialized via `ZMUIToolKitManager.initialize` (recommended). Main thread (`@MainActor`).
- **Side effects**: Touches `ZMRecordingManager.shared` (registers it as an SDK delegate); adds `NotificationCenter` observers; may call `displayallUsers()`, `displayActiveRemoteShares()`, `updatePiPActiveUser()`. Adds itself to the delegate fan-out on window attach (`didMoveToWindow`).
- **Returns**: A configured view instance.

### `init?(coder:)`
```swift
required public init?(coder: NSCoder)
```
- **Not supported.** Calls `fatalError`. Do **not** instantiate from a Storyboard/XIB.

### `setPosition(x:y:)`
```swift
@objc public func setPosition(x: Int, y: Int)
```
Sets the view's origin, preserving the current size. Convenience for Objective-C / frame-based layout.
- **Parameters**: `x`, `y` — new origin in points.

### `setSize(width:height:)`
```swift
@objc public func setSize(width: Int, height: Int)
```
Sets the view's size, preserving the current origin. Triggers `layoutSubviews`, which recomputes page frames, page-control/recording-indicator positions, and rebalances grid pages if the per-page cell count changed.
- **Parameters**: `width`, `height` — new size in points.

### `layoutSubviews()` *(override)*
```swift
public override func layoutSubviews()
```
Lays out the scroll view, self-overlay layer, page control, and recording indicator; brings overlay/page-control/indicator to front; and rebalances video pages if `maxVideoCellsPerPage()` changed. Called automatically by UIKit; not typically called directly.

### `didMoveToWindow()` *(override)*
```swift
public override func didMoveToWindow()
```
Window attach/detach hook. On attach: adds delegate, syncs recording indicator, resumes rendering (if app active), attaches PiP, updates PiP source. On detach: removes delegate, removes share views from the scroll view, detaches PiP, stops rendering. Called automatically; not typically called directly.

### Internal helpers (documented for completeness, not public API)
- `displayallUsers()`, `displayActiveRemoteShares()`, `addUsersToVideoPages(users:)` — populate grid pages and shares from the current session.
- `addSharePanel()` / `removeSharePanel()` — show/hide the local share panel page.
- `ensureShareViewDisplayed(user:shareAction:)` / `removeRemoteShareReceiveView(shareSourceId:)` — track/untrack remote share pages (backed by `ZMUIToolkitShareViewManager.shared`).
- `updatePageVisibility()`, `updateRenderStates()`, `applyScrollViewLayout()`, `showPage(index:)` — recompute layout, render state, and current page.
- `updatePiPActiveUser()` / `activeRemoteShareForPiP()` — choose the PiP source.
- `syncRecordingIndicator()` / `updateRecordingIndicator(visible:paused:)` — drive the recording badge.

## Callbacks (`ZoomVideoSDKDelegate`)

All callbacks are delivered via the toolkit's `CompositeZoomVideoSDKDelegate` fan-out and assumed to run on the main thread (UIKit). The view only registers for callbacks while attached to a window.

### `onSessionJoin()`
```swift
public func onSessionJoin()
```
Local user joined the session. Syncs active remote shares for any existing remote users (`syncActiveRemoteShares(for:)`). Does nothing if there is no session or no remote users.
- **Effect**: ensures any in-progress remote shares get a receive view/page.

### `onSessionLeave(_:)`
```swift
public func onSessionLeave(_ reason: ZoomVideoSDKSessionLeaveReason)
```
Session ended. Calls `ZMUIToolkitPiPController.shared.sessionDidEnd()` to tear down PiP for the ended session.
- **Effect**: tears down PiP for the ended session. Does not itself clear the grid (the view is typically removed afterward).

### `onUserJoin(_:users:)`
```swift
public func onUserJoin(_ helper: ZoomVideoSDKUserHelper?, users: [ZoomVideoSDKUser]?)
```
New users joined. Adds them to grid pages (`addUsersToVideoPages`), syncs any remote shares they may already have, and re-evaluates the PiP source. Does nothing if `users` is nil.

### `onUserLeave(_:users:)`
```swift
public func onUserLeave(_ helper: ZoomVideoSDKUserHelper?, users: [ZoomVideoSDKUser]?)
```
Users left. For each: removes any of their remote share pages; deletes their video cell; compacts grid pages (pulls a cell from the tail page into the gap, drops emptied trailing pages). Updates page visibility and PiP source. Does nothing if `users` is nil.

### `onUserAudioStatusChanged(_:user:)`
```swift
public func onUserAudioStatusChanged(_ helper: ZoomVideoSDKAudioHelper?, user users: [ZoomVideoSDKUser]?)
```
Updates the mute icon on each affected user's grid cell, and on the self-overlay cell for the local user. Does nothing if `users` is nil.

### `onUserActiveAudioChanged(_:users:)`
```swift
public func onUserActiveAudioChanged(_ helper: ZoomVideoSDKUserHelper?, users: [ZoomVideoSDKUser]?)
```
Computes the set of actively-talking user IDs (`audioStatus()?.talking == true`) and updates the active-speaker highlight on all grid cells and the self-overlay cell. Does nothing if `users` is nil.

### `onUserVideoStatusChanged(_:user:)`
```swift
public func onUserVideoStatusChanged(_ helper: ZoomVideoSDKVideoHelper?, user users: [ZoomVideoSDKUser]?)
```
For each user whose camera turned on/off: renders or turns off video on their grid cell (and the self-overlay cell for the local user), then re-evaluates the PiP source. Skips users with no video canvas. Does nothing if `users` is nil.

### `onUserNameChanged(_:)`
```swift
public func onUserNameChanged(_ user: ZoomVideoSDKUser?)
```
Updates the displayed name on the user's grid cell, and on the self-overlay cell if it is the local user. Does nothing if user/ID is nil.

### `onVideoCanvasSubscribeFail(_:user:view:)`
```swift
public func onVideoCanvasSubscribeFail(_ failReason: ZoomVideoSDKSubscribeFailReason, user: ZoomVideoSDKUser?, view: UIView?)
```
Video subscription failed for a user. On the main actor (dispatched via `Task { @MainActor }`), turns off that user's video on all grid pages and refreshes page visibility. Does nothing if user is nil.

### `onUserShareStatusChanged(_:user:shareAction:)`
```swift
public func onUserShareStatusChanged(_ helper: ZoomVideoSDKShareHelper?, user: ZoomVideoSDKUser?, shareAction: ZoomVideoSDKShareAction?)
```
Primary share driver. Distinguishes **local** vs **remote** sharer:
- **Local user**: on `.start`/`.resume` → add the local share panel page and jump to it (page 0); on `.none`/`.stop` → remove the panel.
- **Remote user**: on `.start`/`.resume` → ensure a remote share receive view/page exists and jump to it; on `.none`/`.stop` → remove that remote share page.

Does nothing if the local user, `user`, or `shareAction` is nil. Statuses other than start/resume/none/stop are ignored.

### `onCloudRecordingStatus(_:recordAgreementHandler:)`
```swift
public func onCloudRecordingStatus(_ status: ZoomVideoSDKRecordingStatus, recordAgreementHandler handler: ZoomVideoSDKRecordAgreementHandler?)
```
Drives the recording indicator badge only:
- `.start` → indicator visible, not paused (red REC).
- `.pause` → indicator visible, paused (grey pause).
- `.stop` → indicator hidden.
- other statuses → ignored.

> Note: This view **does not** call the `recordAgreementHandler` (consent). Cloud-recording consent and the "recording stopped" toast are handled separately by `ZMRecordingManager.shared`, which `init()` ensures is registered as a delegate. Consent dialog behavior is governed by `ZMUIToolKitManager.cloudRecordingConsentDialogEnabled`.

## UIScrollViewDelegate

### `scrollViewDidEndDecelerating(_:)`
```swift
public func scrollViewDidEndDecelerating(_ scrollView: UIScrollView)
```
After a user swipe settles: computes the new page index, hides/disappears the previous page, shows/refreshes the new page (refreshes metadata tags for video pages), updates the self-overlay, page control, and PiP source. Does nothing if page width is 0 or the index is unchanged/out of range.

## Notification Observers (lifecycle behavior)

| Notification | Handler | Effect |
|--------------|---------|--------|
| `UIDevice.orientationDidChangeNotification` | `handleDeviceOrientationChange` | Rotates local video (`rotateMyVideo`), rebalances grid pages, re-syncs remote shares (they may need re-adding to a new view instance after rotation). |
| `UIApplication.willResignActiveNotification` | `handleAppWillResignActive` | Calls `updateRenderStates()` (stops inline rendering as the app backgrounds; PiP renders off its own raw-data subscription). |
| `UIApplication.didBecomeActiveNotification` | `handleAppDidBecomeActive` | If in a window: stops any active system PiP window and resumes inline rendering. |
| `ZMUIToolkitPiPSettingsStore.pipEnabledDidChangeNotification` | `handlePiPEnabledChanged` | Re-evaluates the PiP active source when the user toggles the PiP setting. |

## Picture-in-Picture Source Selection

`updatePiPActiveUser()` chooses the content that drives the system PiP window, in priority order:

1. An active remote screen share (`activeRemoteShareForPiP()` — prefers the currently-viewed share, else the first live one). Local ReplayKit shares are **ignored** (the system already drives those).
2. Else a **visible** remote camera that is on (from the current video page's user IDs).
3. Else the local camera if it is on.
4. Else nothing (`setActiveSource(nil)`).

The selected source is handed to `ZMUIToolkitPiPController.shared.setActiveSource(_:)`, which dedupes internally. PiP is attached on window-attach and detached on window-detach / `cleanup()` / `sessionDidEnd()`.

## Error Handling

This view focuses on rendering and session presentation. If your app also wants custom handling for additional SDK callbacks, register your own observer alongside it. Behavior notes:

- Video canvas subscribe failures are handled by `onVideoCanvasSubscribeFail` (turns off that cell).
- Share canvas subscription is delegated to `ZMUIToolkitShareReceiveView`, which marks itself subscribed on `.Errors_Success` and retries the subscription on the next `layoutSubviews`/`appearOnScreen`.
- PiP subscriptions are managed by `ZMUIToolkitPiPController`, which retires receivers as sessions and sources change.
- State is reconciled by subsequent SDK delegate callbacks. Register your own `ZoomVideoSDKDelegate` via `ZMUIToolKitManager.videoSDKDelegate.add(_:)` if you want app-specific handling in parallel with the view.

## Rules

- **REQUIRED**: Initialize the toolkit (`ZMUIToolKitManager.initialize`) before the view can render real session content.
- **REQUIRED**: Construct and use the view on the main thread (`@MainActor`).
- **REQUIRED**: Add the view to a window/view hierarchy — delegate registration, rendering, and PiP attachment are all driven by `didMoveToWindow`. The view starts observing SDK events when it appears on screen and stops when removed, so place it in your view hierarchy to receive live session updates.
- **FORBIDDEN**: Instantiating via `init?(coder:)` / Interface Builder (traps with `fatalError`).
- **DO NOT** assign this view (or anything) directly to `ZoomVideoSDK.delegate`; the toolkit owns that slot. Use `ZMUIToolKitManager.videoSDKDelegate.add(_:)`.
- **INVARIANT**: Remote share receive views are owned by the `ZMUIToolkitShareViewManager` singleton; this view only adds/removes them from its scroll view (so they survive parent recreation during rotation).

## Examples

### Example 1: Embed the full video surface (happy path)
```swift
// Assumes ZMUIToolKitManager.initialize(videoSDK:bundleId:) was already called.
let videoView = ZMUIToolkitRootVideoView()
videoView.translatesAutoresizingMaskIntoConstraints = false
containerView.addSubview(videoView)
NSLayoutConstraint.activate([
    videoView.topAnchor.constraint(equalTo: containerView.topAnchor),
    videoView.leadingAnchor.constraint(equalTo: containerView.leadingAnchor),
    videoView.trailingAnchor.constraint(equalTo: containerView.trailingAnchor),
    videoView.bottomAnchor.constraint(equalTo: containerView.bottomAnchor)
])
// No further wiring: it self-registers on window attach and renders the grid,
// shares, recording badge, and PiP automatically.
```

### Example 2: Frame-based placement (Objective-C friendly)
```swift
let videoView = ZMUIToolkitRootVideoView()
view.addSubview(videoView)
videoView.setPosition(x: 0, y: 0)
videoView.setSize(width: Int(view.bounds.width), height: Int(view.bounds.height))
```

### Example 3: Observe SDK events alongside the view
```swift
final class SessionErrorObserver: NSObject, ZoomVideoSDKDelegate {
    func onError(_ errorType: ZoomVideoSDKError, detail details: Int) {
        // Show retry UI, logging, etc.
    }
}
let observer = SessionErrorObserver()
ZMUIToolKitManager.videoSDKDelegate.add(observer) // held weakly — retain `observer` yourself
```

### Example 4: Control the cloud-recording consent dialog
```swift
// Suppress the blocking consent dialog and auto-accept; a non-blocking
// "Recording started" toast is shown instead. Set before recording starts.
ZMUIToolKitManager.cloudRecordingConsentDialogEnabled = false
// The root video view's recording badge reflects start/pause/stop regardless.
```

### Example 5: Lifecycle / teardown
```swift
// Removing from the window triggers delegate removal, PiP detach, and render stop.
videoView.removeFromSuperview()
// Before tearing down the SDK, call cleanup() to release PiP subscriptions and
// the delegate slot:
ZMUIToolKitManager.cleanup()
```

## Code Snippets (Reusable Patterns)

### Determine the PiP active source (mirrors `updatePiPActiveUser` priority)
```swift
// Priority: active remote share > visible remote camera on > local camera on > none.
let session = ZMUIToolKitManager.videoSDK?.getSession()
let myID = session?.getMySelf()?.getID()
let remotes = session?.getRemoteUsers() ?? []
let remoteCameraOn = remotes.first {
    $0.getID() != myID && $0.getVideoCanvas()?.videoStatus()?.on == true
}
// else fall back to local camera if on, else nothing.
```

### Forward your own observer through the fan-out (do not touch ZoomVideoSDK.delegate)
```swift
final class MyObserver: NSObject, ZoomVideoSDKDelegate {
    func onUserShareStatusChanged(_ helper: ZoomVideoSDKShareHelper?,
                                  user: ZoomVideoSDKUser?,
                                  shareAction: ZoomVideoSDKShareAction?) {
        // react to shares without disturbing the root video view
    }
}
let obs = MyObserver()
ZMUIToolKitManager.videoSDKDelegate.add(obs) // weak — retain `obs`
```

## Type Definitions

| Type | Kind | Notes |
|------|------|-------|
| `ZMUIToolkitVideoPage` | internal class (`UIView`) | Holds up to 4 `ZMUIToolkitVideoCell`s; primary (1) vs grid (2–4) layout |
| `ZMUIToolkitVideoCell` | internal class (`UIView`) | One participant's camera; `renderVideo()`, `turnOffVideo()`, `updateMuteIcon()`, `updateDisplayName()`, `updateActiveAudioStatus(_:)` |
| `ZMUIToolkitVideoOverlayLayerView` | internal class (`UIView`) | Floating layer that hosts the self-overlay cell on top of grid pages |
| `ZMUIToolkitSharePanelView` | internal class (`UIView`) | Local screen-share controls (Stop Sharing button, Share device audio toggle) |
| `ZMUIToolkitShareReceiveView` | internal class (`UIView`) | Renders one remote share; subscribes its canvas; exposes `shareAction`, `sharingUserID`, `shareSourceId` |
| `ZMUIToolkitShareViewManager` | singleton (`@MainActor`) | Owns all share receive views (`getOrCreateShareView`, `getShareView`, `removeShareView`) to avoid duplicate subscriptions across rotation |
| `ZMRecordingIndicatorView` | internal class (`UIView`) | "● REC"/paused badge; `setPaused(_:)` |
| `ZMRecordingManager` | singleton (`ZoomVideoSDKDelegate`) | Owns recording consent dialog/toast and recording state (`isRecording`, `isPaused`) |
| `ZMUIToolkitPiPController` | singleton (`AVPictureInPictureControllerDelegate`) | System PiP; `attach(to:)`, `detach()`, `setActiveSource(_:)`, `stopPiPIfActive()`, `discardStoppedShare(shareSourceId:)`, `sessionDidEnd()`, `stop()`; `ActiveSource = .user / .share` |
| `ZMUIToolkitPiPSettingsStore` | store | Persists PiP-enabled flag; posts `pipEnabledDidChangeNotification` |
| `ZMUIToolKitManager` | class | Holds the `ZoomVideoSDK` instance, `videoSDKDelegate` fan-out, `cloudRecordingConsentDialogEnabled` |
| `CompositeZoomVideoSDKDelegate` | class | Weak fan-out delegate; register via `add(_:)` / `remove(_:)` |
| `ZoomVideoSDKShareAction` | SDK class | Share descriptor; `getShareStatus()`, `getShareSourceId()`, `getShareCanvas()` |
| `ZoomVideoSDKShareStatus` | SDK enum | `.start`, `.resume`, `.pause`, `.stop`, `.none` |
| `ZoomVideoSDKRecordingStatus` | SDK enum | `.start`, `.pause`, `.stop` (others ignored) |
| `ZoomVideoSDKUser` | SDK class | Participant; `getID()`, `getVideoCanvas()`, `audioStatus()`, `getShareActionList()` |
| `ZoomVideoSDKSessionLeaveReason` | SDK enum | Reason passed to `onSessionLeave` |
| `ZoomVideoSDKSubscribeFailReason` | SDK enum | Video subscribe failure reason |

## Metadata
- **Source file**: `ZMUIToolkit/ZMUIToolkit/Sources/Video/ZMUIToolkitRootVideoView.swift`
- **Generated for**: AI-assisted code generation and integration
- **Verification**: Behaviors documented above (lifecycle, delegate callbacks implemented and their effects, paging/share/PiP/recording layout, render gating, error handling) were verified directly against the implementation source and its collaborators (`ZMUIToolkitVideoPage`, `ZMUIToolkitShareReceiveView`, `ZMUIToolkitSharePanelView`, `ZMUIToolkitShareViewManager`, `ZMRecordingIndicatorView`, `ZMUIToolkitPiPController`, `ZMUIToolKitManager`, `CompositeZoomVideoSDKDelegate`); not assumed.
