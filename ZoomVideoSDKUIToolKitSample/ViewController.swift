//
//  ViewController.swift
//  ZoomVideoSDKUIToolKitSample
//
//  This sample code is for debugging purposes only and is provided as-is and without warranties of any kind.
//  It is meant only to be used by the direct recipient and may not be redistributed.
//  Copyright 2025 Zoom Video Communications, Inc. All rights reserved.

import UIKit
import ZoomVideoSDK
import ZoomVideoSDKUIToolkit

class ViewController: UIViewController {
    
    let jwt = <#JWT#>
    let sessionName = <#Session Name#>
    let username = <#Username#>
    let isDefaultView = true
    
//    let password = <#Password#> // For session that requires password in order to join.
    let appGroupID = <#App Group ID#> // For screen sharing of device.
//    let recordingConsentMessage = "You are currently being recorded and must either accept or deny to continue. If you choose to deny, you will be forced to leave the session." // For customized consent message that will be shown during the start of cloud recording.
//    let liveStreamingConsentMessage = "You are currently being live streamed and must either accept or deny to continue. If you choose to deny, you will be forced to leave the session." // For customized consent message that will be shown during the start of live streaming.
    
    override func viewDidLoad() {
        super.viewDidLoad()
    }
    
    @IBAction func onClickStartSessionBtn(_ sender: UIButton) {
        // If your session requires a password, you will need to add it under the sessionPassword parameter under SessionContext.
        // let vc = UIToolkitVC(sessionContext: SessionContext(jwt: jwt, sessionName: sessionName, sessionPassword: password, username: username))
        
        /*
         Under the InitParams, all parameters are optional:
         1. If your session allows screen sharing, you will need to add the App Group ID parameter,
         2. By default the UI Toolkits comes with all available features (with some features require additional license). If you will like to only use some of these features, you will need to add the features you want under the features parameter.
         3. If your session allows and can perform cloud recording, you can add in a customized consent message.
         4. If your session allows and can perform live streaming, you can add in a customized consent message.
         */
        // let vc = UIToolkitVC(sessionContext: SessionContext(jwt: jwt, sessionName: sessionName, username: username), initParams: InitParams(appGroupId: appGroupID, recordingConsentMessage: recordingConsentMessage, liveStreamConsentMessage: liveStreamingConsentMessage))
        let vc = UIToolkitVC(sessionContext: SessionContext(jwt: jwt, sessionName: sessionName, username: username))
        
        /* Default UI */
        if isDefaultView {
            vc.delegate = self
            vc.modalPresentationStyle = .fullScreen
            present(vc, animated: true)
        }
        /* Component UI */
        else {
            UIToolkitComponentManager.shared().setup(with: SessionContext(jwt: jwt, sessionName: sessionName, username: username))
            UIToolkitComponentManager.shared().setupDelegate(with: self)
            UIToolkitComponentManager.shared().startJoinSession()
        }
    }

}

// MARK: Zoom Video UI Kit Delegate

extension ViewController: UIToolkitDelegate {
    func onError(_ errorType: UIToolkitError) {
        print("Sample VC onError Callback: \(errorType.rawValue) -> \(errorType.description)")
    }
    
    /*
     Default UI
     */
    func onViewLoaded() {
        print("Sample VC onViewLoaded")
    }
    
    func onViewDismissed() {
        print("Sample VC onViewDismissed")
    }
    
    /*
     Component UI
     */
    func startJoinSessionSuccessed() {
        print("Sample VC Start/Join Session Successfully")
        performSegue(withIdentifier: "goCustomVC", sender: nil)
    }
    
    func leaveSession(reason: ZoomVideoSDKSessionLeaveReason) {
        print("Sample VC Left Session, reason: \(reason)")
    }
}
