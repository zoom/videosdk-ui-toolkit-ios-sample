# ZMUIToolkitPauseShareBtn

## Description
`ZMUIToolkitPauseShareBtn` is a drop-in, self-managing icon button that toggles the local user's **active screen share** between live (playing) and paused. On tap it delegates to `ZMShareManager.shared` — calling `pauseShare()` while live and `resumeShare()` while paused — and renders the matching icon. It auto-disables (greys out) whenever no share is active and keeps its icon and enabled state in sync with the authoritative SDK share state via a `ZoomVideoSDKDelegate` callback. It subclasses `ZMUIToolkitIconBtn` and is `@MainActor`.

## How to use
Prerequisite: the toolkit must be initialized via `ZMUIToolKitManager.initialize(videoSDK:bundleId:)` and there must be an active screen share to control (see `references/setup.md` for SDK/toolkit init and `references/share-btn.md` for starting a share).

```swift
// Toolkit already initialized; user has an active screen share.
let pauseShareButton = ZMUIToolkitPauseShareBtn()
pauseShareButton.setSize(size: CGSize(width: 44, height: 44))
toolbarStackView.addArrangedSubview(pauseShareButton)
// No further wiring: it self-registers for share-status updates,
// enables only while a share is active, and toggles pause/resume on tap.
```

Key methods:
- `init()` — designated initializer; picks initial state from `ZMShareManager.shared.isPaused`, sets `isEnabled` from `isSharing`, and registers as a delegate observer.
- `setPauseShareIcon(image:)` — override the icon shown while live (the "pause" affordance).
- `setResumeShareIcon(image:)` — override the icon shown while paused (the "resume/play" affordance).
- Inherited styling: `setSize`, `setBackgroundColor`, `setRadius`, `setIconColor`, `setIconPadding`, `setAccessibility(label:hint:)`.

Prefer `setPauseShareIcon` / `setResumeShareIcon` over the inherited `setIcon`/`setImage`, which set the raw image directly and are replaced on the next state resync.

## Limitations & restrictions
- Only meaningful while the local user is actively sharing; the button is **disabled (greyed out)** otherwise and taps are not delivered.
- State is derived from `ZMShareManager.shared` (which reads `ZMUIToolKitManager.videoSDK`); if the toolkit isn't initialized, `isSharing` is `false`, so it constructs disabled, and `pauseShare()`/`resumeShare()` return `.Errors_Wrong_Usage`. The button reflects the share manager's current state.
- The initial icon and enabled state are set inline at construction and reconciled with the SDK share state on attachment (`didMoveToWindow()`) and on the first delegate callback.
- Tap uses **optimistic UI**: on `.Errors_Success` the icon switches immediately; on any other error the icon is left unchanged and reconciled later by `onUserShareStatusChanged`.
- `@MainActor` — must be constructed and used on the main thread.
- `init?(coder:)` traps with `fatalError` — do **not** instantiate from a Storyboard/XIB.
- Controls only the local user's share; it does not reflect or control remote participants' shares. The button does not surface SDK errors — observe them yourself via `ZMUIToolKitManager.videoSDKDelegate.add(_:)`.

## Related components
- `references/share-btn.md` — most related; starts/stops the screen share that this button pauses and resumes. They pair directly in a share workflow.
- `references/setup.md` — initialize the Video SDK and toolkit (required prerequisite) and configure screen sharing / broadcast extension.
- `references/manager.md` — `ZMUIToolKitManager` holds the `videoSDK` and `videoSDKDelegate` fan-out this button reads and registers with.
- `references/composite-delegate.md` — the `CompositeZoomVideoSDKDelegate` fan-out that delivers `onUserShareStatusChanged`; use `add(_:)` to observe errors yourself.
- `references/icon-btn.md` — `ZMUIToolkitIconBtn` superclass providing icon/layout/styling and disabled-state rendering.
- `references/strings.md` — localized labels for accessibility/UI text.

## Full API reference
- Markdown: `docs/api/ZMUIToolkitPauseShareBtn-API.md`
- JSON: `docs/api/ZMUIToolkitPauseShareBtn-API.json`
