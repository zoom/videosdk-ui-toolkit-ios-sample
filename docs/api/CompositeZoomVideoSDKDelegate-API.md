# CompositeZoomVideoSDKDelegate API Documentation

## Module Information
- **Module**: CompositeZoomVideoSDKDelegate
- **Platform**: iOS
- **Language**: Swift (with Objective-C interop via `@objc`)
- **Version**: 1.0
- **Framework**: ZMUIToolkit
- **Objective-C name**: `ZMUICompositeZoomVideoSDKDelegate` (via `@objc(ZMUICompositeZoomVideoSDKDelegate)`)
- **Superclass**: `NSObject`
- **Conforms to**: `ZoomVideoSDKDelegate`
- **Declaration**: `public final class` (cannot be subclassed)

## Overview

`CompositeZoomVideoSDKDelegate` is a fan-out (multicast) implementation of `ZoomVideoSDKDelegate`. The Zoom Video SDK exposes a single `delegate` slot, so only one object can receive its callbacks. This class occupies that single slot and re-broadcasts every callback it receives to a set of **weakly-held child delegates**, allowing many toolkit components (and integrator-supplied observers) to listen to SDK events simultaneously.

Key characteristics:

1. **Single registration target.** The toolkit assigns one instance to `ZoomVideoSDK.delegate` (done by `ZMUIToolKitManager.initialize`). It is exposed publicly as `ZMUIToolKitManager.videoSDKDelegate`.
2. **Weak child storage.** Children are stored in an `NSHashTable.weakObjects()`. The composite does **not** retain them; a child that is deallocated is automatically dropped from the table.
3. **Optional-callback forwarding.** Every `ZoomVideoSDKDelegate` method is optional. The composite implements all of them and forwards each using optional chaining (`$0.onSomething?(...)`), so a child only receives the callbacks it actually implements.
4. **Synchronous fan-out.** When a callback fires, the composite iterates `delegates.allObjects` and invokes each registered child. No state is tracked and no value is returned to the SDK from the fan-out.

The public API surface is intentionally tiny: `init()`, `add(_:)`, and `remove(_:)`. The dozens of `ZoomVideoSDKDelegate` methods on the class are conformance plumbing, not API you call directly.

## Lifecycle

### Prerequisites
- A single instance is created and owned by the toolkit as `ZMUIToolKitManager.videoSDKDelegate` (a `static let`). Integrators normally do **not** construct their own instance.
- The instance becomes "live" (begins receiving SDK callbacks) only after `ZMUIToolKitManager.initialize(videoSDK:bundleId:)` runs, because that method assigns `videoSDK.delegate = videoSDKDelegate`. Before initialization, registered children receive nothing because the SDK has no delegate wired to the composite.

### Entry / Exit
- **Installation** (`ZMUIToolKitManager.initialize`): sets `_videoSDK = videoSDK` and `_videoSDK?.delegate = videoSDKDelegate`. From this point, SDK callbacks fan out to all registered children.
- **Teardown** (`ZMUIToolKitManager.cleanup`): clears `ZoomVideoSDK.delegate` if the toolkit still owns the slot (`sdk.delegate === videoSDKDelegate`), then releases the SDK reference. **Registered child delegates are kept** in the composite's weak table so listeners persist across SDK re-initialization. The composite itself is a `static let` and lives for the process lifetime.

### Registration Model (how integrators register)
Integrators and toolkit components register an observer with:

```swift
ZMUIToolKitManager.videoSDKDelegate.add(myObserver)   // Objective-C: [ZMUIToolKitManager.videoSDKDelegate addDelegate:myObserver];
```

and unregister with:

```swift
ZMUIToolKitManager.videoSDKDelegate.remove(myObserver) // Objective-C: [ZMUIToolKitManager.videoSDKDelegate removeDelegate:myObserver];
```

Always register here rather than assigning anything directly to `ZoomVideoSDK.delegate` — the toolkit owns that slot, and registering through the composite keeps every toolkit component connected. Use `add(_:)` / `remove(_:)` instead.

## Memory Semantics (weak-reference lifecycle)

This describes the memory contract of the class.

- **Children are held weakly.** Storage is `private let delegates = NSHashTable<AnyObject>.weakObjects()`. Each child is held by a weak (zeroing) reference.
- **The caller owns the child's lifetime.** Retain your observer for as long as you want it to receive callbacks. If the only strong reference is dropped, the object is deallocated and removed from the table, and its callbacks stop.
- **No retain cycles.** Because storage is weak, registering with the composite does not create a retain cycle between the composite and its children. This is why toolkit views (e.g. buttons) can `add(self)` in `init` without leaking.
- **`remove` in `deinit` is optional.** A deallocated child is automatically removed from the weak table. Calling `remove(_:)` in `deinit` provides deterministic cleanup but is not required.
- **Duplicate adds are deduplicated by the hash table.** `NSHashTable.weakObjects()` uses object pointer identity, so adding the same instance twice results in a single entry and a single delivery.

## Threading

- Callbacks are delivered to registered observers on the thread the SDK invokes. Each callback method calls `forward { ... }`, which iterates and invokes children on that thread.
- In practice the Zoom Video SDK delivers its `ZoomVideoSDKDelegate` callbacks on the main thread, so children typically receive them on the main thread.
- **Guidance for children:** Register and remove observers on the main thread, and dispatch to the main thread inside your observer if you update UI. Keep callback handlers fast.

## APIs

### `init()`
```swift
public override init()
```
Default initializer. Calls `super.init()` and creates an empty weak child table. Integrators normally do not call this — they use the shared `ZMUIToolKitManager.videoSDKDelegate` instance instead.
- **Preconditions**: none.
- **Side effects**: none beyond constructing an empty table.
- **Returns**: an empty composite with no registered children.

### `add(_:)`
```swift
@objc(addDelegate:)
public func add(_ delegate: ZoomVideoSDKDelegate)
```
Registers a child delegate to receive forwarded SDK callbacks. The child is stored **weakly**.
- **Objective-C selector**: `addDelegate:`
- **Parameters**: `delegate` — an object conforming to `ZoomVideoSDKDelegate`. Implement only the optional methods you care about.
- **Side effects**: adds `delegate` to the weak hash table. Adding the same object more than once results in a single entry (pointer-identity dedup).
- **Memory**: the composite holds `delegate` weakly. The caller must keep a strong reference for as long as callbacks are needed.
- **Threading**: call from the main thread (consistent with how `ZMUIToolKitManager` is used).

### `remove(_:)`
```swift
@objc(removeDelegate:)
public func remove(_ delegate: ZoomVideoSDKDelegate)
```
Unregisters a previously-added child delegate so it stops receiving forwarded callbacks.
- **Objective-C selector**: `removeDelegate:`
- **Parameters**: `delegate` — the object previously passed to `add(_:)`.
- **Side effects**: removes `delegate` from the weak hash table. Removing an object that was never added (or was already removed/deallocated) has no effect.
- **Threading**: call from the main thread.

### `forward(_:)` *(private)*
```swift
private func forward(_ block: (ZoomVideoSDKDelegate) -> Void)
```
Internal fan-out helper. Iterates `delegates.allObjects`, casts each entry to `ZoomVideoSDKDelegate`, and invokes `block` for it. Every public `ZoomVideoSDKDelegate` method on the composite delegates to this. Documented for completeness; not part of the public API surface.

## Forwarded Callbacks (`ZoomVideoSDKDelegate` conformance)

The composite implements and forwards the full set of `ZoomVideoSDKDelegate` callbacks. Each is forwarded with optional chaining, so only children that implement the optional method receive it. None of these are APIs you call — they are events the composite relays. They are grouped by category below.

> Universal forwarding contract for **every** callback in this section:
> - **Semantic**: passthrough relay (no transformation of arguments).
> - **State change**: none — the composite is stateless.
> - **Thread**: delivered on the thread the SDK invokes (typically main).
> - **Delivery**: synchronous, to each registered child that implements the method.

### Session lifecycle
- `onSessionJoin()`
- `onSessionLeave(_ reason: ZoomVideoSDKSessionLeaveReason)`
- `onError(_ ErrorType: ZoomVideoSDKError, detail details: Int)`
- `onSessionNeedPassword(_ completion: ((String?, Bool) -> ZoomVideoSDKError)?)`
- `onSessionPasswordWrong(_ completion: ((String?, Bool) -> ZoomVideoSDKError)?)`

### User changes
- `onUserJoin(_:users:)`
- `onUserLeave(_:users:)`
- `onUserVideoStatusChanged(_:user:)`
- `onUserAudioStatusChanged(_:user:)`
- `onUserActiveAudioChanged(_:users:)`
- `onUserHostChanged(_:users:)`
- `onUserManagerChanged(_:)`
- `onUserNameChanged(_:)`

### Share
- `onUserShareStatusChanged(_:user:shareAction:)`
- `onFailed(toStartShare:user:)`
- `onShareSettingChanged(_:)`
- `onShareContentChanged(_:user:shareAction:)`
- `onShareCaptureSizeChanged(_:user:shareAction:)`
- `onShareContentSizeChanged(_:user:shareAction:)`
- `onShareCanvasSubscribeFail(with:view:shareAction:)`

### Live stream / streaming
- `onLiveStreamStatusChanged(_:status:)`
- `onBindIncomingLiveStreamResponse(_:success:streamKeyID:)`
- `onUnbindIncomingLiveStreamResponse(_:success:streamKeyID:)`
- `onIncomingLiveStreamStatusResponse(_:success:streamsStatusList:)`
- `onStartIncomingLiveStreamResponse(_:success:streamKeyID:)`
- `onStopIncomingLiveStreamResponse(_:success:streamKeyID:)`
- `onStartBroadcastResponse(_:channelID:)`
- `onStopBroadcastResponse(_:)`
- `onGetBroadcastControlStatus(_:status:)`
- `onStreamingJoinStatusChanged(_:)`

### Chat
- `onChatPrivilegeChanged(_:privilege:)`
- `onChatNewMessageNotify(_:message:)`
- `onChatMsgDeleteNotification(_:messageID:deleteBy:)`

### Audio raw data
- `onMixedAudioRawDataReceived(_:)`
- `onOneWayAudioRawDataReceived(_:user:)`
- `onSharedAudioRawDataReceived(_:)`
- `onAudioLevelChanged(_:audioSharing:user:)`
- `onMyAudioSourceTypeChanged(_:)`

### Phone / call out
- `onInvite(by:fail:)`
- `onCalloutJoinSuccess(_:phoneNumber:)`
- `onCallCRCDeviceStatusChanged(_:)`

### Command channel
- `onCmdChannelConnectResult(_:)`
- `onCommandReceived(_:send:)`

### Camera control
- `onCameraControlRequestResult(_:approved:)`

### Recording
- `onCloudRecordingStatus(_:recordAgreementHandler:)`
- `onUserRecordAgreementNotification(_:)`

### Mute / mic test
- `onHostAskUnmute()`
- `onTestMicStatusChanged(_:)`
- `onMicSpeakerVolumeChanged(_:speakerVolume:)`

### Multi-camera
- `onMultiCameraStreamStatusChanged(_:parentUser:videoPipe:)`
- `onMultiCameraStreamStatusChanged(_:parentUser:videoCanvas:)`

### System / permissions
- `onSystemPermissionRequired(_:)`
- `onProxySettingNotification(_:)`
- `onSSLCertVerifiedFailNotification(_:)`

### Live transcription
- `onLiveTranscriptionStatus(_:)`
- `onLiveTranscriptionMsgReceived(_:)`
- `onOriginalLanguageMsgReceived(_:)`
- `onLiveTranscriptionMsgError(_:transLanguage:)`
- `onSpokenLanguageChanged(_:)`

### Network status
- `onUserNetworkStatusChanged(_:level:user:)`
- `onUserOverallNetworkStatusChanged(_:user:)`
- `onUserVideoNetworkStatusChanged(_:user:)`

### Annotation
- `onAnnotationHelperCleanUp(_:)`
- `onAnnotationPrivilegeChange(with:shareAction:)`
- `onAnnotationToolTypeChanged(_:view:toolType:)`

### Subscribe failures
- `onVideoCanvasSubscribeFail(_:user:view:)`

### File transfer
- `onSendFileStatus(_:status:)`
- `onReceiveFileStatus(_:status:)`

### Video alpha / spotlight
- `onVideoAlphaChannelStatusChanged(_:)`
- `onSpotlightVideoChanged(_:userList:)`

### External / UVC camera
- `onUVCCameraStatusChange(_:)`

### Sub-session
- `onSubSessionStatusChanged(_:subSession:)`
- `onSubSessionManagerHandle(_:)`
- `onSubSessionParticipantHandle(_:)`
- `onSubSessionUsersUpdate(_:)`
- `onBroadcastMessage(fromMainSession:userName:)`
- `onSubSessionUserHelpRequestHandler(_:)`
- `onSubSessionUserHelpRequestResult(_:)`

### In-app screen share
- `on(inAppScreenShare:)`
- `on(inAppScreenShareAvailableChanged:)`

### Whiteboard / canvas
- `onUserWhiteboardShareStatusChanged(_:whiteboardhelper:)`
- `onCanvasSnapshotTaken(_:isShare:)`
- `onCanvasSnapshotIncompatible(_:)`

### Real-time media streams
- `onRealTimeMediaStreamsStatus(_:)`
- `onRealTimeMediaStreamsFail(_:)`

## Error Handling

`CompositeZoomVideoSDKDelegate` acts as a relay for SDK callbacks:

- `add(_:)` and `remove(_:)` never fail and never throw. Duplicate adds and unknown removes have no effect.
- Error-bearing SDK callbacks (e.g. `onError(_:detail:)`, `onFailed(toStartShare:user:)`, subscribe-fail callbacks) are forwarded to children that implement them.
- Because fan-out is synchronous, keep callback handlers fast.

## Rules

- **REQUIRED**: Register observers via `ZMUIToolKitManager.videoSDKDelegate.add(_:)` (Obj-C `addDelegate:`). Unregister with `remove(_:)` (Obj-C `removeDelegate:`).
- **REQUIRED**: Keep a strong reference to any observer you register — the composite holds children weakly.
- **REQUIRED**: Call `ZMUIToolKitManager.initialize(videoSDK:bundleId:)` so the composite is wired to `ZoomVideoSDK.delegate`; otherwise registered children receive nothing.
- **REQUIRED**: Register and remove observers on the main thread, and dispatch to the main thread inside your observer if you update UI.
- **RECOMMENDED**: Keep forwarded callback handlers fast, since fan-out is synchronous.
- Always register through the composite rather than assigning directly to `ZoomVideoSDK.delegate`. The toolkit owns that slot; registering here keeps every toolkit component connected.

## Examples

### Example 1: Register a custom observer (happy path)
```swift
final class MySessionObserver: NSObject, ZoomVideoSDKDelegate {
    func onSessionJoin() { /* ... */ }
    func onSessionLeave(_ reason: ZoomVideoSDKSessionLeaveReason) { /* ... */ }
}

// Retain the observer yourself — the composite holds it weakly.
let observer = MySessionObserver()
self.sessionObserver = observer
ZMUIToolKitManager.videoSDKDelegate.add(observer)
```

### Example 2: Observe SDK errors
```swift
final class ErrorObserver: NSObject, ZoomVideoSDKDelegate {
    func onError(_ errorType: ZoomVideoSDKError, detail details: Int) {
        // Surface retry UI, log, etc.
    }
}
let errorObserver = ErrorObserver()        // keep a strong reference
ZMUIToolKitManager.videoSDKDelegate.add(errorObserver)
```

### Example 3: A self-registering UIKit component (weak storage avoids cycles)
```swift
final class MyAudioIndicator: UIView, ZoomVideoSDKDelegate {
    override init(frame: CGRect) {
        super.init(frame: frame)
        // Safe: composite holds `self` weakly, so no retain cycle.
        ZMUIToolKitManager.videoSDKDelegate.add(self)
    }
    deinit {
        // Optional: the weak table removes us automatically.
        ZMUIToolKitManager.videoSDKDelegate.remove(self)
    }
    func onUserAudioStatusChanged(_ helper: ZoomVideoSDKAudioHelper?, user users: [ZoomVideoSDKUser]?) {
        // Dispatch to the main thread here if you update UI.
    }
    required init?(coder: NSCoder) { fatalError() }
}
```

### Example 4: Objective-C registration
```objc
MyObserver *observer = [MyObserver new];      // retain it yourself
[ZMUIToolKitManager.videoSDKDelegate addDelegate:observer];
// ...later...
[ZMUIToolKitManager.videoSDKDelegate removeDelegate:observer];
```

## Code Snippets (Reusable Patterns)

### Register + retain pattern
```swift
// The composite stores children weakly; you MUST own the strong reference.
let observer = MyObserver()
self.myObserver = observer                       // strong property
ZMUIToolKitManager.videoSDKDelegate.add(observer)
```

### Main-thread callback handler
```swift
func onUserVideoStatusChanged(_ helper: ZoomVideoSDKVideoHelper?, user users: [ZoomVideoSDKUser]?) {
    // Callbacks are delivered on the thread the SDK invokes (typically main).
    DispatchQueue.main.async {
        // Update UI safely here.
    }
}
```

## Type Definitions

| Type | Kind | Notes |
|------|------|-------|
| `CompositeZoomVideoSDKDelegate` | `public final class : NSObject` | Weak fan-out delegate. Obj-C name `ZMUICompositeZoomVideoSDKDelegate`. Public API: `init()`, `add(_:)`, `remove(_:)`. |
| `ZoomVideoSDKDelegate` | SDK protocol | The protocol the composite conforms to and that child delegates must conform to. All methods are optional. |
| `NSHashTable<AnyObject>` | Foundation class | Configured via `.weakObjects()` for zeroing weak references and pointer-identity dedup. Backs the child store. |
| `ZMUIToolKitManager` | class | Owns the shared instance as `static let videoSDKDelegate`; assigns it to `ZoomVideoSDK.delegate` in `initialize`. |
| `ZoomVideoSDK` | SDK class | Exposes a single `delegate` slot occupied by the composite. |

## Metadata
- **Source file**: `ZMUIToolkit/ZMUIToolkit/Sources/CompositeZoomVideoSDKDelegate.swift`
- **Related file**: `ZMUIToolkit/ZMUIToolkit/Sources/ZMUIToolKitManager.swift` (exposes the shared instance and assigns it to the SDK)
- **Generated for**: AI-assisted code generation and integration
- **Verification**: Behaviors documented above were verified directly against the implementation source (weak `NSHashTable.weakObjects()` storage, `@objc(addDelegate:)` / `@objc(removeDelegate:)` selectors, weakly-held children, optional-chaining forwarding).
