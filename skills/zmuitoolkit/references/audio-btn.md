# ZMUIToolkitAudioBtn

## Description
`ZMUIToolkitAudioBtn` is an icon-only audio control button for sessions built on the Zoom Video SDK. Once added to a view it reads the local participant's audio status, renders the matching icon, and on tap performs the correct action (join audio, mute, or unmute). It self-registers as a `ZoomVideoSDKDelegate` observer and keeps its icon authoritatively in sync with the SDK — for example when the host mutes the user or audio status changes for any reason. It is a subclass of `ZMUIToolkitIconBtn` and conforms to `ZoomVideoSDKDelegate`.

## How to use
Prerequisite: the toolkit must be initialized first via `ZMUIToolKitManager.initialize(videoSDK:bundleId:)` (see `references/setup.md`). If the SDK is `nil`, the button is built in the `join` state and taps have no effect.

```swift
// Toolkit already initialized.
let audioButton = ZMUIToolkitAudioBtn()
audioButton.setSize(size: CGSize(width: 44, height: 44))
toolbarStackView.addArrangedSubview(audioButton)
// No further wiring needed: it self-registers for audio status updates
// and toggles join/mute/unmute on tap.
```

Key public members:
- `init()` — designated initializer. Picks initial state from SDK audio status, configures the button, registers itself with the delegate fan-out, and syncs state.
- `init?(coder:)` — **not supported**; traps with `fatalError`. Do not use Storyboards/XIBs.
- `setJoinAudioIcon(image:)` — override the icon for the `join` state.
- `setMuteAudioIcon(image:)` — override the icon for the `mute` (muted) state.
- `setUnmuteAudioIcon(image:)` — override the icon for the `unmute` (live mic) state.
- Inherited styling from `ZMUIToolkitIconBtn`: `setSize`, `setRadius`, `setBackgroundColor`, `setPosition`, `setIconColor`, `setIconSize`, `setIconPadding`, `setAccessibility`.

State model (`AudioButtonType`): `join` (audio not joined), `mute` (joined + currently muted), `unmute` (joined + live mic). Case names describe the *current* state, not the tap action.

## Limitations & restrictions
- `@MainActor`: construct and use the button only on the main thread.
- `init?(coder:)` traps with `fatalError` — instantiate in code, never from Interface Builder.
- Taps update the UI **optimistically** only after a `.Errors_Success` result; on any other result the icon is left unchanged and the authoritative `onUserAudioStatusChanged` callback reconciles it. To respond to action outcomes, register your own `ZoomVideoSDKDelegate` via `ZMUIToolKitManager.videoSDKDelegate.add(_:)` and observe SDK events.
- Unmute uses a fallback chain (`unmuteAudio(myself)` → `unmuteAudio(nil)` → `startAudio()`); first success wins, otherwise no UI change.
- Do not assign the button (or anything) directly to `ZoomVideoSDK.delegate` — the toolkit owns that slot. Register extra observers via `ZMUIToolKitManager.videoSDKDelegate.add(_:)`.
- Avoid inherited `setIcon`/`setImage` for audio state; it is transient and overwritten on the next resync. Use the per-state `set*AudioIcon` methods instead.
- Delegate observers are held weakly by the composite delegate — retain your own observer objects.

## Related components
- `references/setup.md` — initialization prerequisite; the toolkit/SDK must be set up before constructing this button.
- `references/icon-btn.md` — superclass providing icon/layout/styling configuration this button inherits.
- `references/manager.md` — holds the `ZoomVideoSDK` instance and the `videoSDKDelegate` fan-out the button reads and registers with.
- `references/composite-delegate.md` — the weak fan-out delegate (`CompositeZoomVideoSDKDelegate`) this button adds itself to for audio status events.
- `references/video-btn.md`, `references/share-btn.md`, `references/pause-share-btn.md`, `references/participants-btn.md`, `references/more-btn.md`, `references/leave-session-btn.md` — sibling toolbar buttons commonly placed alongside the audio button.
- `references/root-video-view.md` — the session video surface the toolbar typically overlays.
- `references/strings.md` — localized labels usable for accessibility/customization.

## Full API reference
See `docs/api/ZMUIToolkitAudioBtn-API.md` and `docs/api/ZMUIToolkitAudioBtn-API.json` for the complete spec (states, transitions, callbacks, error handling, type definitions).
