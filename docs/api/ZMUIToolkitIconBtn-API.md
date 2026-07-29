# ZMUIToolkitIconBtn API Documentation

## Module Information
- **Module**: ZMUIToolkitIconBtn
- **Platform**: iOS
- **Language**: Swift (with Objective-C interop via `@objc`)
- **Version**: 1.0
- **Framework**: ZMUIToolkit
- **Superclass**: `UIButton`
- **Concurrency**: `@MainActor` — all members must be accessed on the main thread.
- **Objective-C name**: `ZMUIToolkitIconBtn` (exposed via `@objc(ZMUIToolkitIconBtn)`).
- **Class kind**: `open class` — subclassable from other modules.

## Overview

`ZMUIToolkitIconBtn` is the **base class for all toolkit buttons** in ZMUIToolkit (e.g. `ZMUIToolkitAudioBtn`, `ZMUIToolkitVideoBtn`, participants button, etc.). It is a `UIButton` subclass that provides:

1. **Custom background rendering** through a dedicated `CALayer` (`backLayer`) inserted below the content, with automatic corner rounding (circular for small buttons, lightly rounded for larger ones), and state-driven coloring (normal / selected / pressed / disabled).
2. **Icon + optional text layout** — centers an icon, or stacks an icon above a single-line title with configurable spacing.
3. **A rich set of `@objc` configuration setters** for size, position, corner radius, background color, icon image/color/size, icon padding, text/title, text color, font size, font weight, accessibility, and a badge (count/dot indicator).
4. **A simple click dispatch model**: tapping invokes `onButtonClicked()`, which calls the `buttonClickAction` closure. Subclasses override `onButtonClicked()` to add behavior.

This class is **not a session/SDK-aware control** by itself (unlike its subclasses). It has **no SDK lifecycle, no delegate registration, and no error codes** — it is a pure UIKit view/configuration component. The "state machine" below is therefore purely a *visual* state machine driven by `UIControl` state and touch events, not a feature/protocol state machine.

## Lifecycle

### Prerequisites
- Must be constructed and used on the **main thread** (`@MainActor`).
- The **designated initializer takes a descriptor** (`ZMUIToolKitIconDescriptor`), which is an **internal** protocol. External callers therefore generally construct a *subclass* (which builds the descriptor internally) rather than instantiating `ZMUIToolkitIconBtn` directly. Subclasses configure colors, icon, padding, accessibility label, and the click callback from the descriptor.

### Entry / Exit
- **Construction** (`init(descriptor:)`): loads the named image from the ZMUIToolkit bundle, computes the button size (descriptor frame size, or 40×40 if zero), then runs `finishInit(image:size:)`:
  - sets the stored button size,
  - sets the normal-state image (if any),
  - centers content vertically/horizontally,
  - wires `touchUpInside` → `handleTouchUpInside` → `onButtonClicked()`,
  - adds the (initially hidden) badge subview,
  - registers a trait-change handler for light/dark mode (iOS 17+),
  - applies the descriptor's background color, accessibility label, icon padding, icon color, and click callback.
- **`init?(coder:)`**: **Not supported** — calls `fatalError`. Do **not** instantiate from a Storyboard/XIB.
- **Destruction**: No custom `deinit`. There is no SDK/delegate cleanup to perform.

### Rendering / layout entry points
- `layoutSubviews()` is the central layout pass. On first run it inserts `backLayer` at index 0; on every run it sizes `backLayer` to `bounds`, computes the corner radius, lays out the icon (and title if present), refreshes badge appearance, and lays out the badge. If `bounds` is degenerate (width or height 0) it self-assigns a size from `buttonSize` first.
- `intrinsicContentSize` returns `buttonSize`, so the button reports its configured size to Auto Layout / stack views.

## State Machine

> This is a **visual** state machine over `UIControl` state + touch phase. It is **not** an SDK/feature state machine. There are no async transitions, no callbacks that change feature state, and no error codes.

### Visual states (driven by `isEnabled`, touch phase)

| State | Trigger | `backLayer` background color |
|-------|---------|------------------------------|
| Normal (enabled, untouched) | default; restore after touch | `_customBackgroundColor` if set, otherwise `UIColor.systemGray6` |
| Selected (enabled) | `isSelected == true` | slightly emphasized normal color; if the normal color is fully transparent, falls back to `UIColor.systemGray5` |
| Pressed (enabled, finger down) | `touchesBegan` | `UIColor.systemGray5` |
| Disabled | `isEnabled == false` | `UIColor.systemGray4` |

### Visual transitions

| From | Trigger | To | Notes |
|------|---------|-----|-------|
| Normal | `touchesBegan` (enabled only) | Pressed (`systemGray5`) | Guarded by `isEnabled`; color change is non-animated (`CATransaction` disables actions). |
| Pressed | `touchesEnded` / `touchesCancelled` (enabled only) | Normal (after delay) | Restoration is scheduled `0.1s` later via `DispatchQueue.main.asyncAfter`, calling `updateBackgroundColor()`. |
| any | `isSelected` set | Normal or Selected | `didSet` on `isSelected` calls `updateBackgroundColor()`. |
| any | `isEnabled` set | Normal or Disabled | `didSet` on `isEnabled` calls `updateBackgroundColor()`. |
| any | trait change (light/dark) | (recolored) | iOS 17+ uses `registerForTraitChanges`; below 17 uses `traitCollectionDidChange`. Both call `updateBackgroundColor()` when `userInterfaceStyle` changes. |

### Icon layout modes (`IconLayoutMode`, internal)

| Mode | Condition | Layout |
|------|-----------|--------|
| `iconOnly` | normal-state title is empty/nil | Icon centered in the content rect (after content insets). |
| `iconWithText` | normal-state title is non-empty | Icon stacked above a single-line, center-aligned, tail-truncated title, separated by `textSpacing`; the whole group is vertically centered. |

The mode is recomputed whenever the title changes (via `setTitle`/`setAttributedTitle` for `.normal`, or `setButtonText`).

### Corner radius rule (`layoutSubviews`)
```
customCornerRadius >= 0           -> backLayer.cornerRadius = customCornerRadius
else if max(width,height) <= 32   -> cornerRadius = backLayer.height / 2   (fully circular)
else                              -> cornerRadius = backLayer.height / 4   (rounded)
```
`layer.cornerRadius` is then set to match `backLayer.cornerRadius`, and `clipsToBounds = true`.

## APIs

> All setters below are `@MainActor`. Most call `setNeedsLayout()` (directly or indirectly) so changes take effect on the next layout pass. None return error codes.

### `init(descriptor:)` *(internal protocol parameter)*
```swift
init(descriptor: ZMUIToolKitIconDescriptor)
```
Designated initializer. Loads the icon asset named `descriptor.iconname` from the ZMUIToolkit bundle, sizes the button (descriptor frame size, or 40×40 if the frame size is `.zero`), then applies background color, accessibility label, icon padding, icon color, and click callback from the descriptor.
- **Access note**: `ZMUIToolKitIconDescriptor` is an **internal** protocol, so this initializer is effectively only usable within the framework / by subclasses that build the descriptor internally.
- **Preconditions**: Main thread.
- **Side effects**: Adds a hidden badge subview; registers a trait-change handler; wires the `touchUpInside` action.

### `init?(coder:)`
```swift
@MainActor required public init?(coder: NSCoder)
```
- **Not supported.** Calls `fatalError("init(coder:) has not been implemented")`. Do **not** instantiate from a Storyboard/XIB.

### `onButtonClicked()`
```swift
@objc public func onButtonClicked()
```
Invoked on `touchUpInside`. The base implementation calls `buttonClickAction(self)`. **Override this in subclasses** to perform a button-specific action. Calling `super.onButtonClicked()` will still fire the closure.
- **Side effects**: Invokes the `buttonClickAction` closure with `self`.

### `setBackgroundColor(color:)`
```swift
@objc public func setBackgroundColor(color: UIColor)
```
Sets the normal-state background color. The color is stored in `_customBackgroundColor` and rendered through `backLayer` (the `UIView.backgroundColor` getter returns this custom color; the setter forces the real `UIView` background to `nil`).
- **Parameters**: `color` — the normal-state background color.
- **Note**: When disabled the button shows `systemGray4`; when pressed it shows `systemGray5`; this color applies only in the normal/enabled state. If never set, normal state falls back to `systemGray6`.

### `setRadius(radius:)`
```swift
@objc public func setRadius(radius: CGFloat)
```
Sets an explicit corner radius (stored in `customCornerRadius`). Passing a value `>= 0` overrides the automatic circular/rounded behavior. Passing `-1` (the default sentinel) restores automatic rounding.
- **Parameters**: `radius` — corner radius in points, or `-1` for automatic.
- **Side effects**: Triggers layout (`setNeedsLayout`). `customCornerRadius`'s `didSet` also calls `setNeedsLayout` when the value changes.

### `setSize(size:)`
```swift
@objc public func setSize(size: CGSize)
```
Sets the button's width and height (stored in `buttonSize`). Non-finite values fall back to `40`; each dimension is clamped to a minimum of `1`. Updates `intrinsicContentSize` and triggers layout.
- **Parameters**: `size` — desired button size.
- **Side effects**: `invalidateIntrinsicContentSize()`, `setNeedsLayout()`.

### `setPosition(x:y:)`
```swift
@objc public func setPosition(x: CGFloat, y: CGFloat)
```
Sets `frame.origin` to `(x, y)`. Useful only with manual (non-Auto-Layout) frame placement.
- **Parameters**: `x`, `y` — top-left origin in the superview's coordinate space.

### `setIconColor(color:)`
```swift
@objc public func setIconColor(color: UIColor)
```
Sets the icon tint. Assigns both `tintColor` and `imageView?.tintColor`. Only affects template (tintable) images.
- **Parameters**: `color` — tint color for the icon.

### `setIcon(icon:)`
```swift
@objc public func setIcon(icon: UIImage)
```
Sets the normal-state image. Re-applies the current `tintColor` to the image view, then calls `setImage(_:for: .normal)` (which triggers layout).
- **Parameters**: `icon` — the image to display.

### `setIconSize(_:)`
```swift
@objc public func setIconSize(_ size: CGSize)
```
Sets a custom icon (image) size used during layout. Non-finite values are treated as `0`; each dimension is clamped to a minimum of `0`. If **both** width and height are `> 0`, the custom size is applied (and the image is constrained to fit the content rect); otherwise (`.zero` or any non-positive dimension) the custom size is cleared and the icon falls back to its intrinsic image size.
- **Parameters**: `size` — desired icon size, or `.zero` to use intrinsic size.
- **Side effects**: `setNeedsLayout()`.

### `setIconPadding(padding:)`
```swift
@objc public func setIconPadding(padding: CGFloat)
```
Sets uniform content insets (top/leading/bottom/trailing) around the icon/title content. Negative values are clamped to `0`.
- **Parameters**: `padding` — uniform inset in points (>= 0).
- **Side effects**: Applies `NSDirectionalEdgeInsets` via `setIconContentInsets`, which calls `setNeedsLayout()`.

### `setAccessibility(label:hint:)`
```swift
@objc public func setAccessibility(label: String?, hint: String?)
```
Sets `accessibilityLabel` and `accessibilityHint`.
- **Parameters**: `label` — VoiceOver label; `hint` — VoiceOver hint.

### `setButtonText(_:)`
```swift
@objc public func setButtonText(_ title: String?)
```
Sets the normal-state title. A non-empty title switches the layout to `iconWithText` (icon above a single-line, center-aligned, tail-truncated title separated by `textSpacing`); an empty/nil title switches back to `iconOnly`.
- **Parameters**: `title` — text to display below the icon, or nil/empty to remove text.
- **Side effects**: `setTitle(_:for: .normal)` → `titleStateDidChange()` → recomputes layout mode and `setNeedsLayout()`.

### `setTextColor(_:)`
```swift
@objc public func setTextColor(_ color: UIColor)
```
Sets the title color for **all** control states (`.normal`, `.highlighted`, `.selected`, `.disabled`).
- **Parameters**: `color` — title text color.

### `setTextFontSize(_:)`
```swift
@objc public func setTextFontSize(_ fontSize: CGFloat)
```
Sets the title font size. Stores the value (so it is re-applied during layout) and immediately sets `titleLabel.font` to `UIFont.systemFont(ofSize:)`.
- **Parameters**: `fontSize` — point size for the title.
- **Side effects**: `setNeedsLayout()`. The stored size is re-applied every `layoutSubviews` via `layoutTitleIfNeeded()`.

### `setTextSpacing(_:)`
```swift
@objc public func setTextSpacing(_ spacing: CGFloat)
```
Sets the vertical spacing between the icon and the title (assigns `textSpacing`). Non-finite values become `0`; negative values are clamped to `0`.
- **Parameters**: `spacing` — gap in points between icon and title.
- **Side effects**: `textSpacing`'s `didSet` updates the title configuration and calls `setNeedsLayout()`.

### `setFontSize(_:)`
```swift
@objc public func setFontSize(_ fontSize: CGFloat)
```
Alias for `setTextFontSize(_:)`. Provided as an alternative name; behavior is identical.
- **Parameters**: `fontSize` — point size for the title.

### `setFontWeight(_:)` *(Swift only)*
```swift
public func setFontWeight(_ weight: UIFont.Weight)
```
Sets the title font weight, preserving the current point size (defaults to `17.0` if no font is set). Swift-only (not `@objc`) because `UIFont.Weight` is not Objective-C representable.
- **Parameters**: `weight` — a `UIFont.Weight`.
- **Side effects**: `setNeedsLayout()`.

### `setFontWeightRawValue(_:)` *(Objective-C compatible)*
```swift
@objc public func setFontWeightRawValue(_ weightValue: CGFloat)
```
Objective-C-friendly font-weight setter. Maps specific raw `CGFloat` values to `UIFont.Weight` and delegates to `setFontWeight(_:)`:

| `weightValue` | Mapped weight |
|---------------|---------------|
| 0.0 | ultraLight |
| 0.2 | thin |
| 0.3 | light |
| 0.4 | regular |
| 0.5 | medium |
| 0.6 | semibold |
| 0.7 | bold |
| 0.8 | heavy |
| 1.0 | black |
| other values | regular |

Accepts these exact values: 0.0 ultraLight, 0.2 thin, 0.3 light, 0.4 regular, 0.5 medium, 0.6 semibold, 0.7 bold, 0.8 heavy, 1.0 black; other values use `.regular`.

- **Parameters**: `weightValue` — one of the discrete values above. Other values map to `regular`. For arbitrary weights, use the typed `setFontWeight(_:)` from Swift.

### Badge configuration (public `@objc` properties)

> The badge is an internal `Badge` (UILabel subclass) rendered in the top-right corner. These properties are public because subclasses such as the participants button use them. Default badge background is red (`#DA1639`) with white text.

| Property | Type | Default | Effect |
|----------|------|---------|--------|
| `showBadge` | `Bool` | `false` | Shows/hides the badge (`badge.isHidden = !showBadge`). |
| `badgeText` | `String?` | `nil` | Badge text. Setting an empty/nil string hides the badge (per `Badge.text` behavior). Triggers layout. |
| `showBadgeBackground` | `Bool` | `true` | When `true`, badge draws a rounded colored pill with default text inset `(2,0)` and an offset; when `false`, the badge is transparent with zero corner radius/insets and sits flush at the top-right. |
| `badgeBackgroundColor` | `UIColor?` | `nil` | Overrides badge fill color (only applied while `showBadgeBackground` is `true`). |
| `badgeTextColor` | `UIColor?` | `nil` | Overrides badge text color. |
| `badgeWidth` | `CGFloat` | `0` | Forces an explicit badge width when non-zero; also scales the corner offset proportionally (`3 * badgeWidth / 12`). `0` means use the badge's intrinsic width and the default `3.0` offset. |

All badge setters trigger an appearance update and/or layout. The badge is laid out at `x = bounds.width - width + offset`, `y = -offset` (when `showBadgeBackground`) so it overhangs the top-right corner.

### Overridden `UIKit` members (behavioral)
- `var backgroundColor: UIColor?` — getter returns `_customBackgroundColor`; setter forces real background to `nil` and stores/renders through `backLayer`.
- `var isEnabled` — `didSet` calls `updateBackgroundColor()`.
- `var isHighlighted` — `didSet` forces `imageView?.alpha = 1.0` (prevents UIKit's default highlight dimming of the icon).
- `setImage(_:for:)` — calls super then `setNeedsLayout()`.
- `setTitle(_:for:)` / `setAttributedTitle(_:for:)` — call super; for `.normal` state also run `titleStateDidChange()` to recompute icon/text layout mode.
- `intrinsicContentSize` — returns `buttonSize`.
- `layoutSubviews()` — see Lifecycle / corner-radius rule above.
- `traitCollectionDidChange(_:)` — pre-iOS-17 dark/light recolor path (deprecated on iOS 17+).

## Callbacks

`ZMUIToolkitIconBtn` defines **no protocol/delegate callbacks**. Its only event hook is the click pipeline:

### Click pipeline
```
touchUpInside -> @objc handleTouchUpInside() -> onButtonClicked() -> buttonClickAction(self)
```
- `buttonClickAction` is a stored closure `(ZMUIToolkitIconBtn) -> Void`, default `{ _ in }` (does nothing). It is set from the descriptor at init time, or assignable directly (internal access).
- Subclasses typically **override `onButtonClicked()`** rather than replacing the closure.
- **Threading**: main thread (UIKit touch delivery).

## Input Values

This class performs **no SDK calls and returns no error codes**. The setters clamp and normalize their inputs to well-defined ranges:

| Setter | Value handling |
|--------|----------------|
| `setSize` / `buttonSize` | Non-finite values use `40`; each dimension is clamped to a minimum of `1`. |
| `setIconSize` | Non-finite values use `0`; clamped to a minimum of `0`; falls back to intrinsic size unless both dimensions are `> 0`. |
| `setIconPadding` | Negative values use `0`. |
| `setTextSpacing` / `textSpacing` | Non-finite values use `0`; negative values use `0`. |
| `setFontWeightRawValue` | Values other than the listed ones use `regular`. |
| `layoutSubviews` | A `bounds` with 0 width or height is reset to `buttonSize` (minimum 1). |

## Rules

- **REQUIRED**: Construct and use on the main thread (`@MainActor`).
- **REQUIRED**: Subclasses overriding `onButtonClicked()` should call `super.onButtonClicked()` if they still want the `buttonClickAction` closure to fire.
- **FORBIDDEN**: Instantiating via `init?(coder:)` / Interface Builder (traps with `fatalError`).
- **DO NOT** set `UIView.backgroundColor` expecting a real view background — it is intercepted and rendered through `backLayer`; the real layer background is forced to `nil`.
- **PREFER** the `set*` configuration methods over directly mutating UIKit properties so that layout invalidation and the `backLayer` rendering path stay consistent.
- **NOTE** `setFontWeightRawValue` accepts the exact discrete values listed (0.0, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0); other values use `regular`. Use the typed `setFontWeight(_:)` from Swift for arbitrary weights.

## Examples

### Example 1: Subclass and override the click behavior
```swift
final class MyToolbarButton: ZMUIToolkitIconBtn {
    override func onButtonClicked() {
        super.onButtonClicked()      // still fires buttonClickAction if set
        // custom behavior here
        print("tapped")
    }
}
```

### Example 2: Configure size, icon, color, and corner radius
```swift
button.setSize(size: CGSize(width: 44, height: 44))
button.setIcon(icon: UIImage(named: "Mic")!)
button.setIconColor(color: .label)
button.setBackgroundColor(color: .systemGray6)
button.setRadius(radius: 8)        // explicit; pass -1 to restore automatic rounding
button.setIconPadding(padding: 8)
```

### Example 3: Icon-with-text button
```swift
button.setIcon(icon: UIImage(named: "Participants")!)
button.setButtonText("Participants")   // switches to iconWithText layout
button.setTextFontSize(12)
button.setTextColor(.label)
button.setTextSpacing(4)
button.setFontWeightRawValue(0.6)      // semibold
```

### Example 4: Show a count badge (e.g. participants count)
```swift
button.badgeBackgroundColor = .systemRed
button.badgeTextColor = .white
button.badgeText = "12"
button.showBadge = true                 // badge becomes visible
// To hide:
button.showBadge = false
```

### Example 5: Programmatic placement and accessibility
```swift
button.setPosition(x: 20, y: 100)       // only meaningful without Auto Layout
button.setAccessibility(label: "Mute", hint: "Mutes your microphone")
```

## Code Snippets (Reusable Patterns)

### Non-animated layer color change (mirrors `layerChangeWithoutAnimation`)
```swift
CATransaction.begin()
CATransaction.setDisableActions(true)
// mutate CALayer properties here so they apply without implicit animation
CATransaction.commit()
```

### Reproduce the automatic corner-radius rule
```swift
let maxSide = max(button.bounds.width, button.bounds.height)
let radius = maxSide <= 32
    ? button.bounds.height / 2     // circular for small buttons
    : button.bounds.height / 4     // rounded for larger buttons
```

### Map an Objective-C raw weight to UIFont.Weight (mirrors setFontWeightRawValue)
```objective-c
// Accepts these exact values:
// 0.0 ultraLight, 0.2 thin, 0.3 light, 0.4 regular, 0.5 medium,
// 0.6 semibold, 0.7 bold, 0.8 heavy, 1.0 black; other values use regular.
[button setFontWeightRawValue:0.6]; // semibold
```

## Type Definitions

| Type | Kind | Notes |
|------|------|-------|
| `ZMUIToolkitIconBtn` | open class (`UIButton`) | This class. `@MainActor`, `@objc(ZMUIToolkitIconBtn)`. |
| `Layout` | private enum (namespace of constants) | `mediumSizeThreshold = 32`, circular divisor `2`, rounded divisor `4`, default size `40×40`, badge offset `3`, default badge text inset `(2,0)`, highlight restore delay `0.1s`. |
| `IconLayoutMode` | private enum | `iconOnly`, `iconWithText`. |
| `ZMUIToolKitIconDescriptor` | internal protocol | Initializer configuration: `frame`, `backgroundColor`, `accessibilityLabel`, `iconname`, `iconPadding` (Int), `iconColor`, `callback`. |
| `ZMUIToolkitIconBtnDescriptor` | internal struct | Concrete `ZMUIToolKitIconDescriptor` used by the library. |
| `Badge` | internal class (`UILabel`) | Top-right badge. Default fill `#DA1639` (red), white text, fully rounded unless `cornerRadius >= 0`; sizes: small 8×8, medium 12×12, large 16×16 (dot) or text-sized pill. |
| `customIconSize` | internal property (`CGSize?`) | Custom icon size; `nil` uses intrinsic image size. Set via `setIconSize`. |
| `customCornerRadius` | internal property (`CGFloat`) | `-1` = automatic. Set via `setRadius`. |
| `buttonSize` | internal computed property (`CGSize`) | Backing `_buttonSize`; sanitized on set. Returned by `intrinsicContentSize`. |
| `buttonClickAction` | internal property (`(ZMUIToolkitIconBtn) -> Void`) | Click closure, default `{ _ in }` (does nothing). |

## Metadata
- **Source file**: `ZMUIToolkit/ZMUIToolkit/Sources/Buttons/ZMUIToolkitIconBtn.swift`
- **Related files**: `ZMUIToolkit/ZMUIToolkit/Sources/Internal/ZMUIToolkitDescriptor.swift`, `ZMUIToolkit/ZMUIToolkit/Sources/Internal/Buttons/ZMBadge.swift`
- **Generated for**: AI-assisted code generation and integration
- **Verification**: Behaviors documented above were verified directly against the implementation source, not assumed.
