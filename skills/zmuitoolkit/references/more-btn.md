# ZMUIToolkitMoreBtn

## Description
`ZMUIToolkitMoreBtn` is a drop-in "More" (overflow / ellipsis) button for ZMUIToolkit Zoom Video SDK sessions. It is a momentary action button with no persistent visual state: tapping it builds and presents a context-sensitive bottom `CustomActionSheet` whose items reflect live SDK state at tap time. The sheet always offers Settings, conditionally adds recording controls (when `ZMRecordingManager.shared.canControlRecording`), and conditionally adds share pause/resume (when `ZMShareManager.shared.isSharing`). Unlike the audio/video buttons, it is **not** a `ZoomVideoSDKDelegate` and does not observe SDK events.

## How to use
Initialize the toolkit first (see `references/setup.md`), then construct and add the button. Tapping presents the menu automatically.

```swift
// Assumes ZMUIToolKitManager.initialize(videoSDK:bundleId:) was already called.
let moreButton = ZMUIToolkitMoreBtn()
moreButton.setSize(size: CGSize(width: 44, height: 44))
toolbarStackView.addArrangedSubview(moreButton)

// Optional: replace the icon (single visual state, applied immediately)
moreButton.setMoreIcon(image: UIImage(named: "custom_ellipsis")!)

// Optional: localize menu/dialog text from your bundle's ZMUIToolkit.strings
ZMUIToolkitStrings.stringsBundle = Bundle.main
```

Key public members:
- `init()` — designated initializer; builds a 40×40 clear-background `Ellipsis` icon button and installs the tap → action-sheet callback. No delegate registration.
- `setMoreIcon(image:)` — replaces the icon via `setImage(_:for: .normal)`.

Recording behavior: Start and Stop recording route through confirmation alerts; pause/resume of recording and sharing execute immediately. Recording consent (the "this meeting is being recorded" dialog) is **owned by `ZMRecordingManager`**, not this button, and is gated toolkit-wide by `ZMUIToolKitManager.cloudRecordingConsentDialogEnabled` (set `false` to suppress it).

## Limitations & restrictions
- **Not a delegate** — does not conform to `ZoomVideoSDKDelegate`, does not register with `ZMUIToolKitManager.videoSDKDelegate`, and does not observe SDK events; it reads state on demand at tap time.
- **Outcomes** — `startRecording()`, `stopRecording()`, `pauseRecording()`, `resumeRecording()`, `pauseShare()`, `resumeShare()` all return `ZoomVideoSDKError`. To respond to outcomes, observe SDK events via `ZMUIToolKitManager.videoSDKDelegate.add(_:)` (e.g. `onCloudRecordingStatus`).
- **Conditional items** — recording controls appear only when `canControlRecording` is `true`; share controls only when `isSharing` is `true`. If the toolkit/SDK is uninitialized, only Settings (and Cancel) appear.
- `@MainActor` — construct and use on the main thread.
- `init?(coder:)` traps with `fatalError` — do not instantiate from Storyboard/XIB.
- All menu and dialog text is localizable via `ZMUIToolkitStrings`.
- Presentation uses `UIApplication.topViewController()`; ensure a presentable top view controller exists when the button is tapped.

## Related components
- **references/setup.md** — prerequisite: initialize the SDK and toolkit before the button is functional.
- **references/pause-share-btn.md** — closely related; the More menu's share pause/resume items call the same `ZMShareManager.shared.pauseShare()` / `resumeShare()`.
- **references/strings.md** — all menu/alert text is resolved through `ZMUIToolkitStrings`; set `stringsBundle` to customize.
- **references/leave-session-btn.md** — a similar menu/action-style button (presents a confirmation/action UI rather than toggling visual state).
- **references/icon-btn.md** — the `ZMUIToolkitIconBtn` superclass providing icon/layout config and tap dispatch.
- **references/manager.md** — `ZMUIToolKitManager` holds the SDK instance and `cloudRecordingConsentDialogEnabled`.

## Full API reference
- `docs/api/ZMUIToolkitMoreBtn-API.md`
- `docs/api/ZMUIToolkitMoreBtn-API.json`
