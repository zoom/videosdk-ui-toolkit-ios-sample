//
//  SessionView.swift
//  ZMUIToolkitSample
//

import SwiftUI
import ZMUIToolkit

struct VideoCanvas: UIViewRepresentable {

    func makeUIView(context: Context) -> ZMUIToolkitRootVideoView {
        let videoCanvas = ZMUIToolkitRootVideoView()
        videoCanvas.isUserInteractionEnabled = true
        return videoCanvas
    }

    func updateUIView(_ uiView: ZMUIToolkitRootVideoView, context: Context) {
    }

    static func dismantleUIView(_ uiView: ZMUIToolkitRootVideoView, coordinator: ()) {
        // Ensure view is removed from window before deallocation
        uiView.removeFromSuperview()
    }
}

struct InSessionView: View {
    var sessionName: String
    @State private var leaveButtonProps = LeaveButtonProperties()
    @State private var areToolbarsVisible = true
    
    private enum Layout {
        static let landscapeToolbarBackgroundOpacity: CGFloat = 0.8
        static let portraitToolbarBackgroundOpacity: CGFloat = 1.0
        static let toolbarAutoHideDelaySeconds: TimeInterval = 5
    }
    
    var body: some View {
        GeometryReader { geometry in
            let isLandscape = geometry.size.width > geometry.size.height
            
            Group {
                if isLandscape {
                    LandscapeView(videoBackground: videoBackground,
                                  size: geometry.size,
                                  sessionName: sessionName,
                                  leaveButtonProperties: $leaveButtonProps,
                                  topBarBackgroundColor: topToolbarBackground(isLandscape: true),
                                  bottomBarBackground: bottomToolbarBackground(isLandscape: true),
                                  shouldShowToolbars: $areToolbarsVisible)
                } else {
                    PortraitView(sessionName: sessionName,
                                 leaveButtonProperties: $leaveButtonProps,
                                 topBarBackgroundColor: topToolbarBackground(isLandscape: false),
                                 videoBackground: videoBackground,
                                 bottomBarBackground: bottomToolbarBackground(isLandscape: false))
                }
            }
            .onAppear {
                areToolbarsVisible = !isLandscape
            }
            .onChange(of: isLandscape) { _, newValue in
                areToolbarsVisible = !newValue
            }
        }
    }
    
    private struct LandscapeView: View {
        let videoBackground: Color
        let size: CGSize
        let sessionName: String
        let leaveButtonProperties: Binding<LeaveButtonProperties>
        let topBarBackgroundColor: Color
        let bottomBarBackground: Color
        @Binding var shouldShowToolbars: Bool
        @State private var autoHideWorkItem: DispatchWorkItem?
        
        var body: some View {
            ZStack {
                videoBackground
                    .ignoresSafeArea()
                
                VideoCanvas()
                    .frame(maxWidth: .infinity, maxHeight: .infinity)
                    .contentShape(Rectangle())
                    .gesture(
                        SpatialTapGesture()
                            .onEnded { value in
                                toggleToolbarsForLandscapeTap(
                                    at: value.location,
                                    in: size
                                )
                            }
                    )
                
                if shouldShowToolbars {
                    VStack(spacing: 0) {
                        TopBarView(
                            sessionName: sessionName,
                            leaveButtonProps: leaveButtonProperties,
                            backgroundColor: topBarBackgroundColor
                        )
                        Spacer()
                        BottomBarView()
                            .background(bottomBarBackground)
                    }
                }
            }
            .background(videoBackground)
            .ignoresSafeArea(edges: [.top, .bottom])
            .onAppear {
                refreshAutoHideTimer()
            }
            .onDisappear {
                cancelAutoHideTimer()
            }
            .onChange(of: shouldShowToolbars) { _, _ in
                refreshAutoHideTimer()
            }
        }
        
        private func toggleToolbarsForLandscapeTap(at location: CGPoint, in size: CGSize) {
            guard CGRect(origin: .zero, size: size).contains(location) else { return }
            withAnimation(.easeInOut(duration: 0.2)) {
                shouldShowToolbars.toggle()
            }
        }
        
        private func refreshAutoHideTimer() {
            cancelAutoHideTimer()
            guard shouldShowToolbars else { return }
            
            let workItem = DispatchWorkItem {
                withAnimation(.easeInOut(duration: 0.2)) {
                    shouldShowToolbars = false
                }
            }
            autoHideWorkItem = workItem
            DispatchQueue.main.asyncAfter(deadline: .now() + Layout.toolbarAutoHideDelaySeconds, execute: workItem)
        }
        
        private func cancelAutoHideTimer() {
            autoHideWorkItem?.cancel()
            autoHideWorkItem = nil
        }
    }
    
    private struct PortraitView: View {
        let sessionName: String
        let leaveButtonProperties: Binding<LeaveButtonProperties>
        let topBarBackgroundColor: Color
        let videoBackground: Color
        let bottomBarBackground: Color
        
        var body: some View {
            VStack(spacing: 0) {
                TopBarView(
                    sessionName: sessionName,
                    leaveButtonProps: leaveButtonProperties,
                    backgroundColor: topBarBackgroundColor
                )
                VideoCanvas()
                    .frame(maxWidth: .infinity, maxHeight: .infinity)
                    .background(videoBackground)
                BottomBarView()
                    .background(bottomBarBackground)
            }
            .background(videoBackground.ignoresSafeArea())
        }
    }
    
    private func bottomToolbarBackground(isLandscape: Bool) -> Color {
        Color(uiColor: .systemBackground)
            .opacity(
                isLandscape
                ? Layout.landscapeToolbarBackgroundOpacity
                : Layout.portraitToolbarBackgroundOpacity
            )
    }
    
    private func topToolbarBackground(isLandscape: Bool) -> Color {
        Color(uiColor: .secondarySystemBackground)
            .opacity(
                isLandscape
                ? Layout.landscapeToolbarBackgroundOpacity
                : Layout.portraitToolbarBackgroundOpacity
            )
    }
    
    private var videoBackground: Color {
        Color(uiColor: .secondarySystemBackground)
    }
}

#Preview {
    InSessionView(sessionName:"Test Session Name")
}
