//
//  EventListener.swift
//  ZMUIToolkitSample
//

import Foundation
import ZMUIToolkit
import ZoomVideoSDK

final class EventListener: NSObject, ObservableObject, ZoomVideoSDKDelegate {

    @Published var isJoined: Bool = false

    override init() {
        super.init()
        ZMUIToolKitManager.videoSDKDelegate.add(self)
    }

    deinit {
        ZMUIToolKitManager.videoSDKDelegate.remove(self)
    }

    private func updateJoinedState(_ joined: Bool) {
        DispatchQueue.main.async {
            self.isJoined = joined
        }
    }

    func onSessionJoin() {
        updateJoinedState(true)
    }

    func onError(_ ErrorType: ZoomVideoSDKError, detail details: Int) {
        print("[EventListener] SDK error: \(ErrorType) detail: \(details)")
    }

    func onSessionLeave(_ reason: ZoomVideoSDKSessionLeaveReason) {
        let reasonString: String

        switch reason {
        case .bySelf:
            reasonString = "User left voluntarily"
        case .kickByHost:
            reasonString = "Kicked by host"
        case .endByHost:
            reasonString = "Session ended by host"
        case .networkError:
            reasonString = "Network error"
        case .joinSubsession:
            reasonString = "Joining subsession"
        case .returnToMainSession:
            reasonString = "Returning to main session"
        case .unknown:
            reasonString = "Unknown reason"
        @unknown default:
            reasonString = "Unhandled reason"
        }

        print("[EventListener] Session left - Reason: \(reasonString)")
        updateJoinedState(false)
    }
}
