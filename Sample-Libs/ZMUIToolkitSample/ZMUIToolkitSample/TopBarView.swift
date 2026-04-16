//
//  TopBarView.swift
//  ZMUIToolkitSample
//

import SwiftUI
import ZMUIToolkit

struct LeaveButtonProperties {
    public var backgroundColor = Color.red
    public var radius: CGFloat = 8.0
    public var size: CGSize = CGSize(width: 40, height: 40)
    public var iconColor = Color.white
    public var iconSize: CGSize = CGSize(width: 24, height: 24)
    public var iconPadding: CGFloat = 8.0
}

struct LeaveSessionButtonView: UIViewRepresentable {
    @Binding var props: LeaveButtonProperties
    
    func makeUIView(context: Context) -> ZMUIToolkitLeaveSessionBtn {
        let button = ZMUIToolkitLeaveSessionBtn()
        button.isUserInteractionEnabled = true
        return button
    }
    
    func updateUIView(_ uiView: ZMUIToolkitLeaveSessionBtn, context: Context) {
        uiView.setBackgroundColor(color: UIColor(props.backgroundColor))
        uiView.setRadius(radius: props.radius)
        uiView.setSize(size: props.size)
        uiView.setIconSize(props.iconSize)
        uiView.setIconColor(color: UIColor(props.iconColor))
        uiView.setIconPadding(padding: props.iconPadding)
    }
}

struct TopBarView: View {
    private enum Layout {
        static let horizontalPadding: CGFloat = 10
    }
    
    let sessionName: String
    @Binding var leaveButtonProps: LeaveButtonProperties
    let backgroundColor: Color
    
    var body: some View {
        ZStack {
            Text(sessionName)
                .frame(maxWidth: .infinity)
                .multilineTextAlignment(.center)
            
            HStack {
                Spacer()
                LeaveSessionButtonView(props: $leaveButtonProps)
                    .fixedSize()
            }
            .padding(.horizontal, Layout.horizontalPadding)
        }
        .padding(.vertical, 12)
        .background(backgroundColor)
    }
}

#Preview {
    TopBarView(
        sessionName: "Test Session Name",
        leaveButtonProps: .constant(LeaveButtonProperties()),
        backgroundColor: Color(uiColor: .secondarySystemBackground)
    )
}
