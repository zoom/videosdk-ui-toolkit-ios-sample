# Zoom Video SDK UI toolkit Sample Project 

The Zoom Video SDK UI Toolkit is a prebuilt video chat user interface powered by the Zoom Video SDK.

The use of this UI toolkit is subject to the [beta program terms of use](https://explore.zoom.us/en/beta-terms-and-conditions/) and the [Video SDK terms of service](https://explore.zoom.us/en/video-sdk-terms/). This sample code is for debugging purposes only and is provided as-is and without warranties of any kind. It is meant only to be used by the direct recipient and may not be redistributed. Copyright 2023 Zoom Video Communications, Inc. All rights reserved.

## Authorize

Learn how to use your credentials to [authenticate](https://developers.zoom.us/docs/video-sdk/auth/#generate-a-video-sdk-jwt) so you can connect.

See the [Video SDK Auth Endpoint Sample](https://github.com/zoom/videosdk-sample-signature-node.js) for a sample app that shows how to quickly, easily, and securely generate a Video SDK JWT.

## Installation, Build & Launch

Currently, the sample app consists of 2 dynamic xcframeworks ZoomVideoSDK.xcframework and ZoomVideoSDKUIKit.xcframework which are added with "Embed & Sign" and can be located directly under the ZoomVideoSDKUIToolkitSample. 

Build and run the sample app by navigating ViewController.swift and key in the following placeholders. If your session requires a password, you can make use of the password parameter.

```Swift
let jwt = <#JWT#>
let sessionName = <#Session Name#>
let username = <#Username#>

// If your session requires a password, you can use the password variable here as well.
// let password = "<#Password#>
```

<br>

The variables above are used to set up a connection with the Video SDK. If your session requires a password, you must use the **SessionContext** class that comes with a sessionPassword parameter.

```Swift
// If your session requires a password, you will need to add it under the sessionPassword parameter under SessionContext.
// let vc = UIToolkitVC(sessionContext: SessionContext(jwt: jwt, sessionName: sessionName, sessionPassword: password, username: username))

let vc = UIToolkitVC(sessionContext: SessionContext(jwt: jwt, sessionName: sessionName, username: username))
vc.delegate = self
vc.modalPresentationStyle = .fullScreen
present(vc, animated: true)
```

<br>

There is a delegate class **UIToolkitDelegate** which consists of essential callbacks such as error, view is loaded, and dismissed.

```Swift
extension ViewController: UIToolkitDelegate {
    func onError(_ errorType: UIToolkitError) {
        print("Sample VC onError Callback: \(errorType.rawValue) -> \(errorType.description)")
    }
    
    func onViewLoaded() {
        print("Sample VC onViewLoaded")
    }
    
    func onViewDismissed() {
        print("Sample VC onViewDismissed")
    }
}
```

## Integration

You can integrate manually or use the Swift Package Manager.

### Manual

You can integrate the Zoom Video SDK UI toolkit into your project by adding the 2 dynamic xcframeworks: ZoomVideoSDK.xcframework and ZoomVideoSDKUIKit.xcframework found in the sample app. Be sure to select the "Embed & Sign" option for these frameworks in Xcode under your project 'General' tab.

### Swift Package Manager

You can also add the framework using the Swift Package Manager using our GitHub link: https://github.com/zoom/videosdk-ui-toolkit-ios.

## Need help?

If you're looking for help, try [Developer Support](https://devsupport.zoom.us/hc/en-us) or our [Developer Forum](https://devforum.zoom.us/). Priority support is also available with [Premier Developer Support](https://explore.zoom.us/docs/en-us/developer-support-plans.html) plans.

---

Copyright 2023 Zoom Video Communications, Inc. All rights reserved.
