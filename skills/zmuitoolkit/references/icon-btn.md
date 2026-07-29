# ZMUIToolkitIconBtn

## Description
`ZMUIToolkitIconBtn` is the BASE CLASS for every ZMUIToolkit button — the concrete controls (audio, video, share, participants, more, leave, pause-share) all subclass it. It is an `open` `UIButton` subclass that renders its background through a dedicated `CALayer` (with state-driven coloring and automatic corner rounding) and exposes a rich `@objc` configuration API for size/position/radius, background and icon color, icon image/size/padding, text/title/font/weight, accessibility, and a top-right badge. It is purely a visual/UIKit component — no SDK lifecycle, delegate, or error codes. You can subclass it for custom buttons, but its designated initializer takes an internal descriptor, so external callers normally use the concrete subclasses instead.

## How to use
Prerequisite: the toolkit must be set up (see `references/setup.md`). You typically obtain a concrete subclass instance and then call the inherited `set*` config methods on it.

```swift
// On any concrete toolkit button instance:
button.setSize(size: CGSize(width: 44, height: 44))
button.setIcon(icon: UIImage(named: "Mic")!)
button.setIconColor(color: .label)
button.setBackgroundColor(color: .systemGray6)
button.setRadius(radius: 8)            // -1 restores automatic rounding
button.setIconPadding(padding: 8)

// Icon-with-text:
button.setButtonText("Participants")   // switches to iconWithText layout
button.setTextFontSize(12)
button.setTextColor(.label)
button.setTextSpacing(4)
button.setFontWeightRawValue(0.6)      // semibold (exact value)

// Badge (e.g. participants count):
button.badgeBackgroundColor = .systemRed
button.badgeTextColor = .white
button.badgeText = "12"
button.showBadge = true                // hide with showBadge = false
```

Key public setters, grouped:
- Layout: `setSize(size:)`, `setPosition(x:y:)`, `setRadius(radius:)`, `setIconPadding(padding:)`, `setIconSize(_:)`, `setTextSpacing(_:)`.
- Color / icon: `setBackgroundColor(color:)`, `setIconColor(color:)`, `setIcon(icon:)`.
- Text / font: `setButtonText(_:)`, `setTextColor(_:)`, `setTextFontSize(_:)` (alias `setFontSize(_:)`), `setFontWeight(_:)` (Swift), `setFontWeightRawValue(_:)` (ObjC), and `setAccessibility(label:hint:)`.
- Badge (public `@objc` properties): `showBadge`, `badgeText`, `showBadgeBackground`, `badgeBackgroundColor`, `badgeTextColor`, `badgeWidth`.
- Click handling: override `onButtonClicked()` in a subclass (call `super` to still fire the closure), or use the internal `buttonClickAction` closure set at init.

## Limitations & restrictions
- `setFontWeightRawValue(_:)` accepts these exact values: 0.0 ultraLight, 0.2 thin, 0.3 light, 0.4 regular, 0.5 medium, 0.6 semibold, 0.7 bold, 0.8 heavy, 1.0 black; other values use `.regular`. Use the typed `setFontWeight(_:)` from Swift for arbitrary weights.
- The designated `init(descriptor:)` takes the internal `ZMUIToolKitIconDescriptor` protocol, so directly constructing the base class from outside the framework isn't practical — use a concrete subclass.
- `@MainActor`: construct and use only on the main thread.
- `init?(coder:)` traps with `fatalError` — do not instantiate from Storyboard/XIB.
- Purely visual/UIKit: no SDK calls, no delegate registration, no error codes. Setting `UIView.backgroundColor` directly is intercepted (rendered via `backLayer`); use `setBackgroundColor(color:)`.

## Related components
- `references/setup.md` — prerequisite framework initialization before any button is used.
- `references/audio-btn.md`, `references/video-btn.md`, `references/share-btn.md`, `references/pause-share-btn.md`, `references/participants-btn.md`, `references/more-btn.md`, `references/leave-session-btn.md` — all subclass `ZMUIToolkitIconBtn` and inherit this config API.
- `references/participants-btn.md` — uses the badge API (showBadge/badgeText) for the participant count.
- `references/manager.md` — creates and owns the toolkit buttons.
- `references/composite-delegate.md` — SDK event fan-out that drives the self-managing subclasses.
- `references/root-video-view.md` — host video view the toolbar buttons sit over.
- `references/strings.md` — localized strings used for button titles/accessibility.

## Full API reference
- `docs/api/ZMUIToolkitIconBtn-API.md`
- `docs/api/ZMUIToolkitIconBtn-API.json`
