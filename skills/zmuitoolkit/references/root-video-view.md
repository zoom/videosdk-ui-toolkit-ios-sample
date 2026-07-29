# ZMUIToolkitRootVideoView

## Description
`ZMUIToolkitRootVideoView` is the toolkit's top-level, self-managing `UIView` for an in-session video experience. Drop it into a view hierarchy and it renders a horizontally paged surface of the participant video grid plus any active screen shares, shows a floating self picture-in-picture overlay of the local camera when remote users are present, and displays a cloud-recording badge ("● REC" / paused). While on screen it registers as an observer on the toolkit's SDK delegate fan-out and reacts to SDK callbacks to keep the grid, shares, recording badge, and system PiP source authoritative. It does **not** own `ZoomVideoSDK.delegate` — the toolkit does.

## How to use
Construct it (no arguments), add it to your hierarchy, and let it fill the superview. No further wiring is needed.

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
// Self-registers on window attach; renders grid, shares, recording badge, and PiP automatically.
```

Key public API:
- `init()` — designated initializer; builds subviews, registers notification observers, ensures `ZMRecordingManager.shared` exists, and (if a session is already active) populates users/shares/PiP source.
- `setPosition(x:y:)` / `setSize(width:height:)` — Objective-C / frame-based layout convenience.
- Overrides `layoutSubviews()` and `didMoveToWindow()` (called automatically; not invoked directly).

Prerequisites:
- Initialize the toolkit first — see **references/setup.md**. The view reads `ZMUIToolKitManager.videoSDK?.getSession()` at construction and waits for `onSessionJoin`/`onUserJoin` if there is no session yet.
- System PiP requires `multitaskingCameraAccessEnabled` to be set at join time — see **references/setup.md** Step 3.
- Teardown: `removeFromSuperview()` triggers delegate removal, PiP detach, and render stop; call `ZMUIToolKitManager.cleanup()` before tearing down the SDK.

## Limitations & restrictions
- **Add the view to a window to begin receiving live session updates.** It registers with the SDK delegate fan-out in `didMoveToWindow` (with a non-nil window): it starts observing SDK events when it appears on screen and stops when removed. Place it in your view hierarchy to receive live session updates.
- **Do not** assign this view (or anything) to `ZoomVideoSDK.delegate` — the toolkit owns that slot. Observe SDK events by registering your own observers via `ZMUIToolKitManager.videoSDKDelegate.add(_:)` (held weakly — retain them yourself).
- Recording **consent** is handled by `ZMRecordingManager`, gated by `ZMUIToolKitManager.cloudRecordingConsentDialogEnabled`. This view drives only the REC badge; it does **not** call the `recordAgreementHandler`.
- Displays up to **4** participant video cells per grid page; overflow spills onto additional pages.
- Register your own `ZoomVideoSDKDelegate` alongside the view if your app wants to react to additional SDK events.
- `@MainActor` — construct and use only on the main thread.
- `init?(coder:)` is not supported (traps with `fatalError`) — instantiate in code rather than from a Storyboard/XIB.

## Related components
- **references/setup.md** — required first (SDK + toolkit init order); also where the PiP join option (`multitaskingCameraAccessEnabled`) is configured.
- **references/audio-btn.md**, **references/video-btn.md**, **references/share-btn.md**, **references/pause-share-btn.md**, **references/participants-btn.md**, **references/more-btn.md**, **references/leave-session-btn.md** — control buttons that typically overlay this video view in a session toolbar.
- **references/manager.md** — owns the `ZoomVideoSDK` instance, the `videoSDKDelegate` fan-out, and `cloudRecordingConsentDialogEnabled`.
- **references/composite-delegate.md** — the weak fan-out (`add(_:)` / `remove(_:)`) this view registers with; use it to observe SDK events alongside this view.
- **references/icon-btn.md** — base class for the toolkit's control buttons.
- **references/strings.md** — localized strings used across the toolkit UI.

## Full API reference
- `docs/api/ZMUIToolkitRootVideoView-API.md`
- `docs/api/ZMUIToolkitRootVideoView-API.json`
