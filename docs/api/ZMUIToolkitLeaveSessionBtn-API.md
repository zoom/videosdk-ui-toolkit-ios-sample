# ZMUIToolkitLeaveSessionBtn API Documentation

## Module Information
- **Module**: ZMUIToolkitLeaveSessionBtn
- **Platform**: iOS
- **Language**: Swift (with Objective-C interop via `@objc`)
- **Version**: 1.0
- **Framework**: ZMUIToolkit
- **Superclass**: `ZMUIToolkitIconBtn` (which subclasses `UIButton`)
- **Conforms to**: *(none — this button is not a `ZoomVideoSDKDelegate`)*
- **Concurrency**: `@MainActor` — all members must be accessed on the main thread.

## Overview

`ZMUIToolkitLeaveSessionBtn` is a drop-in button that lets the local participant leave (or, if host, end) a Zoom Video SDK session. It is a **momentary action button** with no persistent visual state machine: tapping it presents a context-sensitive action sheet whose options depend on whether the local user is the **host**.

Behavior on tap:
- **Non-host**: action sheet with *Leave session* and *Cancel*. *Leave session* calls `leaveSession(false)`.
- **Host with remote users present**: *Leave session* opens an "Assign a host" bottom panel so the host can transfer host role before leaving. *End session for all* presents a confirmation dialog that ends the session for everyone.
- **Host with no remote users**: *Leave session* directly calls `leaveSession(true)` (ends the empty session).

All user-facing strings are localizable via `ZMUIToolkitStrings`.

## Lifecycle

### Prerequisites
- The Zoom Video SDK must be initialized and registered with the toolkit via `ZMUIToolKitManager.initialize(videoSDK:bundleId:)` before the button is used, so it can resolve the local user's host status and perform the leave/end action. The tap callback reads `ZMUIToolKitManager.videoSDK` to resolve the local user, host status, remote users, and to call `leaveSession`/`makeHost`.
- (Optional) Set `ZMUIToolkitStrings.stringsBundle` to localize the menu/dialog text.

### Entry / Exit
- **Construction** (`init()`): builds the underlying `ZMUIToolkitIconBtn` descriptor (40×40, **red** background, `LeaveSession` icon, `secondaryLabel` icon color), installs the tap callback, calls `super.init`. It does not register any SDK delegate.
- **Destruction**: The button holds a `BottomPanelPresenter` which weakly references the presented panel view controller.

### Delegate model
This button does **not** observe SDK events and does **not** add/remove itself from `ZMUIToolKitManager.videoSDKDelegate`. It reads SDK state on demand (at tap time) rather than continuously syncing.

## State Machine

The button itself has no persistent state. However, the **leave flow** it drives has a decision tree:

```
tap
 ├─ localUser == nil ───────────────► action sheet (isHost = false)
 └─ localUser present
      └─ isHost = localUser.isHost()
           ├─ isHost == false ─────► action sheet: [Leave session, Cancel]
           │      └─ Leave session ─► leaveSession(false)
           └─ isHost == true ──────► action sheet: [Leave session, End session for all, Cancel]
                  ├─ Leave session
                  │     ├─ remoteUsers.count > 0 ─► showAssignHostPanel()
                  │     │        └─ Assign and Leave ─► makeHost(selected) + leaveSession(false)
                  │     └─ remoteUsers.count == 0 ─► leaveSession(true)
                  └─ End session for all ─► confirm dialog
                         └─ Confirm ─► leaveSession(true)
```

### `leaveSession(_:)` flag semantics
The `Bool` passed to `ZoomVideoSDK.leaveSession(_:)` is the **end-session flag**:
- `leaveSession(false)` — the local user leaves; the session continues for others.
- `leaveSession(true)` — the session ends for all participants.

## APIs

### `init()`
```swift
@objc public init()
```
Designated public initializer. Builds the button (40×40, red background, `LeaveSession` icon, zero icon padding, `secondaryLabel` icon color) and installs the tap callback that presents the host-aware action sheet.
- **Preconditions**: Toolkit initialized via `ZMUIToolKitManager.initialize` (for functional taps). Main thread (`@MainActor`).
- **Side effects**: Beyond `super.init`, no delegate registration is performed.
- **Returns**: A configured button instance.

### `init?(coder:)`
```swift
@MainActor required public init?(coder: NSCoder)
```
- **Not supported.** Calls `fatalError`. Do **not** instantiate from a Storyboard/XIB.

### `setLeaveSessionIcon(image:)`
```swift
public func setLeaveSessionIcon(image: UIImage)
```
Replaces the button's icon. Since this button has a single visual state, the image is applied immediately via `setImage(image, for: .normal)`.
- **Parameters**: `image` — the replacement icon.

### `displayActionSheetMenu(isHost:)` *(internal)*
```swift
func displayActionSheetMenu(isHost: Bool)
```
Builds and presents the leave/end action sheet (`UIAlertController`, `.actionSheet`). Adds *End session for all* only when `isHost == true`. Configures iPad popover anchoring to the button, then presents from the top-most view controller (`UIApplication.topViewController()`). Internal; documented for completeness.

### `showAssignHostPanel()` *(private)*
Toggles a bottom panel via `BottomPanelPresenter`: dismisses if already presented, otherwise presents the assign-host participants panel.

### `createAssignHostPanel()` *(private)*
Builds a `ZMUIToolkitParticipantsPanel` configured with: title "Assign a host", base cell style, persistent single selection, `observeSDKEvents: false`, and a search bar. Adds a single *Assign and Leave* primary button (initially disabled) that is enabled only when a participant is selected (via `onSelectionChanged`).

### `assignHostAndLeave(panel:)` *(private)*
Resolves the selected participant to its SDK `ZoomVideoSDKUser` (matching `getID()`), calls `userHelper.makeHost(user)`, then `leaveSession(false)`, and dismisses the panel. Takes no action if there is no selection or the user cannot be resolved.

## Callbacks

This button implements **no SDK delegate callbacks**. It interacts with the SDK imperatively (read state and call actions) at the moment of user interaction.

UI callbacks used internally:
- The tap callback (`ZMUIToolkitIconBtn` click action) — resolves host status and presents the action sheet.
- `ZMUIToolkitParticipantsPanel.onSelectionChanged` — enables/disables the *Assign and Leave* button.

## Localized Strings (`ZMUIToolkitStrings`)

All menu and dialog text is resolved through `ZMUIToolkitStrings.string(for:default:)`. To customize, set `ZMUIToolkitStrings.stringsBundle` and provide a `ZMUIToolkit.strings` table.

| Key | Default value | Used for |
|-----|---------------|----------|
| `leave_session` | "Leave session" | First action sheet item |
| `end_session_for_all` | "End session for all" | Host-only action sheet item |
| `cancel` | "Cancel" | Action sheet + confirm dialog cancel |
| `end_session_alert_title` | "End session for all" | Confirm dialog title |
| `end_session_alert_message` | "Are you sure you want to end the session for all participants?" | Confirm dialog message |
| `confirm` | "Confirm" | Confirm dialog confirm button |
| `assign_host` | "Assign a host" | Assign-host panel title |
| `assign_and_leave` | "Assign and Leave" | Assign-host panel primary button |

## Responding to leave/end outcomes

The button drives the SDK calls (`leaveSession(_:)`, `makeHost(_:)`) imperatively at tap time. To respond to the leave/end outcome, observe SDK session events (e.g. `onSessionLeave`) via `ZMUIToolKitManager.videoSDKDelegate.add(_:)`.

Initialize the toolkit before the button is used so it can resolve the local user's host status and perform the leave/end action. With the toolkit initialized, `ZMUIToolKitManager.videoSDK` resolves the local user, host status, and remote users, and the action sheet reflects the correct host/non-host options.

## Rules

- **REQUIRED**: Initialize the toolkit (`ZMUIToolKitManager.initialize`) before the button is tapped for functional leave/end behavior.
- **REQUIRED**: Construct and use on the main thread (`@MainActor`).
- **FORBIDDEN**: Instantiating via `init?(coder:)` / Interface Builder (traps with `fatalError`).
- **NOTE**: The button reads SDK state at tap time only; it does not observe SDK events.
- **NOTE**: Presentation uses `UIApplication.topViewController()`; ensure a presentable top view controller exists when the button is tapped.

## Examples

### Example 1: Add the button to a toolbar (happy path)
```swift
// Assumes ZMUIToolKitManager.initialize(videoSDK:bundleId:) was already called.
let leaveButton = ZMUIToolkitLeaveSessionBtn()
leaveButton.setSize(size: CGSize(width: 44, height: 44))
toolbarStackView.addArrangedSubview(leaveButton)
// Tapping presents the host-aware leave/end action sheet automatically.
```

### Example 2: Customize the icon and localize the strings
```swift
let leaveButton = ZMUIToolkitLeaveSessionBtn()
leaveButton.setLeaveSessionIcon(image: UIImage(named: "custom_leave")!)

// Localize menu/dialog text from your app bundle's ZMUIToolkit.strings table.
ZMUIToolkitStrings.stringsBundle = Bundle.main
```

### Example 3: React to the leave/end outcome via SDK events
```swift
final class SessionExitObserver: NSObject, ZoomVideoSDKDelegate {
    func onSessionLeave(_ reason: ZoomVideoSDKSessionLeaveReason) {
        // Tear down UI, navigate away, etc.
    }
}
let observer = SessionExitObserver()
ZMUIToolKitManager.videoSDKDelegate.add(observer) // held weakly — retain `observer` yourself
```

## Code Snippets (Reusable Patterns)

### Resolve host status the way the button does
```swift
let localUser = ZMUIToolKitManager.videoSDK?.getSession()?.getMySelf()
let isHost = localUser?.isHost() ?? false
```

### Decide leave vs. assign-host (host path)
```swift
let remoteCount = ZMUIToolKitManager.videoSDK?.getSession()?.getRemoteUsers()?.count ?? 0
if remoteCount > 0 {
    // present assign-host panel
} else {
    ZMUIToolKitManager.videoSDK?.leaveSession(true) // end empty session
}
```

## Type Definitions

| Type | Kind | Notes |
|------|------|-------|
| `ZMUIToolkitIconBtn` | open class (superclass) | UIButton subclass providing icon/layout config |
| `ZMUIToolkitIconBtnDescriptor` | struct | Initializer configuration (frame, colors, icon name, padding, callback) |
| `ZMUIToolKitManager` | class | Holds the `ZoomVideoSDK` instance and `videoSDKDelegate` fan-out |
| `ZMUIToolkitStrings` | enum | Localization helper; set `stringsBundle` to customize text |
| `BottomPanelPresenter` | internal class | Presents/dismisses a single bottom panel; weakly references the panel VC |
| `CustomUIAlertController` | internal class | Custom modal alert used for the End-session confirmation dialog |
| `CustomUIAlertController.Action` | struct | `title`, `style` (`.default`/`.confirm`/`.cancel`), `handler` |
| `ZMUIToolkitParticipantsPanel` | class | Bottom panel listing participants; used for assign-host |
| `ZMUIToolkitParticipantsPanelConfiguration` | struct | Panel config (title, cellStyle, selectionMode, observeSDKEvents, showSearchBar) |
| `ZMUIToolkitPanelButton` | type | Panel action button (title, style, initiallyEnabled, handler) |
| `ZoomVideoSDKUser` | SDK class | Local user via `getMySelf()`; remote users via `getRemoteUsers()` |
| `ZoomVideoSDKUserHelper` | SDK class | Provides `makeHost(_:)` |

## Metadata
- **Source file**: `ZMUIToolkit/ZMUIToolkit/Sources/Buttons/ZMUIToolkitLeaveSessionBtn.swift`
- **Generated for**: AI-assisted code generation and integration
- **Verification**: Behaviors documented above were verified directly against the implementation source (`ZMUIToolkitLeaveSessionBtn.swift`, `ZMUIToolkitStrings.swift`, `BottomPanelPresenter.swift`, `CustomUIAlertController.swift`), not assumed.
