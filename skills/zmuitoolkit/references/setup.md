# ZMUIToolkit Setup & Integration

How to integrate the **ZMUIToolkit** iOS framework into a host app: initialize the Zoom Video SDK, then the toolkit, join a session, and configure the project (especially for screen sharing).

All facts below are verified against the sample app under `ZMUIToolkitSample/` and the framework `Sources/`. When advising on a real integration, **read the user's actual config/entitlements files first** — do not assume the sample's identifiers are theirs.

## Golden-path ordering (do these in order)

1. **Initialize the Zoom Video SDK** — `ZoomVideoSDK.shareInstance()?.initialize(params)`.
2. **Initialize the toolkit** — `ZMUIToolKitManager.initialize(videoSDK:bundleId:)`, passing the same SDK instance and the **broadcast extension bundle id**.
3. **Join a session** — `ZMUIToolKitManager.videoSDK?.joinSession(sessionContext)`.
4. **Tear down** (on sign-out / SDK teardown) — `ZMUIToolKitManager.cleanup()` **then** `ZoomVideoSDK.shareInstance()?.cleanup()`.

> The toolkit takes ownership of `ZoomVideoSDK.delegate`. After `initialize`, observe SDK events via `ZMUIToolKitManager.videoSDKDelegate.add(_:)` — never reassign `ZoomVideoSDK.delegate` yourself.

## ⚠️ First: does this app need screen sharing?

**Screen share is optional.** It requires extra, non-trivial project setup — a separate Broadcast Upload Extension target, a shared App Group, and matching identifiers across four places (Steps 5 & 6). An app that doesn't share its screen needs none of it.

**Before doing any screen-share setup, ask the user whether they intend to support screen sharing in their app**, and branch:

- **Not using screen share** → skip Steps 5 & 6 entirely. In Step 1 omit `appGroupId`, and in Step 2 pass `bundleId: nil`. **Do not add the `ZMUIToolkitShareBtn` to the UI** — include it only in apps that support ReplayKit screen sharing. Everything else (audio, video, participants, leave, video grid) works fully.
- **Using screen share** → do Steps 5 & 6, set `appGroupId` and `bundleId`, and add `ZMUIToolkitShareBtn`.

The steps below mark screen-share-only work with **[screen share only]**.

---

## Step 1 — Initialize the Video SDK

```swift
import ZoomVideoSDK

let sdkInitParams = ZoomVideoSDKInitParams()
sdkInitParams.domain = "zoom.us"                 // your SDK domain
sdkInitParams.enableLog = true
sdkInitParams.logFilePrefix = "myapp"            // any prefix for SDK log files
sdkInitParams.appGroupId = "group.<your-app-group>"   // [screen share only] — omit if not sharing the screen

let error = ZoomVideoSDK.shareInstance()?.initialize(sdkInitParams) ?? .Errors_Internal_Error
guard error == .Errors_Success else {
    // Inspect error: .Errors_Auth_*, .Errors_Load_Module_Error, .Errors_Invalid_Parameter, etc.
    return
}
```

- `appGroupId` **must** be set here (and match the extension) for screen sharing to work — it is how the SDK and the broadcast extension share a container.
- Initialize the SDK **once** per process. The sample guards this with a `static var isSDKInitialized` flag (`ZMUIToolkitSampleConfig.swift`).
- Reference: `ZMUIToolkitSample/ZMUIToolkitSample/StartingPageView.swift` (`initializeToolkitIfNeeded`).

## Step 2 — Initialize the Toolkit

```swift
guard let sdk = ZoomVideoSDK.shareInstance() else { return }

let initialized = ZMUIToolKitManager.initialize(
    videoSDK: sdk,
    bundleId: "com.example.myapp.ScreenShare"   // [screen share only] BROADCAST EXTENSION bundle id; pass nil if not sharing
)
// initialized == false means the toolkit could not attach to the provided SDK.
```

Signature (`Sources/ZMUIToolKitManager.swift`):
```swift
@MainActor @objc static public func initialize(videoSDK: ZoomVideoSDK, bundleId: String?) -> Bool
```

What it does:
- Stores the SDK instance (`ZMUIToolKitManager.videoSDK`) and the `bundleId`.
- Returns `false` and rolls back if the toolkit cannot attach to the provided SDK.
- On success, installs the toolkit's fan-out delegate: `videoSDK.delegate = videoSDKDelegate`.

**`@MainActor`** — call on the main thread.

Treat the `Bool` return from `initialize` as authoritative for toolkit attachment; do not use `getVersion()` as your own SDK-initialization check.

### What `bundleId` is for
The `bundleId` you pass is the **Broadcast Upload Extension's** bundle id. The Share button assigns it to the system broadcast picker:
```swift
// Sources/Buttons/ZMUIToolkitShareBtn.swift
if (!ZMUIToolKitManager.bundleId.isEmpty) {
    broadcastPickerView.preferredExtension = ZMUIToolKitManager.bundleId
}
```
If `bundleId` is empty, `preferredExtension` is never set, so the picker cannot pre-select your extension. Confirm the extension configuration if screen sharing does not start as expected.

## Step 3 — Join a Session

```swift
let sessionContext = ZoomVideoSDKSessionContext()
sessionContext.sessionName = sessionName
sessionContext.sessionPassword = sessionPassword
sessionContext.userName = displayName
sessionContext.token = jwtToken                 // JWT auth token (generate server-side)
sessionContext.sessionIdleTimeoutMins = 60

let audioOption = ZoomVideoSDKAudioOptions()
audioOption.connect = true
audioOption.mute = false
sessionContext.audioOption = audioOption

let videoOption = ZoomVideoSDKVideoOptions()
videoOption.localVideoOn = true
videoOption.multitaskingCameraAccessEnabled = true   // REQUIRED for Picture-in-Picture
sessionContext.videoOption = videoOption

let session = ZMUIToolKitManager.videoSDK?.joinSession(sessionContext)
```

- Authentication is via the **JWT `token`** on the session context (typically minted server-side from your SDK key/secret — do not ship the secret in a production app).
- `multitaskingCameraAccessEnabled = true` forwards to `AVCaptureSession.isMultitaskingCameraAccessEnabled` and is required for PiP.
- Reference: `ZMUIToolkitSample/ZMUIToolkitSample/JoinSessionView.swift`.

## Step 4 — Teardown

```swift
ZMUIToolKitManager.cleanup()                 // stops PiP, releases SDK ref, clears delegate slot if owned
ZoomVideoSDK.shareInstance()?.cleanup()      // then tear down the SDK itself
```

- `cleanup()` is `@MainActor`. It stops PiP, and clears `ZoomVideoSDK.delegate` **only if** the toolkit still owns it (`sdk.delegate === videoSDKDelegate`).
- Observers registered with `videoSDKDelegate.add(_:)` are held **weakly** and survive `cleanup()` → re-`initialize()`, so listeners persist across re-init.
- The sample app intentionally does **not** call cleanup (it keeps the toolkit alive for the process lifetime) — real integrations should pair init with cleanup on sign-out.

---

## Step 5 — Project settings for Screen Share — [screen share only]

> Skip this step and Step 6 if the app does not support screen sharing.

Screen share on iOS requires a **Broadcast Upload Extension** target plus a shared **App Group**. Three identifiers must line up across four places:

| Identifier | Example (placeholder — use your own) | Where it must appear |
|------------|--------------------------------------|----------------------|
| App Group id | `group.com.example.myapp` | Main app entitlements, extension entitlements, `ZoomVideoSDKInitParams.appGroupId`, extension's `ZoomVideoSDKScreenShareServiceInitParams.appGroupId` |
| Extension bundle id | `com.example.myapp.ScreenShare` | Extension target `PRODUCT_BUNDLE_IDENTIFIER`, `bundleId` arg to `ZMUIToolKitManager.initialize` |
| Main app bundle id | `com.example.myapp` | Main app target |

> The extension bundle id is conventionally the app bundle id + a suffix (e.g. `.ScreenShare`), but the only hard requirement is that the value passed as `bundleId` equals the extension target's actual bundle id.

### Main app target

**Entitlements** (`ZMUIToolkitSample.entitlements`) — App Group:
```xml
<key>com.apple.security.application-groups</key>
<array>
    <string>group.com.example.myapp</string>
</array>
```

**Info.plist** — background modes for live audio/video:
```xml
<key>UIBackgroundModes</key>
<array>
    <string>voip</string>
    <string>audio</string>
</array>
```

**Privacy usage descriptions** (in the sample these are set via `INFOPLIST_KEY_*` build settings; in most projects they go in Info.plist):
- `NSCameraUsageDescription`
- `NSMicrophoneUsageDescription`
- `NSBluetoothAlwaysUsageDescription` (if using Bluetooth audio)
- `NSPhotoLibraryUsageDescription` (if sharing photos)

### Capabilities checklist (main app)
- ✅ App Groups (the group above)
- ✅ Background Modes: Audio, AirPlay, and Picture in Picture + Voice over IP

---

## Step 6 — Broadcast Upload Extension — [screen share only]

Add a **Broadcast Upload Extension** target (File ▸ New ▸ Target ▸ Broadcast Upload Extension). Then:

### Extension entitlements
Must contain the **same** App Group as the main app:
```xml
<key>com.apple.security.application-groups</key>
<array>
    <string>group.com.example.myapp</string>
</array>
```

### Extension Info.plist
```xml
<key>NSExtension</key>
<dict>
    <key>NSExtensionPointIdentifier</key>
    <string>com.apple.broadcast-services-upload</string>
    <key>NSExtensionPrincipalClass</key>
    <string>$(PRODUCT_MODULE_NAME).SampleHandler</string>
    <key>RPBroadcastProcessMode</key>
    <string>RPBroadcastProcessModeSampleBuffer</string>
</dict>
```

### SampleHandler implementation
Subclass `RPBroadcastSampleHandler`, conform to `ZoomVideoSDKScreenShareServiceDelegate`, and forward every ReplayKit lifecycle callback to a `ZoomVideoSDKScreenShareService` configured with the **same App Group id**:

```swift
import ReplayKit
import ZoomVideoSDK   // (sample imports the SDK that vends ZoomVideoSDKScreenShareService)

class SampleHandler: RPBroadcastSampleHandler, ZoomVideoSDKScreenShareServiceDelegate {
    var screenShareService: ZoomVideoSDKScreenShareService!

    override init() {
        let params = ZoomVideoSDKScreenShareServiceInitParams()
        params.appGroupId = "group.com.example.myapp"   // MUST match the main app's App Group
        params.isWithDeviceAudio = true
        super.init()
        let service = ZoomVideoSDKScreenShareService(params: params)
        self.screenShareService = service
        self.screenShareService.delegate = self
    }

    func zoomVideoSDKScreenShareServiceFinishBroadcastWithError(_ error: (any Error)?) {
        guard let error = error else { return }
        self.finishBroadcastWithError(error)
    }

    override func broadcastStarted(withSetupInfo setupInfo: [String : NSObject]?) {
        self.screenShareService.broadcastStarted(withSetupInfo: setupInfo)
    }
    override func broadcastPaused()  { self.screenShareService.broadcastPaused() }
    override func broadcastResumed() { self.screenShareService.broadcastResumed() }
    override func broadcastFinished(){ self.screenShareService.broadcastFinished() }

    override func processSampleBuffer(_ sampleBuffer: CMSampleBuffer, with sampleBufferType: RPSampleBufferType) {
        self.screenShareService.processSampleBuffer(sampleBuffer, with: sampleBufferType)
    }
}
```

Reference: `ZMUIToolkitSample/ZMUIToolkitSampleBroadcastExtension/SampleHandler.swift`.

---

## Using the toolkit UI

After init + join, drop the public components into your view hierarchy. They self-register for SDK events (where applicable) and need no further wiring:

```swift
let audioBtn   = ZMUIToolkitAudioBtn()
let videoBtn   = ZMUIToolkitVideoBtn()
let leaveBtn   = ZMUIToolkitLeaveSessionBtn()
let videoView  = ZMUIToolkitRootVideoView(...)   // main video grid
let shareBtn   = ZMUIToolkitShareBtn()           // [screen share only] — add only if Steps 5 & 6 are done
```

- **Only add `ZMUIToolkitShareBtn` if the app supports screen sharing** (Steps 5 & 6 completed and `bundleId` set).
- **Add `ZMUIToolkitRootVideoView` to a window-backed view hierarchy** so it can begin observing SDK events and rendering live session content.
- See per-component docs in `docs/api/` for behavior, states, and gotchas.

### Optional: localization
Set `ZMUIToolkitStrings.stringsBundle = Bundle.main` and provide a `ZMUIToolkit.strings` table to override toolkit text. (Avoid localizing a key to a value byte-identical to the key name — the resolver treats that as "missing" and falls back to the default.)

### Optional: recording consent
`ZMUIToolKitManager.cloudRecordingConsentDialogEnabled` (default `true`). Set `false` to suppress the blocking consent dialog (auto-accepts) and show a non-blocking toast instead.

---

## Troubleshooting

| Symptom | Likely cause |
|---------|--------------|
| Tapping Share does nothing, no error | `bundleId` not passed to `initialize`, or it doesn't match the extension's bundle id → `preferredExtension` unset. Also check the share start has no error feedback by design. |
| Share picker appears but broadcast never connects | App Group mismatch between main app `ZoomVideoSDKInitParams.appGroupId`, the extension entitlements, and `ZoomVideoSDKScreenShareServiceInitParams.appGroupId`. All three must be identical. |
| `ZMUIToolKitManager.initialize` returns `false` | The toolkit could not attach to the provided SDK. Re-check the host SDK lifecycle, make sure Step 1 completed successfully, then try Step 2 again. |
| Video view shows nothing / no updates | `ZMUIToolkitRootVideoView` not added to a window (delegate registers on `didMoveToWindow`). |
| PiP doesn't work | `videoOption.multitaskingCameraAccessEnabled` not set to `true` at join; or Background Modes capability missing. |
| Auth errors on join | Bad/expired JWT `token`, or key/secret mismatch when minting it. |
| Buttons don't reflect host-initiated changes | Confirm the toolkit owns `ZoomVideoSDK.delegate` (don't reassign it); observe via `videoSDKDelegate.add(_:)`. |

## Key references
- `ZMUIToolkitSample/ZMUIToolkitSample/StartingPageView.swift` — SDK + toolkit init
- `ZMUIToolkitSample/ZMUIToolkitSample/JoinSessionView.swift` — session join
- `ZMUIToolkitSample/ZMUIToolkitSample/ZMUIToolkitSampleConfig.swift` — config identifiers
- `ZMUIToolkitSample/ZMUIToolkitSampleBroadcastExtension/` — extension target, entitlements, Info.plist, SampleHandler
- `ZMUIToolkit/ZMUIToolkit/Sources/ZMUIToolKitManager.swift` — initialize/cleanup
- `docs/api/` — per-component API documentation
- `README.md` — building the xcframework and configuring the sample
