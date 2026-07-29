# ZMUIToolkitLeaveSessionBtn

## Description
`ZMUIToolkitLeaveSessionBtn` is a drop-in, momentary action button (subclass of `ZMUIToolkitIconBtn` / `UIButton`) that lets the local participant leave or, if host, end a Zoom Video SDK session. It has no persistent visual state: tapping it presents a host-aware action sheet. A non-host sees *Leave session* + *Cancel*; a host additionally sees *End session for all*, and when remote users are present the *Leave session* option opens an "Assign a host" panel to transfer the host role before leaving. It is **not** a `ZoomVideoSDKDelegate` and does not observe SDK events.

## How to use
Prerequisite: the toolkit must be initialized first via `ZMUIToolKitManager.initialize(videoSDK:bundleId:)` — see **references/setup.md**. The tap callback reads `ZMUIToolKitManager.videoSDK` at tap time to resolve the local user, host status, and remote users.

```swift
// Assumes ZMUIToolKitManager.initialize(videoSDK:bundleId:) was already called.
let leaveButton = ZMUIToolkitLeaveSessionBtn()
leaveButton.setSize(size: CGSize(width: 44, height: 44))
toolbarStackView.addArrangedSubview(leaveButton)
// Tapping presents the host-aware leave/end action sheet automatically.
```

Key methods:
- `init()` — designated public initializer; builds a 40×40 red button with the `LeaveSession` icon and installs the tap callback. No SDK delegate is registered.
- `setLeaveSessionIcon(image:)` — replaces the icon (applied immediately, single visual state).

All menu/dialog text is localizable via `ZMUIToolkitStrings` (set `ZMUIToolkitStrings.stringsBundle`); see **references/strings.md**. Keys include `leave_session`, `end_session_for_all`, `cancel`, `end_session_alert_title`, `assign_host`, `assign_and_leave`.

## Limitations & restrictions
- **Not a delegate**: does not observe SDK events and does not register with `ZMUIToolKitManager.videoSDKDelegate`.
- **Reads SDK state at tap time only** — host status and remote-user count are resolved on each tap, not continuously.
- **Initialize the toolkit first** via `ZMUIToolKitManager.initialize(videoSDK:bundleId:)` so the button can resolve the local user's host status and perform the leave/end action.
- **Observing outcomes**: to respond to the leave/end outcome, observe SDK session events (e.g. `onSessionLeave`) via `ZMUIToolKitManager.videoSDKDelegate.add(_:)`.
- **Presentation** uses `UIApplication.topViewController()` — a presentable top view controller must exist when tapped.
- **`@MainActor`** — construct and use on the main thread.
- **`init?(coder:)` is not supported** (traps with `fatalError`); do not instantiate from Storyboard/XIB.

## Related components
- **references/setup.md** — required initialization order before this button is functional.
- **references/strings.md** — localizing the action sheet, confirmation dialog, and assign-host panel text.
- **references/participants-btn.md** — shares the underlying participants panel used by the assign-host flow.
- **references/more-btn.md** — a similar menu/action button that presents on tap.
- **references/icon-btn.md** — the superclass providing icon/layout configuration.

## Full API reference
- `docs/api/ZMUIToolkitLeaveSessionBtn-API.md`
- `docs/api/ZMUIToolkitLeaveSessionBtn-API.json`
