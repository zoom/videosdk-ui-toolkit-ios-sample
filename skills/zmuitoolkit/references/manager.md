# ZMUIToolKitManager

## Description
`ZMUIToolKitManager` is the central, all-static `@objc` (`NSObject`) manager that connects the ZMUIToolkit framework to an already-initialized `ZoomVideoSDK` instance. It holds the SDK reference (`videoSDK`), installs and owns a single fan-out delegate (`videoSDKDelegate`, a `CompositeZoomVideoSDKDelegate`) onto `ZoomVideoSDK.delegate`, and exposes config flags such as `cloudRecordingConsentDialogEnabled`. It is the entry point every other self-managing toolkit component depends on — components read `ZMUIToolKitManager.videoSDK` and register observers on its fan-out delegate.

## How to use
Initialize on the main thread with an already-initialized SDK, check the `Bool`, build components, then clean up before tearing the SDK down.

```swift
// On the main thread, AFTER ZoomVideoSDK is initialized & ideally before/around session join.
let ok = ZMUIToolKitManager.initialize(
    videoSDK: sdk,
    bundleId: Bundle.main.bundleIdentifier   // broadcast-extension id for screen share; nil -> ""
)
guard ok else { return }   // false means the toolkit could not attach to the SDK; do NOT build toolkit UI

// Read the connected SDK
let mySelf = ZMUIToolKitManager.videoSDK?.getSession()?.getMySelf()

// Observe SDK events via the fan-out delegate (never reassign ZoomVideoSDK.delegate)
let observer = MyObserver()                          // NSObject & ZoomVideoSDKDelegate
ZMUIToolKitManager.videoSDKDelegate.add(observer)    // held WEAKLY — retain it yourself

// Optional config
ZMUIToolKitManager.cloudRecordingConsentDialogEnabled = false  // auto-accept + toast

// Teardown before the host destroys the SDK
ZMUIToolKitManager.cleanup()
```

Ordering: `ZoomVideoSDK.initialize` (host) → `ZMUIToolKitManager.initialize(...)` → construct components → ... session work ... → `ZMUIToolKitManager.cleanup()` → host SDK teardown. For the full end-to-end sequence (SDK init, join, and screen-share `bundleId` / broadcast-extension setup), see `references/setup.md`.

## Limitations & restrictions
- `initialize(videoSDK:bundleId:)` and `cleanup()` are `@MainActor` — call them on the main thread.
- `initialize` returns `false` and rolls back (`videoSDK = nil`, `bundleId = ""`) when the toolkit cannot attach to the provided SDK; the delegate is NOT installed. Treat the `Bool` return as authoritative instead of using `getVersion()` as your app-level SDK-initialization check. Always check the return value before building toolkit UI.
- The manager takes ownership of `ZoomVideoSDK.delegate` on success (sets it to `videoSDKDelegate`). FORBIDDEN to reassign `ZoomVideoSDK.delegate` afterward — it breaks fan-out and defeats cleanup's ownership check. Add observers via `videoSDKDelegate.add(_:)` instead.
- `cleanup()` clears the SDK delegate slot only if the toolkit still owns it (`sdk.delegate === videoSDKDelegate`); it also stops Picture-in-Picture (`ZMUIToolkitPiPController.shared.stop()`) and is idempotent.
- Registered observers are held weakly and are NOT cleared by `cleanup()` — they survive across cleanup/re-`initialize()`. Retain your own observers; remove them explicitly if you need them gone.
- Access manager properties (`videoSDK`, `cloudRecordingConsentDialogEnabled`, `bundleId`) from the main thread, consistent with the toolkit lifecycle APIs.
- `bundleId` is the broadcast-extension identifier used by the screen-share / PiP code paths; `nil` is normalized to `""`.

## Related components
- **references/setup.md** — the end-to-end integration guide (SDK init order, join, screen-share/broadcast-extension setup); start here.
- **references/composite-delegate.md** — the `CompositeZoomVideoSDKDelegate` fan-out that this manager creates and owns as `videoSDKDelegate`.
- **references/icon-btn.md** — base icon button that all the control buttons extend.
- **references/audio-btn.md**, **references/video-btn.md**, **references/share-btn.md**, **references/pause-share-btn.md**, **references/participants-btn.md**, **references/more-btn.md**, **references/leave-session-btn.md** — self-managing control buttons; each reads `ZMUIToolKitManager.videoSDK` and registers on `videoSDKDelegate`, so they require this to be initialized first.
- **references/root-video-view.md** — root video container; depends on the manager being initialized.
- **references/strings.md** — localized strings used across the toolkit UI.

## Full API reference
- `docs/api/ZMUIToolKitManager-API.md`
- `docs/api/ZMUIToolKitManager-API.json`
