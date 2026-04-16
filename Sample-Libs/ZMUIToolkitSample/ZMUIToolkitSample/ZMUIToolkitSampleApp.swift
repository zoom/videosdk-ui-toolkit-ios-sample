//
//  ZMUIToolkitSampleApp.swift
//  ZMUIToolkitSample
//

import SwiftUI
import ZMUIToolkit

@main
struct ZMUIToolkitSampleApp: App {
    
    init() {
        // Enable custom strings from this app's ZMUIToolkit.strings file
        ZMUIToolkitStrings.stringsBundle = Bundle.main
    }
    
    var body: some Scene {
        WindowGroup {
            StartingPageView()
        }
    }
}
