# CompositeZoomVideoSDKDelegate

## Description
`CompositeZoomVideoSDKDelegate` is a fan-out (multicast) `ZoomVideoSDKDelegate`. The Zoom Video SDK exposes only a single `delegate` slot, so this class occupies it and re-broadcasts every callback to a set of weakly-held child delegates, letting many components observe SDK events at once. It is exposed publicly as `ZMUIToolKitManager.videoSDKDelegate` (a process-lifetime `static let`). The toolkit installs the shared instance onto `ZoomVideoSDK.delegate` during `ZMUIToolKitManager.initialize(videoSDK:bundleId:)`; before that runs, registered children receive nothing.

## How to use
Register your own observer (any object conforming to `ZoomVideoSDKDelegate`, implementing only the optional methods you care about) on the shared instance:

```swift
let observer = MySessionObserver()          // NSObject + ZoomVideoSDKDelegate
self.observer = observer                    // keep a STRONG reference — held weakly below
ZMUIToolKitManager.videoSDKDelegate.add(observer)
// later:
ZMUIToolKitManager.videoSDKDelegate.remove(observer)
```

Objective-C uses the `@objc` selectors `addDelegate:` / `removeDelegate:`:

```objc
[ZMUIToolKitManager.videoSDKDelegate addDelegate:observer];   // retain observer yourself
[ZMUIToolKitManager.videoSDKDelegate removeDelegate:observer];
```

- NEVER assign anything directly to `ZoomVideoSDK.delegate` — the toolkit owns that slot, and reassigning it silently disconnects every toolkit component. Register here instead.
- Children are held WEAKLY: you must retain your observer for as long as you want callbacks. A self-registering UIView can safely `add(self)` in `init` (no retain cycle) because storage is weak.
- Ensure `ZMUIToolKitManager.initialize(videoSDK:bundleId:)` has run (see `references/setup.md` / `references/manager.md`), or children get no events.

## Limitations & restrictions
- Children are held WEAKLY (`NSHashTable.weakObjects()`). Drop your only strong reference and the observer is deallocated and silently removed — callbacks just stop, with no error.
- Register/remove observers on the main thread. If your observer updates UIKit, dispatch to the main thread if needed.
- Fan-out order is undefined/unstable (`allObjects` iteration order).
- Keep handlers fast and non-throwing so callback delivery remains responsive.
- `ZMUIToolKitManager.cleanup()` keeps registered children in the weak table, so observers survive across SDK re-initialization.
- Duplicate `add` of the same instance is deduplicated (pointer identity); unknown `remove` is a harmless no-op. Neither throws.
- Public API is just `init()`, `add(_:)`, `remove(_:)`. The dozens of `ZoomVideoSDKDelegate` methods on the class are conformance plumbing you do not call.

## Related components
- `references/manager.md` — owns and exposes this as `ZMUIToolKitManager.videoSDKDelegate` and installs it onto `ZoomVideoSDK.delegate`.
- `references/setup.md` — covers SDK + toolkit initialization order that wires the composite to the SDK.
- `references/audio-btn.md`, `references/video-btn.md`, `references/share-btn.md`, `references/pause-share-btn.md` — delegate-conforming buttons that register themselves here to react to SDK state.
- `references/root-video-view.md` — registers with the composite to observe video/user events.

## Full API reference
- `docs/api/CompositeZoomVideoSDKDelegate-API.md`
- `docs/api/CompositeZoomVideoSDKDelegate-API.json`
