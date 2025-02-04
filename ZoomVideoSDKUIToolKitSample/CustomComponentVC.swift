//
//  CustomComponentVC.swift
//  ZoomVideoSDKUIToolkit
//
//  This sample code is for debugging purposes only and is provided as-is and without warranties of any kind.
//  It is meant only to be used by the direct recipient and may not be redistributed.
//  Copyright 2025 Zoom Video Communications, Inc. All rights reserved.

import UIKit
import ZoomVideoSDK
import ZoomVideoSDKUIToolkit

class CustomComponentVC: UIViewController {
    
    @IBOutlet weak var topNavBar: UIView!
    @IBOutlet weak var activeSpeakerGalleryView: UIView!
    @IBOutlet weak var chatBtn: UIButton!
    @IBOutlet weak var btmNavBar: TitleNavBar!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        setupUI()
        setupData()
    }
    
    override func viewDidDisappear(_ animated: Bool) {
        UIToolkitComponentManager.shared().cleanup()
    }
    
    private func setupUI() {
        setupTopCTABar()
        setupActiveSpeakerGalleryView()
        setupBottomTitleBar()
        
        chatBtn.isEnabled = UIToolkitComponentManager.shared().isChatAvailable()
    }
    
    private func setupTopCTABar() {
        let ctaBarComponent = CTANavBar()
        topNavBar.addSubview(ctaBarComponent)
        ctaBarComponent.fitLayoutTo(view: topNavBar)
        UIToolkitComponentManager.shared().setCTABar(with: ctaBarComponent)
    }
    
    private func setupActiveSpeakerGalleryView() {
        let activeSpeakerGalleryComponent = ActiveSpeakerGalleryView()
        activeSpeakerGalleryView.addSubview(activeSpeakerGalleryComponent)
        activeSpeakerGalleryComponent.fitLayoutTo(view: activeSpeakerGalleryView)
        UIToolkitComponentManager.shared().setActiveSpeakerGalleryView(with: activeSpeakerGalleryComponent)
    }
    
    private func setupBottomTitleBar() {
        let titleBarComponent = TitleNavBar()
        btmNavBar.addSubview(titleBarComponent)
        titleBarComponent.fitLayoutTo(view: btmNavBar)
        UIToolkitComponentManager.shared().setTitleBar(with: titleBarComponent)
    }
    
    private func setupData() {
        UIToolkitComponentManager.shared().setupDelegate(with: self)
    }
    
    @IBAction func onClickParticipantBtn(_ sender: UIButton) {
        //UIToolkitComponentManager.shared().addParticipantsView(to: self, view: self.activeSpeakerGalleryView)
        
        UIToolkitComponentManager.shared().pushParticipantsView()
    }
    
    @IBAction func onClickChatBtn(_ sender: UIButton) {
        UIToolkitComponentManager.shared().addChatView(to: self, view: self.activeSpeakerGalleryView)
    }
    
    @IBAction func onClickSettingBtn(_ sender: UIButton) {
        UIToolkitComponentManager.shared().addSettingsView(to: self, view: self.view)
    }
}

extension CustomComponentVC: UIToolkitDelegate {
    func onError(_ errorType: UIToolkitError) {
        print("Sample CustomComponentVC onError Callback: \(errorType.rawValue) -> \(errorType.description)")
    }
    
    func leaveSession(reason: ZoomVideoSDKSessionLeaveReason) {
        print("Sample CustomComponentVC Left Session, reason: \(reason)")
        self.dismiss(animated: true)
    }
    
    func chatPermissionChanged(enable: Bool) {
        chatBtn.isEnabled = enable
    }
}
