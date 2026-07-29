# ZMUIToolkitParticipantsBtn

## Description
`ZMUIToolkitParticipantsBtn` is a drop-in, self-managing participants control button for sessions built on the Zoom Video SDK. It displays a fixed `Participants` icon with a numeric badge showing the current participant count (remote users + the local user), and keeps that count in sync via SDK join/leave and session-lifecycle callbacks. Tapping it toggles a bottom-sheet participants panel (`ZMUIToolkitParticipantsPanel`) with search. From the panel, host/manager users get per-participant moderation actions (mute/unmute, make host, make co-host, rename, remove) and host-only bottom buttons ("Mute all", "Ask all to unmute").

## How to use
Prerequisite: initialize the toolkit first (see `references/setup.md`). The button reads `ZMUIToolKitManager.videoSDK` at `init()` and on every callback/action; without it the badge hides and the panel is empty.

```swift
// Assumes ZMUIToolKitManager.initialize(videoSDK:bundleId:) was already called.
let participantsButton = ZMUIToolkitParticipantsBtn()
participantsButton.setSize(size: CGSize(width: 44, height: 44))
toolbarStackView.addArrangedSubview(participantsButton)
// No further wiring: it self-registers for roster updates, keeps the badge in
// sync, and toggles the participants panel on tap (tap again to dismiss).
```

Key points:
- **Tap behavior**: no public toggle method. Tapping presents a freshly built panel if none is shown, otherwise dismisses it.
- **Badge is automatic**: driven by `refreshParticipantBadge()` from `init()` and the four delegate callbacks. The badge hides when there is no session or count is 0; otherwise shows `String(count)`.
- **Customize visual badge only** (these survive refreshes): `showBadgeBackground`, `badgeBackgroundColor`, `badgeTextColor`, `badgeWidth`. Do NOT set `badgeText`/`showBadge` manually — they are overwritten on the next roster/lifecycle callback.
- **Inherited styling** (from `ZMUIToolkitIconBtn`): `setBackgroundColor`, `setRadius`, `setSize`, `setPosition`, `setIconColor`, `setIcon`, `setIconSize`, `setIconPadding`, `setAccessibility(label:hint:)`.
- **Observe results yourself**: to respond to moderation/SDK outcomes, register your own delegate via `ZMUIToolKitManager.videoSDKDelegate.add(_:)` (held weakly — retain it) and watch `onError(_:detail:)` etc.

## Limitations & restrictions
- **Moderation actions**: mute/unmute, make host, make co-host, rename, remove are issued to the SDK. Host bottom buttons show "All muted" / "Request sent" toasts. To respond to outcomes, observe SDK events via `ZMUIToolKitManager.videoSDKDelegate.add(_:)`.
- **Target resolution**: each action resolves the target user via `findUser(for:)` and the relevant helper; the action runs only when both are available. Rename applies only when the new name is non-empty and changed.
- **Role-based actions**: host/manager actions are determined from the local user's role when the panel is opened (from `getMySelf()?.isHost()`/`isManager()`). To reflect a role change, re-open the panel to recompute.
- **Badge refresh**: the badge refreshes at construction and on the listed delegate events (session join/leave, user join/leave).
- **`@MainActor`**: construct and use on the main thread.
- **`init?(coder:)` not supported**: traps with `fatalError` — do not instantiate from Storyboard/XIB.
- Do NOT assign anything directly to `ZoomVideoSDK.delegate`; the toolkit owns that slot. Use `ZMUIToolKitManager.videoSDKDelegate.add(_:)`.

## Related components
- **references/setup.md** — required first; provides `ZMUIToolKitManager.initialize` and the SDK the badge/panel depend on.
- **references/icon-btn.md** — the superclass; defines the badge API (`showBadge`, `badgeText`, `showBadgeBackground`, `badgeTextColor`, `badgeBackgroundColor`, `badgeWidth`) and the inherited styling setters used here.
- **references/leave-session-btn.md** — also drives a participants panel (for assign-host on leave), so panel behavior overlaps.
- **references/manager.md** — owns `videoSDK`, the `videoSDKDelegate` fan-out, and the audio/user helpers the moderation actions call.
- **references/composite-delegate.md** — the weak fan-out (`add`/`remove`) this button registers with; use it to observe SDK results.
- **references/audio-btn.md**, **references/video-btn.md**, **references/share-btn.md**, **references/pause-share-btn.md**, **references/more-btn.md** — sibling toolbar controls commonly placed alongside it.
- **references/strings.md** — localized strings for the panel/action-sheet/toast text.

## Full API reference
- Markdown: `docs/api/ZMUIToolkitParticipantsBtn-API.md`
- JSON: `docs/api/ZMUIToolkitParticipantsBtn-API.json`
