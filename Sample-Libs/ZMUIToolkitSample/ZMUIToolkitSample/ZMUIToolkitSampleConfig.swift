//
//  ZMUIToolkitSampleConfig.swift
//  ZMUIToolkitSample
//

import Foundation

enum ZMUIToolkitSampleConfig {
    /// Video SDK domain used for initialization.
    static let sdkDomain: String = "zoom.us"
    
    /// Video SDK log prefix used for initialization.
    static let logFilePrefix: String = "uitoolkit-sample"

    /// App-scoped SDK initialization flag to avoid repeated initialize calls.
    static var isSDKInitialized: Bool = false

    /// App Group ID shared between the main app and the main app's broadcast extension.
    /// Example: "group.com.yourcompany.yourapp"
    static let screenShareAppGroupId: String = <#App Group ID#>
    
    /// Bundle identifier of the main app's broadcast extension.
    /// Example: "com.yourcompany.yourapp.ScreenShare"
    static let screenShareExtensionBundleId: String = <#Bundle ID#>
    
    /// Video SDK app key.
    static let sdkKey: String = <#SDK Key#>

    /// Video SDK app secret.
    static let sdkSecret: String = <#SDK Secret#>
}

