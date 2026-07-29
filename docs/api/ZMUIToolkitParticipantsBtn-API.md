# ZMUIToolkitParticipantsBtn API Documentation

## Module Information
- **Module**: ZMUIToolkitParticipantsBtn
- **Platform**: iOS
- **Language**: Swift (with Objective-C interop via `@objc`)
- **Version**: 1.0
- **Framework**: ZMUIToolkit
- **Superclass**: `ZMUIToolkitIconBtn` (which subclasses `UIButton`)
- **Conforms to**: `ZoomVideoSDKDelegate`
- **Concurrency**: `@MainActor` — all members must be accessed on the main thread.

## Overview

`ZMUIToolkitParticipantsBtn` is a drop-in, self-managing participants control button for sessions built on the Zoom Video SDK. Once added to a view, it:

1. Displays a fixed `Participants` icon and renders a **numeric badge** showing the current participant count (remote users + the local user).
2. Registers itself as a `ZoomVideoSDKDelegate` observer and **automatically keeps the count badge in sync** with user join/leave and session lifecycle events.
3. On tap, presents a **bottom-sheet participants panel** (`ZMUIToolkitParticipantsPanel`) listing all session participants with search; tapping it again dismisses the panel.
4. Provides host/manager moderation actions from the panel: an action sheet per participant (mute/unmute, make host, make co-host, rename, remove) and host-only bottom buttons ("Mute all", "Ask all to unmute").

The button is icon-only. Its visible "state" is its badge value and whether the panel is currently presented; it does not have an enumerated icon state machine like the audio button.

## Lifecycle

### Prerequisites
- The Zoom Video SDK must be initialized **and** registered with the toolkit via `ZMUIToolKitManager.initialize(videoSDK:bundleId:)` **before** creating the button. The button reads `ZMUIToolKitManager.videoSDK` during `init()` (via `refreshParticipantBadge()`) and on every delegate callback and panel action.
- If `ZMUIToolKitManager.videoSDK` is `nil` (toolkit not initialized or already cleaned up) or there is no active session, `refreshParticipantBadge()` hides the badge (`showBadge = false`, `badgeText = nil`). The tap still presents an (empty) panel, but `createParticipantsPanel`/`refreshList` produce no participants because `getSession()` is `nil`.

### Entry / Exit
- **Construction** (`init()`): builds the tap callback (toggle panel), builds a `ZMUIToolkitIconBtnDescriptor` (40×40, clear background, `secondaryLabel` icon color, zero icon padding, icon name `"Participants"`), calls `super.init(descriptor:)`, sets the normal-state image, configures the badge style (`showBadgeBackground = false`, `badgeTextColor = .secondaryLabel`), **adds itself** to `ZMUIToolKitManager.videoSDKDelegate`, then calls `refreshParticipantBadge()`.
- **Destruction** (`deinit`): **removes itself** from `ZMUIToolKitManager.videoSDKDelegate`. Delegates are held weakly by `CompositeZoomVideoSDKDelegate`.

> Note: The badge refreshes at construction and on the listed delegate events (session join/leave, user join/leave).

### Delegate Registration Model
The button does **not** assign itself to `ZoomVideoSDK.delegate`. Instead it registers with the toolkit's fan-out delegate (`ZMUIToolKitManager.videoSDKDelegate`, a `CompositeZoomVideoSDKDelegate`). This allows multiple toolkit components to observe SDK events simultaneously. Child delegates are stored in a weak hash table. The presented `ZMUIToolkitParticipantsPanel` independently registers/unregisters itself with the same fan-out delegate while visible (`observeSDKEvents = true`).

## State Machine

This button has no enumerated icon-state machine. Its observable runtime state is:

| Aspect | Source | Values |
|--------|--------|--------|
| Badge | `refreshParticipantBadge()` | hidden (no session / count == 0) or visible with text = participant count |
| Panel | `BottomPanelPresenter.isPresented` | `false` (dismissed) or `true` (presented) |

### Badge computation rule (`refreshParticipantBadge`)
```
session = ZMUIToolKitManager.videoSDK?.getSession()
if session == nil               -> showBadge = false; badgeText = nil
remoteCount = session.getRemoteUsers()?.count ?? 0
selfCount   = session.getMySelf() == nil ? 0 : 1
participantCount = remoteCount + selfCount
if participantCount > 0         -> badgeText = String(participantCount); showBadge = true
else                            -> badgeText = nil; showBadge = false
```

### Panel toggle rule (tap callback)
```
if presenter.isPresented  -> presenter.dismiss()
else                      -> presenter.present { createParticipantsPanel() }
```

### Badge transition drivers

| Trigger | Action | Result |
|---------|--------|--------|
| `init()` | `refreshParticipantBadge()` | initial count |
| `onSessionJoin` | `refreshParticipantBadge()` | updated count |
| `onSessionLeave(_:)` | `refreshParticipantBadge()` | updated count (typically hidden) |
| `onUserJoin(_:users:)` | `refreshParticipantBadge()` | updated count |
| `onUserLeave(_:users:)` | `refreshParticipantBadge()` | updated count |

The badge updates on session join/leave and user join/leave.

## APIs

### `init()`
```swift
@objc public init()
```
Designated public initializer. Builds the panel-toggle tap callback, constructs the underlying icon button (40×40, clear background, `secondaryLabel` icon color, zero icon padding, `"Participants"` icon), configures a borderless/background-less badge with `secondaryLabel` text color, registers as a delegate observer, and computes the initial badge count.

- **Preconditions**: Toolkit initialized via `ZMUIToolKitManager.initialize` (recommended). Must be called on the main thread (`@MainActor`).
- **Side effects**: Adds `self` to `ZMUIToolKitManager.videoSDKDelegate`; sets `showBadgeBackground = false`, `badgeTextColor = .secondaryLabel`; calls `refreshParticipantBadge()`.
- **Returns**: A configured button instance.

### `init?(coder:)`
```swift
@MainActor required public init?(coder: NSCoder)
```
- **Not supported.** Calls `fatalError`. Do **not** instantiate this button from a Storyboard/XIB.

### Tap behavior (inherited click dispatch)
The button has no public toggle method; tapping invokes the descriptor `callback` through `ZMUIToolkitIconBtn.onButtonClicked()`. The callback toggles the participants panel: if presented it dismisses; otherwise it presents a freshly created panel. Presentation/dismissal is dispatched to the main queue by `BottomPanelPresenter` and the panel is shown modally over `UIApplication.topViewController()`.

### `refreshParticipantBadge()` *(private)*
Recomputes the participant count from the SDK session and updates `showBadge` / `badgeText`. Invoked from `init()`, `onSessionJoin()`, `onSessionLeave(_:)`, `onUserJoin(_:users:)`, and `onUserLeave(_:users:)`. Documented for completeness; not part of the public API surface.

### `createParticipantsPanel()` *(private)*
Builds a `ZMUIToolkitParticipantsPanel` configured with `cellStyle: .full`, `selectionMode: .tapAction`, `observeSDKEvents: true`, `showSearchBar: true`. Wires `onParticipantTapped` to present a per-participant action sheet. If the local user `isHost()`, attaches two host-only bottom buttons: "Mute all" (`audioHelper.muteAllAudio(false)` + "All muted" toast) and "Ask all to unmute" (`audioHelper.unmuteAllAudio()` + "Request sent" toast).

### Inherited badge configuration (from `ZMUIToolkitIconBtn`)
The base class exposes `@objc` badge properties this button uses and that integrators may further tune: `showBadge: Bool`, `badgeText: String?`, `showBadgeBackground: Bool`, `badgeTextColor: UIColor?`, `badgeBackgroundColor: UIColor?`, `badgeWidth: CGFloat`. Note that `badgeText`/`showBadge` are driven automatically by `refreshParticipantBadge()`; manual writes will be overwritten on the next roster/lifecycle callback.

### Inherited configuration (from `ZMUIToolkitIconBtn`)
Commonly-used setters are inherited and may be used to style the button: `setBackgroundColor(color:)`, `setRadius(radius:)`, `setSize(size:)`, `setPosition(x:y:)`, `setIconColor(color:)`, `setIcon(icon:)`, `setIconSize(_:)`, `setIconPadding(padding:)`, `setAccessibility(label:hint:)`.

## Callbacks (`ZoomVideoSDKDelegate`)

The button implements four delegate methods. All are invoked on the main thread via the toolkit's `CompositeZoomVideoSDKDelegate` fan-out. Every one calls `refreshParticipantBadge()`; none changes the panel presentation state.

### `onSessionJoin()`
```swift
public func onSessionJoin()
```
Fired when the local user joins the session. Refreshes the participant-count badge.
- **State change**: badge recomputed from SDK session.

### `onSessionLeave(_:)`
```swift
public func onSessionLeave(_ reason: ZoomVideoSDKSessionLeaveReason)
```
Fired when the local user leaves/ends the session. Refreshes the badge (typically hides it because `getSession()` is no longer valid or the count becomes 0).
- **State change**: badge recomputed; usually hidden.

### `onUserJoin(_:users:)`
```swift
public func onUserJoin(_ helper: ZoomVideoSDKUserHelper?, users: [ZoomVideoSDKUser]?)
```
Fired when one or more users join. Refreshes the participant-count badge once the SDK roster reflects the change.
- **State change**: badge recomputed.

### `onUserLeave(_:users:)`
```swift
public func onUserLeave(_ helper: ZoomVideoSDKUserHelper?, users: [ZoomVideoSDKUser]?)
```
Fired when one or more users leave. Refreshes the participant-count badge once the SDK roster reflects the change.
- **State change**: badge recomputed.

## Moderation actions and outcomes

Moderation actions are issued to the SDK from the panel:

| Action | SDK call |
|--------|----------|
| Mute one | `audioHelper.muteAudio(user)` |
| Ask to unmute one | `audioHelper.unmuteAudio(user)` |
| Make host | `userHelper.makeHost(user)` |
| Make co-host | `userHelper.makeManager(user)` |
| Remove | `userHelper.remove(user)` |
| Rename | `userHelper.changeName(newName, with: user)` |
| Mute all (host) | `audioHelper.muteAllAudio(false)`; shows "All muted" toast |
| Ask all to unmute (host) | `audioHelper.unmuteAllAudio()`; shows "Request sent" toast |

**Target resolution**: Each action first resolves the target `ZoomVideoSDKUser` via `findUser(for:)` (matches `getMySelf()` then `getRemoteUsers()` by `getID()`) and fetches the relevant helper from `ZMUIToolKitManager.videoSDK`. If either is unavailable, the action does not run. `commitRename` applies only when the new name is non-empty and changed.

**Observing outcomes**: To respond to the results of moderation actions, observe SDK events by registering your own `ZoomVideoSDKDelegate` via `ZMUIToolKitManager.videoSDKDelegate.add(_:)` and watching `onError(_:detail:)` and the relevant status callbacks (e.g. `onUserHostChanged`).

## Rules

- **REQUIRED**: Initialize the toolkit (`ZMUIToolKitManager.initialize`) before constructing the button for a correct badge count and functional panel/actions.
- **REQUIRED**: Construct and use the button on the main thread (`@MainActor`).
- **FORBIDDEN**: Instantiating via `init?(coder:)` / Interface Builder (traps with `fatalError`).
- **DO NOT** assign the button (or anything) directly to `ZoomVideoSDK.delegate`; the toolkit owns that slot. Use `ZMUIToolKitManager.videoSDKDelegate.add(_:)`.
- **AVOID** manually setting `badgeText` / `showBadge` to represent the count — they are driven by `refreshParticipantBadge()` and overwritten on the next roster/lifecycle callback. You may safely customize purely-visual badge properties (`badgeTextColor`, `badgeBackgroundColor`, `showBadgeBackground`, `badgeWidth`).
- **NOTE**: Host/manager actions are determined from the local user's role when the panel is opened (from `getMySelf()?.isHost()`/`isManager()`). To reflect a role change, re-open the panel to recompute available actions.

## Examples

### Example 1: Add the button to a toolbar (happy path)
```swift
// Assumes ZMUIToolKitManager.initialize(videoSDK:bundleId:) was already called.
let participantsButton = ZMUIToolkitParticipantsBtn()
participantsButton.setSize(size: CGSize(width: 44, height: 44))
toolbarStackView.addArrangedSubview(participantsButton)
// No further wiring needed: the button self-registers for roster updates,
// keeps its count badge in sync, and toggles the participants panel on tap.
```

### Example 2: Customize the badge appearance (visual only)
```swift
let participantsButton = ZMUIToolkitParticipantsBtn()
participantsButton.showBadgeBackground = true
participantsButton.badgeBackgroundColor = .systemRed
participantsButton.badgeTextColor = .white
// Do NOT set badgeText/showBadge manually — they reflect the live participant count.
```

### Example 3: Observe moderation/SDK results yourself
```swift
final class ParticipantsObserver: NSObject, ZoomVideoSDKDelegate {
    func onError(_ errorType: ZoomVideoSDKError, detail details: Int) {
        // Show retry UI, logging, etc.
    }
    func onUserHostChanged(_ helper: ZoomVideoSDKUserHelper?, users user: ZoomVideoSDKUser?) {
        // React to make-host / make-co-host results.
    }
}
let observer = ParticipantsObserver()
ZMUIToolKitManager.videoSDKDelegate.add(observer) // held weakly — retain `observer` yourself
```

### Example 4: Lifecycle / teardown
```swift
// The button removes itself from the delegate fan-out in deinit.
// Just release your reference (e.g. remove from view hierarchy) when done.
participantsButton.removeFromSuperview()
// participantsButton = nil  // deinit -> ZMUIToolKitManager.videoSDKDelegate.remove(self)
```

## Code Snippets (Reusable Patterns)

### Compute the participant count from the SDK (mirrors `refreshParticipantBadge`)
```swift
let session = ZMUIToolKitManager.videoSDK?.getSession()
let remoteCount = session?.getRemoteUsers()?.count ?? 0
let selfCount = session?.getMySelf() == nil ? 0 : 1
let participantCount = remoteCount + selfCount
// participantCount > 0 -> show badge with String(participantCount); else hide.
```

### Resolve a session user by participant id (mirrors `findUser(for:)`)
```swift
func findUser(byID id: Int32) -> ZoomVideoSDKUser? {
    guard let session = ZMUIToolKitManager.videoSDK?.getSession() else { return nil }
    if let me = session.getMySelf(), me.getID() == id { return me }
    return session.getRemoteUsers()?.first(where: { $0.getID() == id })
}
```

## Type Definitions

| Type | Kind | Notes |
|------|------|-------|
| `ZMUIToolkitIconBtn` | open class (superclass) | UIButton subclass providing icon/badge/layout config; exposes `@objc` badge properties (`showBadge`, `badgeText`, `showBadgeBackground`, `badgeTextColor`, `badgeBackgroundColor`, `badgeWidth`) |
| `ZMUIToolkitIconBtnDescriptor` | struct | Initializer configuration (frame, colors, icon name, padding, callback) |
| `ZMUIToolKitManager` | class | Holds the `ZoomVideoSDK` instance and `videoSDKDelegate` fan-out; also `getAudioHelper()`/`getUserHelper()` accessed via `videoSDK` |
| `CompositeZoomVideoSDKDelegate` | class | Weak fan-out delegate; register via `add(_:)` / `remove(_:)` |
| `BottomPanelPresenter` | internal final class | Presents/dismisses a bottom-sheet panel modally over the top view controller on the main queue; `isPresented` reflects current state |
| `ZMUIToolkitParticipantsPanel` | internal class | Bottom-sheet list of participants with search; self-registers as an SDK observer while visible; surfaces `onParticipantTapped` and `setButtons(_:)` |
| `ZMUIToolkitParticipantsPanelConfiguration` | struct | Panel config: `title`, `cellStyle` (`.full`/`.base`), `selectionMode` (`.none`/`.tapAction`/`.persistentSelection`), `observeSDKEvents`, `showSearchBar` |
| `ZMUIToolkitPanelButton` | struct | Bottom-button config: `title`, `style` (`.primary`/`.secondary`), `initiallyEnabled`, `action` |
| `ZMUIToolkitParticipant` | internal struct | Snapshot of a session user: `name`, `id: Int32`, `muted`, `sharing`, `camera`, `isHost`, `isManager`, `isMe` |
| `CustomActionSheet` | internal | Presents a titled action sheet of `CustomActionSheet.Action` items from the top view controller |
| `ZMToastManager` | class | Shows non-blocking toasts (`showAllMuted()`, `showRequestSent()`) |
| `ZoomVideoSDKSessionLeaveReason` | SDK enum | Reason passed to `onSessionLeave` |
| `ZoomVideoSDKUser` | SDK class | Local user via `getSession()?.getMySelf()`; remotes via `getRemoteUsers()`; provides `getID()`, `getName()`, `isHost()`, `isManager()`, `audioStatus()` |
| `ZoomVideoSDKAudioHelper` | SDK class | `muteAudio(_:)`, `unmuteAudio(_:)`, `muteAllAudio(_:)`, `unmuteAllAudio()` |
| `ZoomVideoSDKUserHelper` | SDK class | `makeHost(_:)`, `makeManager(_:)`, `remove(_:)`, `changeName(_:with:)` |

## Metadata
- **Source file**: `ZMUIToolkit/ZMUIToolkit/Sources/Buttons/ZMUIToolkitParticipantsBtn.swift`
- **Generated for**: AI-assisted code generation and integration
- **Verification**: Behaviors documented above were verified directly against the implementation source, not assumed.
