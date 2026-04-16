//
//  StartingPageView.swift
//  ZMUIToolkitSample
//

import SwiftUI
import ZMUIToolkit

struct StartingPageView: View {
    private let slides: [String] = (1...6).map { "intro_image\($0)" }
    @State private var sdkInitialized: Bool = ZMUIToolkitSampleConfig.isSDKInitialized

    private enum Layout {
        static let whiteCardHeightPercent: CGFloat = 0.3
        static let buttonMinHeight: CGFloat = 20
        static let buttonHeightPercent: CGFloat = 0.042
        static let buttonWidthPercent: CGFloat = 0.6
        static let buttonRadius: CGFloat = 6
        static let buttonBorderWidth: CGFloat = 1.5
        static let buttonSpacingPercent: CGFloat = 0.5
        static let buttonContainerBottomPadding: CGFloat = 8
    }

    var body: some View {
        NavigationStack {
            GeometryReader { geometry in
                ZStack(alignment: .center) {
                    
                    Image("intro_bg")
                        .resizable()
                        .frame(maxWidth: .infinity, maxHeight: .infinity)
                    
                    SlideShowBackground(slides: slides)
                        .frame(maxWidth: .infinity)
                        .frame(height: geometry.size.height)
                        .safeAreaPadding(.all)
                    
                    VStack (alignment: .center) {
                        Spacer()
                        ZStack {
                            WhiteCardWave()
                                .fill(Color.white)
                                .frame(maxWidth: .infinity)
                            
                            let spacing = max(geometry.size.height * Layout.buttonHeightPercent, Layout.buttonMinHeight) * Layout.buttonSpacingPercent
                            VStack(spacing: spacing) {
                                JoinSessionButton(mode: .create,
                                                  text: "Create",
                                                  foregroundStyle: .white,
                                                  width: geometry.size.width * Layout.buttonWidthPercent,
                                                  height: geometry.size.height * Layout.buttonHeightPercent,
                                                  minimumButtonHeight: Layout.buttonMinHeight,
                                                  backgroundColor: Color.blue,
                                                  buttonRadius: Layout.buttonRadius,
                                                  sdkInitialized: sdkInitialized)
                                
                                JoinSessionButton(mode: .join,
                                                  text: "Join",
                                                  foregroundStyle: .blue,
                                                  width: geometry.size.width * Layout.buttonWidthPercent,
                                                  height: geometry.size.height * Layout.buttonHeightPercent,
                                                  minimumButtonHeight: Layout.buttonMinHeight,
                                                  backgroundColor: Color.white,
                                                  buttonRadius: Layout.buttonRadius,
                                                  sdkInitialized: sdkInitialized
                                )
                                .overlay(
                                    RoundedRectangle(cornerRadius: Layout.buttonRadius)
                                        .stroke(Color.blue, lineWidth: Layout.buttonBorderWidth)
                                )
                            }
                            .offset(y: -spacing) // Shift the view up, so we aren't too low
                        }
                        .frame(height: geometry.size.height * Layout.whiteCardHeightPercent)
                    }
                }
            }
            .ignoresSafeArea()
            .toolbar(.hidden, for: .navigationBar)
            .onAppear {
                initializeToolkitIfNeeded()
            }
        }
    }

    private func initializeToolkitIfNeeded() {
        if ZMUIToolkitSampleConfig.isSDKInitialized {
            sdkInitialized = true
            return
        }

        let sdkinit = ZMUIToolkitVideoSDKInitializer()
        sdkinit.domain = ZMUIToolkitSampleConfig.sdkDomain
        sdkinit.enableLog = true
        sdkinit.logFilePrefix = ZMUIToolkitSampleConfig.logFilePrefix
        sdkinit.appGroupId = ZMUIToolkitSampleConfig.screenShareAppGroupId
        sdkinit.bundleId = ZMUIToolkitSampleConfig.screenShareExtensionBundleId

        let error = ZMUIToolKitManager.initialize(initializer: sdkinit)
        if error == .success {
            ZMUIToolkitSampleConfig.isSDKInitialized = true
            sdkInitialized = true
        } else {
            print("[SDKInit] ERROR: SDK initialization failed with error: \(error)")
            print("[SDKInit] Error details:")
            switch error {
            case .load_Module_Error:
                print("[SDKInit]   - Could not load SDK module")
            case .auth_Error, .auth_Empty_Key_or_Secret, .auth_Wrong_Key_or_Secret:
                print("[SDKInit]   - Authentication error - check SDK key/secret")
            case .auth_DoesNot_Support_SDK, .auth_Disable_SDK:
                print("[SDKInit]   - SDK not supported or disabled for this account")
            case .invalid_Parameter:
                print("[SDKInit]   - Invalid initialization parameters")
            case .memory_Error:
                print("[SDKInit]   - Memory allocation error")
            case .internal_Error:
                print("[SDKInit]   - Internal SDK error")
            default:
                print("[SDKInit]   - Unhandled error type")
            }
            sdkInitialized = false
        }
    }
}

private struct SlideShowBackground: View {
    let slides: [String]

    var body: some View {
        TabView {
            ForEach(slides, id: \.self) { slideName in
                Image(slideName)
                    .resizable()
                    .scaledToFit()
                    .frame(maxWidth: .infinity, maxHeight: .infinity, alignment: .center)
            }
        }
        .tabViewStyle(.page(indexDisplayMode: .always))
        .indexViewStyle(.page(backgroundDisplayMode: .never))
    }
}

private struct WhiteCardWave: Shape {
    private let whiteCardWavePercent: CGFloat = 0.139
    func path(in rect: CGRect) -> Path {
        var path = Path()
        path.move(to: .zero)
        path.addQuadCurve(
            to: CGPoint(x: rect.maxX, y: 0),
            control: CGPoint(x: rect.midX, y: rect.height * whiteCardWavePercent)
        )
        path.addLine(to: CGPoint(x: rect.maxX, y: rect.maxY))
        path.addLine(to: CGPoint(x: 0, y: rect.maxY))
        path.closeSubpath()
        return path
    }
}

private struct JoinSessionButton: View {
    let mode: JoinSessionView.Mode
    let text: String
    let foregroundStyle: Color
    let width: CGFloat
    let height: CGFloat
    let minimumButtonHeight: CGFloat
    let backgroundColor: Color
    let buttonRadius: CGFloat
    let sdkInitialized: Bool
    
    var body: some View {
        NavigationLink {
            JoinSessionView(mode: mode)
        } label: {
            Text(text)
                .font(.headline)
                .foregroundStyle(foregroundStyle)
                .frame(
                    width: width,
                    height: height
                )
                .frame(minHeight: minimumButtonHeight)
                .background(backgroundColor)
                .clipShape(RoundedRectangle(cornerRadius: buttonRadius))
        }
        .disabled(!sdkInitialized)
    }
}

#Preview {
    StartingPageView()
}
