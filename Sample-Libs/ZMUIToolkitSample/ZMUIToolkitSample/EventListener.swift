//
//  EventListener.swift
//  ZMUIToolkitSample
//

import Foundation
import ZMUIToolkit

class EventListener: ObservableObject, ZMUIToolkitEventHandler {

    @Published var isJoined: Bool = false

    func onSessionJoin() {
        isJoined = true
    }

    func onSessionLeave(_ reason: ZMUIToolkitLeaveSessionReason) {
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
        case .unknown:
            reasonString = "Unknown reason"
        @unknown default:
            reasonString = "Unhandled reason"
        }

        print("[EventListener] Session left - Reason: \(reasonString)")
        isJoined = false
    }

    func onUserJoin(_ users: [ZMUIToolkitUser]?) {
        print("[EventListener] Users joined: \(users?.map { $0.userName ?? "unknown" } ?? [])")
    }

    func onUserLeave(_ users: [ZMUIToolkitUser]?) {
        print("[EventListener] Users left: \(users?.map { $0.userName ?? "unknown" } ?? [])")
    }

    func onUserVideoStatusChanged(_ users: [ZMUIToolkitUser]?) {
        print("[EventListener] Video status changed: \(users?.map { $0.userName ?? "unknown" } ?? [])")
    }

    func onUserAudioStatusChanged(_ users: [ZMUIToolkitUser]?) {
        print("[EventListener] Audio status changed: \(users?.map { $0.userName ?? "unknown" } ?? [])")
    }

    func onUserShareStatusChanged(_ user: ZMUIToolkitUser?, shareStatus: ZMUIToolkitShareStatus) {
        print("[EventListener] Share status changed for \(user?.userName ?? "unknown"): \(shareStatus)")
    }

    func onUserNameChanged(_ user: ZMUIToolkitUser?) {
        print("[EventListener] User renamed: \(user?.userName ?? "unknown")")
    }

    func onUserHostChanged(_ user: ZMUIToolkitUser?) {
        print("[EventListener] Host changed to: \(user?.userName ?? "unknown")")
    }

    func onUserManagerChanged(_ user: ZMUIToolkitUser?) {
        print("[EventListener] Manager changed: \(user?.userName ?? "unknown")")
    }

    func onUserActiveAudioChanged(_ users: [ZMUIToolkitUser]?) {
        print("[EventListener] Active audio changed: \(users?.map { $0.userName ?? "unknown" } ?? [])")
    }
}
