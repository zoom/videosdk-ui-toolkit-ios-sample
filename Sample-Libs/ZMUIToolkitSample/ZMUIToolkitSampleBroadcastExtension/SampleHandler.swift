//
//  SampleHandler.swift
//  ZMUIToolkitSampleBroadcastExtension
//

import ReplayKit
import ZMUIToolkit

class SampleHandler: RPBroadcastSampleHandler, ZMUIToolkitScreenShareServiceDelegate {
    func zoomVideoSDKScreenShareServiceFinishBroadcastWithError(_ error: (any Error)?) {
        guard let error = error else { return }
        self.finishBroadcastWithError(error)
    }
    
    
    var screenShareService: ZMUIToolkitScreenShareService!
    
    override init() {
        let params = ZMUIToolkitScreenShareInitParams()
        /**
        * if you need use screen share feature, Here are a few things to note:
        * <1> Create your own groupid on the Apple Developer Web site, and fill the group ID in ZMUIToolkitSampleConfig.swift
        * <2> Create an "App Groups" Capability in the main project target and the replayKit project target, and select the groupId correctly.
        * <3> If you can't select groupId correctly in "App Groups" Capability, Please check ZMUIToolkitSample.Entitlements and ZMUIToolkitSampleBroadcastExtension.entitlements this two files, here also need to configure the correct group id.
            *   <key>com.apple.security.application-groups</key>
        <array>
        <string> your group id </string>
        </array
        *
        * For details, please refer: https://marketplace.zoom.us/docs/sdk/video/ios/advanced/screen-share
        *
        * if you don't need screen share, please delete ZMUIToolkitSampleBroadcastExtension target.
        *
        */
        params.appGroupId = ZMUIToolkitSampleConfig.screenShareAppGroupId;
        params.isWithDeviceAudio = true;
            
        super.init()
        let service = ZMUIToolkitScreenShareService(params: params)
        self.screenShareService = service
        
        self.screenShareService.delegate = self
    }

    override func broadcastStarted(withSetupInfo setupInfo: [String : NSObject]?) {
        // User has requested to start the broadcast. Setup info from the UI extension can be supplied but optional.
        self.screenShareService.broadcastStarted(withSetupInfo: setupInfo)
    }
    
    override func broadcastPaused() {
        // User has requested to pause the broadcast. Samples will stop being delivered.
        self.screenShareService.broadcastPaused()
    }
    
    override func broadcastResumed() {
        // User has requested to resume the broadcast. Samples delivery will resume.
        self.screenShareService.broadcastResumed()
    }
    
    override func broadcastFinished() {
        // User has requested to finish the broadcast.
        self.screenShareService.broadcastFinished()
    }
    
    override func processSampleBuffer(_ sampleBuffer: CMSampleBuffer, with sampleBufferType: RPSampleBufferType) {
        self.screenShareService.processSampleBuffer(sampleBuffer, with: sampleBufferType)
    }
}
