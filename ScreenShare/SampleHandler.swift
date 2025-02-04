//
//  SampleHandler.swift
//  ScreenShare
//
//  This sample code is for debugging purposes only and is provided as-is and without warranties of any kind.
//  It is meant only to be used by the direct recipient and may not be redistributed.
//  Copyright 2025 Zoom Video Communications, Inc. All rights reserved.

import ReplayKit

class SampleHandler: RPBroadcastSampleHandler, ZoomVideoSDKScreenShareServiceDelegate {
    
    var screenShareService: ZoomVideoSDKScreenShareService?

    override init() {
        super.init()
        // Create an instance of ZoomVideoSDKScreenShareService to handle broadcast actions.
        let params = ZoomVideoSDKScreenShareServiceInitParams()
        // Provide your app group ID from your Apple Developer account.
        params.appGroupId = <#App Group ID#>
        // Set this to true to enable sharing device audio during screenshare
        params.isWithDeviceAudio = true
        let service = ZoomVideoSDKScreenShareService(params: params)
        self.screenShareService = service
        screenShareService?.delegate = self
    }
    

    override func broadcastStarted(withSetupInfo setupInfo: [String : NSObject]?) {
        // User has requested to start the broadcast.
        guard let setupInfo = setupInfo else { return }
        // Pass setup info to SDK.
        screenShareService?.broadcastStarted(withSetupInfo: setupInfo)
    }
    
    override func broadcastPaused() {
        // Notify SDK the broadcast was paused.
        screenShareService?.broadcastPaused()
    }
    
    override func broadcastResumed() {
        // Notify SDK the broadcast was resumed.
        screenShareService?.broadcastResumed()
    }
    
    override func broadcastFinished() {
        // Notify SDK the broadcast has finished.
        screenShareService?.broadcastFinished()
    }
    
    override func processSampleBuffer(_ sampleBuffer: CMSampleBuffer, with sampleBufferType: RPSampleBufferType) {
        // Pass sample buffer into SDK.
        screenShareService?.processSampleBuffer(sampleBuffer, with: sampleBufferType)
    }
    
    func zoomVideoSDKScreenShareServiceFinishBroadcastWithError(_ error: Error?) {
        guard let error = error else { return }
        // Terminate broadcast when notified of error from SDK.
        finishBroadcastWithError(error)
    }
}
