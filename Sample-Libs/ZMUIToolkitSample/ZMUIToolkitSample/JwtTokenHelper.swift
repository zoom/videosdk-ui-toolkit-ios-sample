//
//  JwtTokenHelper.swift
//  ZMUIToolkitSample
//

import Foundation
import CryptoKit

/// Helper for Zoom Video SDK JWT tokens.
///
/// Production: Generate the JWT on your backend and fetch it from the app (see [fetchTokenFromServer]).
/// Development only: Use [generateZoomVideoSdkToken] with your SDK secret. Never ship the secret in release builds.
enum JwtTokenHelper {
    /// Generate a Zoom Video SDK JWT on device. **Development / testing only.**
    /// Do not embed your SDK secret in production; use a backend to issue tokens.
    ///
    /// - Parameters:
    ///   - sdkKey: Zoom Video SDK key
    ///   - sdkSecret: Zoom Video SDK secret (never commit hardcoded secrets)
    ///   - sessionName: Topic/session name (for example: "monument-valley-770")
    ///   - sessionKey: Optional session key (empty when not used)
    ///   - userIdentity: User identifier (for example: "13579")
    ///   - roleType: 1 = host, 0 = participant
    ///   - validSeconds: Token lifetime in seconds (for example: 86400)
    static func generateZoomVideoSdkToken(
        sdkKey: String,
        sdkSecret: String,
        sessionName: String,
        sessionKey: String = "",
        userIdentity: String = "123456",
        roleType: Int = 1,
        validSeconds: Int = 86400
    ) -> String {
        let now = Int(Date().timeIntervalSince1970)
        let payload: [String: Any] = [
            "app_key": sdkKey,
            "version": 1,
            "iat": now,
            "exp": now + validSeconds,
            "tpc": sessionName,
            "session_key": sessionKey,
            "role_type": roleType,
            "user_identity": userIdentity
        ]
        return signJwtHs256(payload: payload, secret: sdkSecret)
    }

    /// Sign a JWT with HS256. Header: {"alg":"HS256","typ":"JWT"}.
    private static func signJwtHs256(payload: [String: Any], secret: String) -> String {
        let header: [String: String] = ["alg": "HS256", "typ": "JWT"]

        let headerData = (try? JSONSerialization.data(withJSONObject: header, options: [])) ?? Data()
        let payloadData = (try? JSONSerialization.data(withJSONObject: payload, options: [])) ?? Data()

        let headerB64 = base64UrlEncode(headerData)
        let payloadB64 = base64UrlEncode(payloadData)
        let signingInput = "\(headerB64).\(payloadB64)"

        let key = SymmetricKey(data: Data(secret.utf8))
        let signature = HMAC<SHA256>.authenticationCode(for: Data(signingInput.utf8), using: key)
        let signatureData = Data(signature)
        let signatureB64 = base64UrlEncode(signatureData)

        return "\(signingInput).\(signatureB64)"
    }

    private static func base64UrlEncode(_ data: Data) -> String {
        data.base64EncodedString()
            .replacingOccurrences(of: "+", with: "-")
            .replacingOccurrences(of: "/", with: "_")
            .replacingOccurrences(of: "=", with: "")
    }
}
