# ZMUIToolkitMoreBtn API Documentation

## Module Information
- **Module**: ZMUIToolkitMoreBtn
- **Platform**: iOS
- **Language**: Swift (with Objective-C interop via `@objc`)
- **Version**: 1.0
- **Framework**: ZMUIToolkit
- **Superclass**: `ZMUIToolkitIconBtn` (which subclasses `UIButton`)
- **Conforms to**: *(none — unlike `ZMUIToolkitAudioBtn`, this button is not a `ZoomVideoSDKDelegate`)*
- **Concurrency**: `@MainActor` — all members must be accessed on the main thread.

## Overview

`ZMUIToolkitMoreBtn` is a drop-in "More" (overflow / ellipsis) button that surfaces secondary in-session actions. It is a **momentary action button** with no persistent visual state machine: tapping it builds and presents a context-sensitive bottom action sheet (`CustomActionSheet`) whose items depend on the live SDK state at tap time.

Behavior on tap (`showMoreActionSheet()`):
- **Settings** — always present. Opens the settings bottom panel (`ZMUIToolkitSettingsPanel`) via a `BottomPanelPresenter`.
- **Recording controls** — included only when `ZMRecordingManager.shared.canControlRecording` is `true` (the user has a cloud-recording plan and the feature enabled):
  - If a recording is active or paused (`isRecording || isPaused`): a **Stop recording** item (red) plus either **Resume recording** (when paused) or **Pause recording** (when active).
  - Otherwise (no recording in progress): a **Record** item to start one.
- **Share pause/resume** — included only when `ZMShareManager.shared.isSharing` is `true`: **Resume sharing** (when the share is paused) or **Pause sharing** (when actively sharing).
- **Cancel** — always present (the action sheet's built-in cancel button).

The action sheet is rebuilt from current state on every tap, so the visible items always reflect the latest recording/share status. **Start** and **Stop recording** route through confirmation alerts (`CustomUIAlertController`); pause/resume of both recording and sharing execute immediately with no confirmation.

All user-facing strings are localizable via `ZMUIToolkitStrings`.

## Lifecycle

### Prerequisites
- The Zoom Video SDK must be initialized and registered with the toolkit via `ZMUIToolKitManager.initialize(videoSDK:bundleId:)` before tapping the button. The action items read SDK-backed state through `ZMRecordingManager.shared` and `ZMShareManager.shared`, both of which resolve helpers from `ZMUIToolKitManager.videoSDK`.
- (Optional) Set `ZMUIToolkitStrings.stringsBundle` to localize the menu/dialog text.

### Entry / Exit
- **Construction** (`init()`): builds the underlying `ZMUIToolkitIconBtn` descriptor (40×40, **clear** background, `Ellipsis` icon, `secondaryLabel` icon color, zero icon padding), installs the tap callback that calls `showMoreActionSheet()`, and calls `super.init`. **Does not register any SDK delegate.** Creates a `BottomPanelPresenter` instance (`presenter`) used for the Settings/QoS panels.
- **Destruction**: No custom `deinit`. The `BottomPanelPresenter` weakly references the presented panel view controller.

### Difference from `ZMUIToolkitAudioBtn`
This button does **not** observe SDK events and does **not** add/remove itself from `ZMUIToolKitManager.videoSDKDelegate`. It reads SDK-derived state on demand (at tap time, via the manager singletons) rather than continuously syncing. Note, however, that `ZMRecordingManager.shared` (a separate singleton) *does* register itself as a delegate to drive recording-consent dialogs and toasts; the button merely invokes its control methods.

## State Machine

The button itself has no persistent state. The **action sheet contents** are a decision tree evaluated fresh on each tap:

```
tap → showMoreActionSheet()
 ├─ [Settings]  (always) ─────────────► showSettingsPanel()
 ├─ if ZMRecordingManager.shared.canControlRecording:
 │     ├─ isRecording || isPaused
 │     │     ├─ [Stop recording] (red) ─► showStopRecordingAlert() ─► confirm ─► stopRecording()
 │     │     └─ isPaused
 │     │           ├─ true  ─► [Resume recording] ─► resumeRecording()
 │     │           └─ false ─► [Pause recording]  ─► pauseRecording()
 │     └─ else (not recording)
 │           └─ [Record] ─► showStartRecordingAlert() ─► confirm ─► startRecording()
 ├─ if ZMShareManager.shared.isSharing:
 │     ├─ isPaused == true  ─► [Resume sharing] ─► ZMShareManager.shared.resumeShare()
 │     └─ isPaused == false ─► [Pause sharing]  ─► ZMShareManager.shared.pauseShare()
 └─ [Cancel] (always — action sheet cancel button)
```

### Item ordering
Items are appended in this fixed order: Settings, then recording control(s), then share control. The cancel button is rendered separately by `CustomActionSheet`.

### Settings panel toggle
`showSettingsPanel()` toggles the bottom panel: if `presenter.isPresented` it dismisses; otherwise it presents a `ZMUIToolkitSettingsPanel`. The panel's `onStatisticsTapped` closure dismisses the settings panel and, after a 0.3s delay (`DispatchQueue.main.asyncAfter`), presents a `ZMUIToolkitQoSPanel`.

## APIs

### `init()`
```swift
@objc public init()
```
Designated public initializer. Builds the button (40×40, clear background, `Ellipsis` icon, zero icon padding, `secondaryLabel` icon color) and installs the tap callback that presents the More action sheet via `showMoreActionSheet()`.
- **Preconditions**: Toolkit initialized via `ZMUIToolKitManager.initialize` (for functional taps). Main thread (`@MainActor`).
- **Side effects**: None beyond `super.init`. No delegate registration. Constructs the `BottomPanelPresenter`.
- **Returns**: A configured button instance.

### `init?(coder:)`
```swift
@MainActor required public init?(coder: NSCoder)
```
- **Not supported.** Calls `fatalError`. Do **not** instantiate from a Storyboard/XIB.

### `setMoreIcon(image:)`
```swift
public func setMoreIcon(image: UIImage)
```
Replaces the button's icon. Since this button has a single visual state, the image is applied immediately via `setImage(image, for: .normal)`.
- **Parameters**: `image` — the replacement icon.

### `showMoreActionSheet()` *(private)*
```swift
private func showMoreActionSheet()
```
Builds the `[CustomActionSheet.Action]` array from live state (always adds Settings; conditionally adds recording controls when `canControlRecording`, and share controls when `isSharing`) and presents a `CustomActionSheet` from `UIApplication.topViewController()`, with the cancel title from `cancelText`. Private; documented for completeness.

### `showStartRecordingAlert()` *(private)*
```swift
private func showStartRecordingAlert()
```
Presents a `CustomUIAlertController` (inset button layout) with `start_recording_alert_title` / `start_recording_alert_message`, a **Cancel** action (no handler) and a **Start** action (`.default`) that calls `ZMRecordingManager.shared.startRecording()`.

### `showStopRecordingAlert()` *(private)*
```swift
private func showStopRecordingAlert()
```
Presents a `CustomUIAlertController` (inset button layout) with `stop_recording_alert_title` / `stop_recording_alert_message`, a **Cancel** action and a **Stop** action (`.default`) that calls `ZMRecordingManager.shared.stopRecording()`.

### `showSettingsPanel()` *(private)*
```swift
private func showSettingsPanel()
```
Toggles the settings bottom panel via the `BottomPanelPresenter`: dismisses if already presented; otherwise presents a `ZMUIToolkitSettingsPanel` and wires its `onStatisticsTapped` to dismiss settings and (after 0.3s) present a `ZMUIToolkitQoSPanel`.

## Manager Methods Invoked

These are not methods of `ZMUIToolkitMoreBtn`; they are the singleton entry points the action items call. These methods return `ZoomVideoSDKError`; to respond to outcomes, observe SDK events via `ZMUIToolKitManager.videoSDKDelegate.add(_:)`.

| Caller item | Method | Underlying SDK call | Notes |
|-------------|--------|---------------------|-------|
| Record (after confirm) | `ZMRecordingManager.shared.startRecording()` | `recordingHelper.canStartRecording()` then `startCloudRecording()` | Returns early if `canStartRecording() != .Errors_Success`. Sets an internal `didInitiateRecording` flag on success so the initiator skips the consent dialog. |
| Stop recording (after confirm) | `ZMRecordingManager.shared.stopRecording()` | `recordingHelper.stopCloudRecording()` | — |
| Pause recording | `ZMRecordingManager.shared.pauseRecording()` | `recordingHelper.pauseCloudRecording()` | Immediate, no confirmation. |
| Resume recording | `ZMRecordingManager.shared.resumeRecording()` | `recordingHelper.resumeCloudRecording()` | Immediate, no confirmation. |
| Pause sharing | `ZMShareManager.shared.pauseShare()` | `shareHelper.pauseShare()` | Immediate, no confirmation. |
| Resume sharing | `ZMShareManager.shared.resumeShare()` | `shareHelper.resumeShare()` | Immediate, no confirmation. |

### State queries that gate the items

| Query | Definition | Used to gate |
|-------|-----------|--------------|
| `ZMRecordingManager.shared.canControlRecording` | `recordingHelper?.canStartRecording() == .Errors_Success` | Whether any recording item is shown |
| `ZMRecordingManager.shared.isRecording` | `getCloudRecordingStatus() == .start` | Stop/Pause vs. Record |
| `ZMRecordingManager.shared.isPaused` | `getCloudRecordingStatus() == .pause` | Resume vs. Pause recording |
| `ZMShareManager.shared.isSharing` | `shareHelper?.isScreenSharingOut() ?? false` | Whether any share item is shown |
| `ZMShareManager.shared.isPaused` | share action list contains a `.pause` status | Resume vs. Pause sharing |

## Callbacks

This button implements **no SDK delegate callbacks**. It interacts with the SDK indirectly (through `ZMRecordingManager` / `ZMShareManager`) at the moment of user interaction.

UI callbacks used internally:
- The tap callback (`ZMUIToolkitIconBtn` click action) — builds and presents the action sheet.
- `CustomActionSheet.Action.handler` — runs after the sheet dismisses (recording/share control closures, or `showStart/StopRecordingAlert()` / `showSettingsPanel()`).
- `CustomUIAlertController.Action.handler` — Start/Stop recording confirmation handlers.
- `ZMUIToolkitSettingsPanel.onStatisticsTapped` — dismisses settings, then presents the QoS panel after a 0.3s delay.

> Recording-consent prompts (the "This meeting is being recorded" dialog) are owned by `ZMRecordingManager`'s own `ZoomVideoSDKDelegate` implementation (`onCloudRecordingStatus`), **not** by this button.

## Localized Strings (`ZMUIToolkitStrings`)

All menu and dialog text is resolved through `ZMUIToolkitStrings.string(for:default:)`. To customize, set `ZMUIToolkitStrings.stringsBundle` and provide a `ZMUIToolkit.strings` table.

| Key | Default value | Used for |
|-----|---------------|----------|
| `settings` | "Settings" | Settings action sheet item |
| `cancel` | "Cancel" | Action sheet cancel + alert cancel buttons |
| `record` | "Record" | Start-recording action sheet item |
| `stop_recording` | "Stop recording" | Stop-recording action sheet item (red) |
| `pause_recording` | "Pause recording" | Pause-recording action sheet item |
| `resume_recording` | "Resume recording" | Resume-recording action sheet item |
| `start_recording_alert_title` | "Start recording" | Start confirmation dialog title |
| `start_recording_alert_message` | "After the recording starts, all participants will be informed and the meeting audio, video and shared screen view will be recorded to the cloud." | Start confirmation dialog message |
| `start` | "Start" | Start confirmation dialog confirm button |
| `stop_recording_alert_title` | "Stop Recording" | Stop confirmation dialog title |
| `stop_recording_alert_message` | "You will receive an email notification when the cloud recording is ready." | Stop confirmation dialog message |
| `stop` | "Stop" | Stop confirmation dialog confirm button |
| `pause_share` | "Pause sharing" | Pause-share action sheet item |
| `resume_share` | "Resume sharing" | Resume-share action sheet item |

## Error Handling

`startRecording()`, `stopRecording()`, `pauseRecording()`, `resumeRecording()`, `pauseShare()`, and `resumeShare()` return `ZoomVideoSDKError`. To respond to outcomes, observe SDK events via `ZMUIToolKitManager.videoSDKDelegate.add(_:)`.

- If the toolkit/SDK is not initialized, `ZMUIToolKitManager.videoSDK` is `nil`. The manager helpers then resolve to `nil`, so:
  - `canControlRecording` is `false` and `isSharing` is `false` → only **Settings** (and Cancel) appear in the sheet.
  - A control method called with a nil helper returns `.Errors_Wrong_Usage`.
- The action sheet and alerts are presented from `UIApplication.topViewController()`. Presentation requires a presentable top view controller.

Integrators needing to react to recording/share outcomes should observe the SDK directly (e.g. `onCloudRecordingStatus`, share status callbacks) via their own `ZoomVideoSDKDelegate` registered with `ZMUIToolKitManager.videoSDKDelegate.add(_:)`.

## Rules

- **REQUIRED**: Initialize the toolkit (`ZMUIToolKitManager.initialize`) before the button is tapped for functional recording/share behavior.
- **REQUIRED**: Construct and use on the main thread (`@MainActor`).
- **FORBIDDEN**: Instantiating via `init?(coder:)` / Interface Builder (traps with `fatalError`).
- **NOTE**: The button reads SDK-derived state at tap time only (via `ZMRecordingManager`/`ZMShareManager`); it does not observe SDK events.
- **NOTE**: Recording controls only appear when `canControlRecording` is `true`; share controls only when `isSharing` is `true`.
- **NOTE**: Start and Stop recording are gated behind confirmation alerts; pause/resume of recording and sharing are immediate.
- **NOTE**: Presentation uses `UIApplication.topViewController()`; ensure a presentable top view controller exists when the button is tapped.

## Examples

### Example 1: Add the button to a toolbar (happy path)
```swift
// Assumes ZMUIToolKitManager.initialize(videoSDK:bundleId:) was already called.
let moreButton = ZMUIToolkitMoreBtn()
moreButton.setSize(size: CGSize(width: 44, height: 44))
toolbarStackView.addArrangedSubview(moreButton)
// Tapping presents the context-sensitive More action sheet automatically.
```

### Example 2: Customize the icon and localize the strings
```swift
let moreButton = ZMUIToolkitMoreBtn()
moreButton.setMoreIcon(image: UIImage(named: "custom_ellipsis")!)

// Localize menu/dialog text from your app bundle's ZMUIToolkit.strings table.
ZMUIToolkitStrings.stringsBundle = Bundle.main
```

### Example 3: Suppress the recording-consent dialog (toolkit-level toggle)
```swift
// The More button starts recording via ZMRecordingManager, which presents a
// consent dialog to participants. Disable that dialog toolkit-wide if desired.
ZMUIToolKitManager.cloudRecordingConsentDialogEnabled = false
```

### Example 4: React to the recording outcome via the SDK delegate
```swift
final class RecordingObserver: NSObject, ZoomVideoSDKDelegate {
    func onCloudRecordingStatus(_ status: ZoomVideoSDKRecordingStatus,
                                recordAgreementHandler handler: ZoomVideoSDKRecordAgreementHandler?) {
        // Update your own UI based on status (.start / .pause / .stop).
        handler?.accept()
    }
}
let observer = RecordingObserver()
ZMUIToolKitManager.videoSDKDelegate.add(observer) // held weakly — retain `observer` yourself
```

## Code Snippets (Reusable Patterns)

### Decide which recording item to show (the way the button does)
```swift
let manager = ZMRecordingManager.shared
if manager.canControlRecording {
    if manager.isRecording || manager.isPaused {
        // show Stop recording, plus Resume (if paused) or Pause (if active)
    } else {
        // show Record (start)
    }
}
```

### Decide which share item to show
```swift
if ZMShareManager.shared.isSharing {
    if ZMShareManager.shared.isPaused {
        ZMShareManager.shared.resumeShare()
    } else {
        ZMShareManager.shared.pauseShare()
    }
}
```

## Type Definitions

| Type | Kind | Notes |
|------|------|-------|
| `ZMUIToolkitIconBtn` | open class (superclass) | UIButton subclass providing icon/layout config and click callback dispatch |
| `ZMUIToolkitIconBtnDescriptor` | struct | Initializer configuration (frame, backgroundColor, accessibilityLabel, iconname, iconPadding, iconColor, callback) |
| `ZMUIToolKitManager` | class | Holds the `ZoomVideoSDK` instance, the `videoSDKDelegate` fan-out, and `cloudRecordingConsentDialogEnabled` |
| `ZMUIToolkitStrings` | enum | Localization helper; set `stringsBundle` to customize text (table `ZMUIToolkit`) |
| `ZMRecordingManager` | internal final class (singleton, `ZoomVideoSDKDelegate`) | Owns cloud-recording control (start/stop/pause/resume), `canControlRecording`/`isRecording`/`isPaused`, and the consent dialog/toasts |
| `ZMShareManager` | internal final class (singleton) | Owns local screen-share pause/resume control and `isSharing`/`isPaused` queries |
| `CustomActionSheet` | internal class (UIViewController) | Custom bottom action sheet presented over full screen |
| `CustomActionSheet.Action` | struct | `title`, `icon` (optional), `textColor`, `handler` |
| `CustomUIAlertController` | internal class (UIViewController) | Custom modal alert used for Start/Stop recording confirmation |
| `CustomUIAlertController.Action` | struct | `title`, `style` (`.default`/`.confirm`/`.cancel`), `handler` |
| `BottomPanelPresenter` | internal class | Presents/dismisses a single bottom panel; weakly references the panel VC |
| `ZMUIToolkitSettingsPanel` | internal class (UIView, `BottomPanelClosable`) | Settings bottom panel; exposes `onStatisticsTapped` |
| `ZMUIToolkitQoSPanel` | internal class (UIView, `BottomPanelClosable`) | Statistics/QoS bottom panel presented from Settings |
| `ZoomVideoSDKRecordingHelper` | SDK class | `canStartRecording()`, `getCloudRecordingStatus()`, `startCloudRecording()`, `stopCloudRecording()`, `pauseCloudRecording()`, `resumeCloudRecording()` |
| `ZoomVideoSDKShareHelper` | SDK class | `isScreenSharingOut()`, `pauseShare()`, `resumeShare()` |
| `ZoomVideoSDKError` | SDK enum | Return type of the control methods; observe SDK events via `ZMUIToolKitManager.videoSDKDelegate.add(_:)` to respond to outcomes |

## Metadata
- **Source file**: `ZMUIToolkit/ZMUIToolkit/Sources/Buttons/ZMUIToolkitMoreBtn.swift`
- **Generated for**: AI-assisted code generation and integration
- **Verification**: Behaviors documented above were verified directly against the implementation source (`ZMUIToolkitMoreBtn.swift`, `ZMUIToolkitIconBtn.swift`, `ZMUIToolkitDescriptor.swift`, `ZMRecordingManager.swift`, `ZMShareManager.swift`, `CustomActionSheet.swift`, `CustomUIAlertController.swift`, `BottomPanelPresenter.swift`, `ZMUIToolkitStrings.swift`, `ZMUIToolKitManager.swift`), not assumed.
