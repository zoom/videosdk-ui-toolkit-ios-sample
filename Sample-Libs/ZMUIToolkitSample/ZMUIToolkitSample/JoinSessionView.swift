//
//  ContentView.swift
//  ZMUIToolkitSample
//

import SwiftUI
import ZMUIToolkit

struct JoinSessionView: View {
    enum Mode {
        case create
        case join

        var title: String {
            switch self {
            case .create:
                return "Create a session"
            case .join:
                return "Join a session"
            }
        }

        var actionTitle: String {
            switch self {
            case .create:
                return "Create"
            case .join:
                return "Join"
            }
        }
    }

    let mode: Mode
    private enum Layout {
        static let fieldBottomSpacing: CGFloat = 18
        static let passwordBottomSpacing: CGFloat = 34
        static let actionButtonHeight: CGFloat = 44
        static let actionButtonCornerRadius: CGFloat = 6
        static let actionButtonWidthPercent: CGFloat = 0.6
        static let horizontalPadding: CGFloat = 24
        static let topPadding: CGFloat = 12
    }

    @State private var sessionName = ""
    @State private var displayName = ""
    @State private var sessionPassword = ""
    @State private var sessionIdleTimeoutMins = ""
    @State private var roleType = ""
    @StateObject private var eventListener: EventListener = EventListener()
    @State public var videoSession: ZMUIToolkitSession?

    var body: some View {
        if eventListener.isJoined == false {
            GeometryReader { geometry in
                ScrollView {
                    VStack(alignment: .leading, spacing: 0) {
                        LabeledLineTextField(
                            label: "Session Name",
                            placeholder: "Enter session name.",
                            text: $sessionName
                        )
                        .padding(.bottom, Layout.fieldBottomSpacing)

                        LabeledLineTextField(
                            label: "Display name",
                            placeholder: "Enter display name.",
                            text: $displayName
                        )
                        .padding(.bottom, Layout.fieldBottomSpacing)

                        LabeledLineSecureField(
                            label: "Session Password",
                            placeholder: "Enter password.",
                            text: $sessionPassword
                        )
                        .padding(.bottom, Layout.passwordBottomSpacing)
                        
                        LabeledLineTextField(
                            label: "SessionIdleTimeoutMins",
                            placeholder: "Enter session idle timeout in minutes.",
                            text: $sessionIdleTimeoutMins
                        )
                        .padding(.bottom, Layout.fieldBottomSpacing)
                        
                        LabeledLineTextField(
                            label: "Role Type",
                            placeholder: "Enter 1 for host, 0 for attendee.",
                            text: $roleType
                        )
                        .padding(.bottom, Layout.fieldBottomSpacing)

                        Button(action: {
                            joinSession()
                        }) {
                            Text(mode.actionTitle)
                                .font(.headline)
                                .foregroundStyle(.white)
                                .frame(maxWidth: .infinity)
                                .frame(height: Layout.actionButtonHeight)
                                .background(Color(uiColor: .systemBlue))
                                .clipShape(RoundedRectangle(cornerRadius: Layout.actionButtonCornerRadius))
                        }
                        .frame(width: geometry.size.width * Layout.actionButtonWidthPercent, alignment: .center)
                        .frame(maxWidth: .infinity, alignment: .center)

                        Spacer(minLength: 0)
                    }
                }
                .padding(.horizontal, Layout.horizontalPadding)
                .padding(.top, Layout.topPadding)
            }
            .navigationTitle(mode.title)
            .navigationBarTitleDisplayMode(.inline)
            .toolbar(.visible, for: .navigationBar)
        } else {
            InSessionView(sessionName: sessionName)
                .navigationBarBackButtonHidden(true)
                .toolbar(.hidden, for: .navigationBar)
        }
    }
    
    func joinSession() {
        if ZMUIToolkitSampleConfig.isSDKInitialized == false {
            print("[JoinSession] ERROR: SDK is not initialized")
            return
        }

        ZMUIToolKitManager.setEventHandler(handler: eventListener)

        let type: Int

        if let intValue = Int(roleType), (intValue == 1 || intValue == 0) {
            type = intValue
        } else {
            type = 1
        }

        let token: String = JwtTokenHelper.generateZoomVideoSdkToken(
            sdkKey: ZMUIToolkitSampleConfig.sdkKey,
            sdkSecret: ZMUIToolkitSampleConfig.sdkSecret,
            sessionName: sessionName,
            userIdentity: displayName,
            roleType: type,
            validSeconds: 86400
        )

        if token.isEmpty {
            print("[JoinSession] ERROR: Failed to generate JWT token")
            return
        }

        let sessioncontext = ZMUIToolkitVideoSDKSessionContext()
        sessioncontext.sessionName = sessionName
        sessioncontext.sessionPassword = sessionPassword
        sessioncontext.userName = displayName
        sessioncontext.token = token
        sessioncontext.localVideoOn = true
        sessioncontext.connect = true
        sessioncontext.mute = false

        videoSession = ZMUIToolKitManager.joinSession(context: sessioncontext)

        if videoSession == nil {
            print("[JoinSession] ERROR: Failed to create session - joinSession returned nil")
            print("[JoinSession] This could indicate:")
            print("[JoinSession]   - Invalid session token")
            print("[JoinSession]   - Network connectivity issues")
            print("[JoinSession]   - SDK not properly initialized")
            print("[JoinSession]   - Session name/password mismatch")
        }
    }
}

private struct LabeledLineTextField: View {
    let label: String
    let placeholder: String
    @Binding var text: String
    private enum Layout {
        static let stackSpacing: CGFloat = 8
        static let inputBottomPadding: CGFloat = 8
        static let lineHeight: CGFloat = 1
    }

    var body: some View {
        VStack(alignment: .leading, spacing: Layout.stackSpacing) {
            Text(label)
                .font(.subheadline.weight(.medium))
                .foregroundStyle(Color(uiColor: .label))

            TextField(placeholder, text: $text)
                .autocorrectionDisabled(true)
                .textInputAutocapitalization(.never)
                .foregroundStyle(Color(uiColor: .label))
                .padding(.bottom, Layout.inputBottomPadding)

            Rectangle()
                .fill(Color(uiColor: .separator))
                .frame(height: Layout.lineHeight)
        }
    }
}

private struct LabeledLineSecureField: View {
    let label: String
    let placeholder: String
    @Binding var text: String
    private enum Layout {
        static let stackSpacing: CGFloat = 8
        static let inputBottomPadding: CGFloat = 8
        static let lineHeight: CGFloat = 1
    }

    var body: some View {
        VStack(alignment: .leading, spacing: Layout.stackSpacing) {
            Text(label)
                .font(.subheadline.weight(.medium))
                .foregroundStyle(Color(uiColor: .label))

            SecureField(placeholder, text: $text)
                .autocorrectionDisabled(true)
                .textInputAutocapitalization(.never)
                .foregroundStyle(Color(uiColor: .label))
                .padding(.bottom, Layout.inputBottomPadding)

            Rectangle()
                .fill(Color(uiColor: .separator))
                .frame(height: Layout.lineHeight)
        }
    }
}

#Preview {
    JoinSessionView(mode: .join)
}
