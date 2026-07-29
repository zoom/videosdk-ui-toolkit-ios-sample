# ZMUIToolkitVideoBtn

## Description
`ZMUIToolkitVideoBtn` is an icon-only, self-managing camera/video control button for sessions built on the Zoom Video SDK. On tap it starts or stops the local participant's video; it registers as a `ZoomVideoSDKDelegate` observer and automatically keeps its icon in sync with the authoritative SDK video state. It has exactly two states — `unmute` (video on/live) and `mute` (video off). Unlike `ZMUIToolkitAudioBtn`, there is no separate "join" state; video is simply on or off.

## How to use
Prerequisite: initialize the toolkit first via `ZMUIToolKitManager.initialize(videoSDK:bundleId:)` (see `references/setup.md`). The button reads `ZMUIToolKitManager.videoSDK` during `init()` to pick its initial state. If the toolkit is not initialized, the button constructs in the `mute` state and taps take no effect.

```swift
// Toolkit must already be initialized.
let videoButton = ZMUIToolkitVideoBtn()
videoButton.setSize(size: CGSize(width: 44, height: 44))
toolbarStackView.addArrangedSubview(videoButton)
// Self-registers for video status updates and toggles start/stop video on tap.

// Optional: customize per-state icons (applied immediately if matching current state).
videoButton.setMuteVideoIcon(image: UIImage(named: "custom_video_off")!)   // video off
videoButton.setUnmuteVideoIcon(image: UIImage(named: "custom_video_on")!)  // video on
```

Key public API:
- `init()` — designated initializer; reads initial state, registers as a delegate, syncs.
- `setMuteVideoIcon(image:)` / `setUnmuteVideoIcon(image:)` — override per-state icons.
- `didMoveToWindow()` — re-syncs the icon to current SDK state on attach.
- Inherited styling from `ZMUIToolkitIconBtn`: `setSize`, `setRadius`, `setBackgroundColor`, `setIconColor`, `setIconPadding`, `setPosition`, `setAccessibility(label:hint:)`, etc.

Note: prefer `set*VideoIcon` over inherited `setIcon`/`setImage` — raw images are transient and overwritten on the next state resync.

## Limitations & restrictions
- `@MainActor`: construct and use the button only on the main thread.
- `init?(coder:)` traps with `fatalError` — do NOT instantiate from a Storyboard/XIB or Interface Builder.
- Optimistic UI + authoritative resync: tapping toggles the camera. The icon updates only if the SDK call returns `.Errors_Success`; the displayed state is reconciled by `onUserVideoStatusChanged`.
- Toggle behavior: stopping video updates the icon immediately; starting video is performed asynchronously, and the icon updates on success. Because the class is `@MainActor`, the optimistic `switchTo(...)` updates run on the main thread.
- Responding to outcomes: to respond to outcomes, observe SDK events by registering your own delegate via `ZMUIToolKitManager.videoSDKDelegate.add(_:)` (delegates are held weakly — retain your observer yourself).
- Do NOT assign the button (or anything) to `ZoomVideoSDK.delegate`; the toolkit owns that slot.

## Related components
- `references/setup.md` — REQUIRED first; initializes the Video SDK and toolkit before the button can read state or function.
- `references/icon-btn.md` — superclass providing the inherited icon/layout/styling setters.
- `references/manager.md` — holds `videoSDK` and the `videoSDKDelegate` fan-out the button registers with.
- `references/composite-delegate.md` — the weak fan-out delegate (`add(_:)`/`remove(_:)`) used for SDK callbacks and custom error observation.
- `references/audio-btn.md` — sibling control; differs by having a "join" state that this button lacks.
- `references/share-btn.md`, `references/pause-share-btn.md`, `references/participants-btn.md`, `references/more-btn.md`, `references/leave-session-btn.md` — other session toolbar controls commonly placed alongside.
- `references/root-video-view.md` — renders the video this button enables/disables.
- `references/strings.md` — localized strings for accessibility labels/hints.

## Full API reference
- `docs/api/ZMUIToolkitVideoBtn-API.md`
- `docs/api/ZMUIToolkitVideoBtn-API.json`
