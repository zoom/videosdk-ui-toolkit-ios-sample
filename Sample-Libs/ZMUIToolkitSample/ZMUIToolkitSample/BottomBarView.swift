//
//  BottomBarView.swift
//  ZMUIToolkitSample
//

import SwiftUI
import ZMUIToolkit

struct BottomBarView: View {
    private enum Layout {
        static let horizontalPadding: CGFloat = 10
        static let verticalPadding: CGFloat = 10
    }
    
    @State private var audioButtonProps = AudioButtonProperties()
    @State private var videoButtonProps = VideoButtonProperties()
    @State private var shareButtonProps = ShareButtonProperties()
    @State private var participantsButtonProps = ParticipantsButtonProperties()
    @State private var moreButtonProps = MoreButtonProperties()
    
    var body: some View {
        HStack {
            Spacer()
            VStack {
                AudioButtonView(props: $audioButtonProps)
                    .fixedSize()
                Text("Mic")
                    .font(.caption)
            }
            Spacer()
            VStack {
                VideoButtonView(props: $videoButtonProps)
                    .fixedSize()
                Text("Camera")
                    .font(.caption)
            }
            Spacer()
            VStack {
                ShareButtonView(props:$shareButtonProps)
                    .fixedSize()
                Text("Share Screen")
                    .font(.caption)
            }
            Spacer()
            VStack {
                ParticipantsButtonView(props: $participantsButtonProps)
                    .fixedSize()
                Text("Participants")
                    .font(.caption)
            }
            Spacer()
            VStack {
                MoreButtonView(props: $moreButtonProps)
                    .fixedSize()
                Text("More")
                    .font(.caption)
            }
            Spacer()
        }
        .padding(.horizontal, Layout.horizontalPadding)
        .padding(.vertical, Layout.verticalPadding)
        .frame(maxWidth: .infinity)
    }
}

#Preview {
    BottomBarView()
}

struct AudioButtonProperties {
    public var backgroundColor = Color.clear
    public var radius: CGFloat = 8.0
    public var size: CGSize = CGSize(width: 40, height: 40)
    public var iconColor = Color.gray
    public var iconSize: CGSize = CGSize(width: 24, height: 24)
    public var iconPadding: CGFloat = 8.0
    public var title: String = ""
    public var textColor = Color.white
    public var fontSize: CGFloat = 14.0
    public var textSpacing: CGFloat = 4.0
}

struct AudioButtonView: UIViewRepresentable {
    @Binding var props: AudioButtonProperties
    
    func makeUIView(context: Context) -> ZMUIToolkitAudioBtn {
        let button = ZMUIToolkitAudioBtn()
        button.isUserInteractionEnabled = true
        return button
    }
    
    func updateUIView(_ uiView: ZMUIToolkitAudioBtn, context: Context) {
        uiView.setBackgroundColor(color: UIColor(props.backgroundColor))
        uiView.setRadius(radius: props.radius)
        uiView.setSize(size: props.size)
        uiView.setIconColor(color: UIColor(props.iconColor))
        uiView.setIconSize(props.iconSize)
        uiView.setIconPadding(padding: props.iconPadding)
        uiView.setTitle(props.title.isEmpty ? nil : props.title, for: .normal)
        uiView.setTextSpacing(props.textSpacing)
        uiView.setTextColor(UIColor(props.textColor))
        uiView.setTextFontSize(props.fontSize)
    }
}

struct VideoButtonProperties {
    public var backgroundColor = Color.clear
    public var radius: CGFloat = 8.0
    public var size: CGSize = CGSize(width: 40, height: 40)
    public var iconSize: CGSize = CGSize(width: 24, height: 24)
    public var iconColor = Color.gray
    public var iconPadding: CGFloat = 8.0
    public var title: String = ""
    public var textColor = Color.white
    public var fontSize: CGFloat = 14.0
    public var textSpacing: CGFloat = 4.0
}

struct VideoButtonView: UIViewRepresentable {
    @Binding var props:VideoButtonProperties
    
    func makeUIView(context: Context) -> ZMUIToolkitVideoBtn {
        let button = ZMUIToolkitVideoBtn()
        button.isUserInteractionEnabled = true
        return button
    }
    
    func updateUIView(_ uiView: ZMUIToolkitVideoBtn, context: Context) {
        uiView.setBackgroundColor(color: UIColor(props.backgroundColor))
        uiView.setRadius(radius: props.radius)
        uiView.setSize(size: props.size)
        uiView.setIconSize(props.iconSize)
        uiView.setIconColor(color: UIColor(props.iconColor))
        uiView.setIconPadding(padding: props.iconPadding)
        uiView.setTitle(props.title.isEmpty ? nil : props.title, for: .normal)
        uiView.setTextSpacing(props.textSpacing)
        uiView.setTextColor(UIColor(props.textColor))
        uiView.setTextFontSize(props.fontSize)
    }
}

struct ShareButtonProperties {
    public var backgroundColor = Color.clear
    public var radius: CGFloat = 8.0
    public var size: CGSize = CGSize(width: 40, height: 40)
    public var iconSize: CGSize = CGSize(width: 24, height: 24)
    public var iconColor = Color.gray
    public var iconPadding: CGFloat = 8.0
    public var title: String = ""
    public var textColor = Color.white
    public var fontSize: CGFloat = 14.0
    public var textSpacing: CGFloat = 4.0
}

struct ShareButtonView: UIViewRepresentable {
    @Binding var props:ShareButtonProperties
    
    func makeUIView(context: Context) -> ZMUIToolkitShareBtn {
        let button = ZMUIToolkitShareBtn()
        button.isUserInteractionEnabled = true
        return button
    }
    
    func updateUIView(_ uiView: ZMUIToolkitShareBtn, context: Context) {
        uiView.setBackgroundColor(color: UIColor( props.backgroundColor))
        uiView.setRadius(radius: props.radius)
        uiView.setSize(size: props.size)
        uiView.setIconColor(color: UIColor(props.iconColor))
        uiView.setIconSize(props.iconSize)
        uiView.setIconPadding(padding: props.iconPadding)
        uiView.setTitle(props.title.isEmpty ? nil : props.title, for: .normal)
        uiView.setTextSpacing(props.textSpacing)
        uiView.setTextColor(UIColor(props.textColor))
        uiView.setTextFontSize(props.fontSize)
    }
}

struct ParticipantsButtonProperties {
    public var backgroundColor = Color.clear
    public var radius: CGFloat = 8.0
    public var size: CGSize = CGSize(width: 40, height: 40)
    public var iconSize: CGSize = CGSize(width: 24, height: 24)
    public var iconColor = Color.gray
    public var iconPadding: CGFloat = 8.0
    public var title: String = ""
    public var textColor = Color.white
    public var fontSize: CGFloat = 14.0
    public var textSpacing: CGFloat = 4.0
}

struct ParticipantsButtonView: UIViewRepresentable {
    @Binding var props: ParticipantsButtonProperties

    func makeUIView(context: Context) -> ZMUIToolkitParticipantsBtn {
        let button = ZMUIToolkitParticipantsBtn()
        button.isUserInteractionEnabled = true
        return button
    }

    func updateUIView(_ uiView: ZMUIToolkitParticipantsBtn, context: Context) {
        uiView.setBackgroundColor(color: UIColor(props.backgroundColor))
        uiView.setRadius(radius: props.radius)
        uiView.setSize(size: props.size)
        uiView.setIconColor(color: UIColor(props.iconColor))
        uiView.setIconSize(props.iconSize)
        uiView.setIconPadding(padding: props.iconPadding)
        uiView.setTitle(props.title.isEmpty ? nil : props.title, for: .normal)
        uiView.setTextSpacing(props.textSpacing)
        uiView.setTextColor(UIColor(props.textColor))
        uiView.setTextFontSize(props.fontSize)
    }
}

struct MoreButtonProperties {
    public var backgroundColor = Color.clear
    public var radius: CGFloat = 8.0
    public var size: CGSize = CGSize(width: 40, height: 40)
    public var iconSize: CGSize = CGSize(width: 24, height: 24)
    public var iconColor = Color.gray
    public var iconPadding: CGFloat = 8.0
    public var title: String = ""
    public var textColor = Color.white
    public var fontSize: CGFloat = 14.0
    public var textSpacing: CGFloat = 4.0
}

struct MoreButtonView: UIViewRepresentable {
    @Binding var props: MoreButtonProperties

    func makeUIView(context: Context) -> ZMUIToolkitMoreBtn {
        let button = ZMUIToolkitMoreBtn()
        button.isUserInteractionEnabled = true
        return button
    }

    func updateUIView(_ uiView: ZMUIToolkitMoreBtn, context: Context) {
        uiView.setBackgroundColor(color: UIColor(props.backgroundColor))
        uiView.setRadius(radius: props.radius)
        uiView.setSize(size: props.size)
        uiView.setIconColor(color: UIColor(props.iconColor))
        uiView.setIconSize(props.iconSize)
        uiView.setIconPadding(padding: props.iconPadding)
        uiView.setTitle(props.title.isEmpty ? nil : props.title, for: .normal)
        uiView.setTextSpacing(props.textSpacing)
        uiView.setTextColor(UIColor(props.textColor))
        uiView.setTextFontSize(props.fontSize)
    }
}
