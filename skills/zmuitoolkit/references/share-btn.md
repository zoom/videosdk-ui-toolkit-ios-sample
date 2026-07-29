# ZMUIToolkitShareBtn

## Description
`ZMUIToolkitShareBtn` is a drop-in, icon-only screen-share control for sessions built on the Zoom Video SDK. On tap it drives **in-app / ReplayKit screen sharing**: when idle it presents Apple's `RPSystemBroadcastPickerView` (the ReplayKit system broadcast picker) to begin a broadcast, and when already sharing it calls the SDK's `stopShare()`. It is self-managing — it registers as a `ZoomVideoSDKDelegate` observer and keeps its icon and enabled state in sync with the authoritative SDK share state via `onUserShareStatusChanged`. The button subclasses `ZMUIToolkitIconBtn` and is `@MainActor`.

## How to use
Minimal example (assumes the toolkit is already initialized — see prerequisite below):

```swift
let shareButton = ZMUIToolkitShareBtn()
shareButton.setSize(size: CGSize(width: 44, height: 44))
toolbarStackView.addArrangedSubview(shareButton)
// Optional: custom per-state icons
shareButton.setStartShareIcon(image: myStartImage)
shareButton.setStopShareIcon(image: myStopImage)
```

No tap wiring is needed: the button self-registers for share-status updates, presents the broadcast picker when idle, and stops sharing when active.

**CRITICAL prerequisite — full screen-share project setup is required.** Screen sharing only works if the host app has:
1. An **App Group** shared between the app and a **Broadcast Upload Extension**.
2. A configured **ReplayKit Broadcast Upload Extension** target.
3. The extension's **bundle identifier passed as `bundleId`** to `ZMUIToolKitManager.initialize(videoSDK:bundleId:)`, so the button can set `RPSystemBroadcastPickerView.preferredExtension`.

See `references/setup.md` (Steps 5 & 6) for this configuration. Add this button only when screen-share setup is complete (Steps 5 & 6 of `references/setup.md`); otherwise it has nothing to broadcast to.

Key methods: `init()`, `setStartShareIcon(image:)`, `setStopShareIcon(image:)`, plus inherited `ZMUIToolkitIconBtn` styling setters (`setSize`, `setIconColor`, `setAccessibility(label:hint:)`, etc.).

## Limitations & restrictions
- **Requires a Broadcast Upload Extension + App Group + `bundleId`** wired through `ZMUIToolKitManager.initialize`. Without `bundleId`, the picker does not preselect your extension and the user must choose manually. Add this button only when screen-share setup is complete (Steps 5 & 6 of references/setup.md); otherwise it has nothing to broadcast to.
- **Tapping `start` updates the icon after the broadcast begins:** the button reflects the authoritative share state reported by the SDK; its icon updates to the sharing state once a broadcast begins (via `onUserShareStatusChanged`). To respond to outcomes, observe SDK events via `ZMUIToolKitManager.videoSDKDelegate.add(_:)`.
- **No `didMoveToWindow` resync:** this button does not re-sync state on attachment. Initial state is set at construction and thereafter updated by `onUserShareStatusChanged`.
- **Self-disables** via `canCurrentUserStartScreenShare()` (state-based enablement: in-app-share support, host share lock, multi-share setting, another user sharing). Returns `false` when the share helper is `nil`.
- **`@MainActor`:** construct and use only on the main thread.
- **`init?(coder:)` traps** with `fatalError` — never instantiate from Storyboard/XIB.
- **Do not** assign the button directly to `ZoomVideoSDK.delegate`, and **do not** add another view with tag `10001` (`kBroadcastPickerTag`) to its window — the button uses that tag to dismiss the picker.

## Related components
- **references/setup.md** — REQUIRED prerequisite; Steps 5 & 6 cover App Group, broadcast extension, and `bundleId` setup without which this button can't share.
- **references/pause-share-btn.md** — closely related; pauses/resumes an *active* share that this button starts/stops.
- **references/manager.md** — `ZMUIToolKitManager` holds the SDK, `bundleId`, and the `videoSDKDelegate` fan-out the button registers with.
- **references/composite-delegate.md** — `CompositeZoomVideoSDKDelegate`; register your own `ZoomVideoSDKDelegate` here to observe SDK events (e.g. `onFailed(toStartShare:)`).
- **references/icon-btn.md** — superclass providing the styling setters used above.
- **references/audio-btn.md**, **references/video-btn.md**, **references/participants-btn.md**, **references/more-btn.md**, **references/leave-session-btn.md** — sibling toolbar buttons typically arranged alongside this one.
- **references/root-video-view.md** — the session video surface these controls overlay.
- **references/strings.md** — localized accessibility/label strings.

## Full API reference
- `docs/api/ZMUIToolkitShareBtn-API.md`
- `docs/api/ZMUIToolkitShareBtn-API.json`
