# ZMUIToolkitStrings

## Description
`ZMUIToolkitStrings` is the localization entry point for the entire ZMUIToolkit framework. Every piece of user-facing text the toolkit renders (button menus, action sheets, dialogs, toasts, settings, the share panel, and the QoS/statistics panel) is resolved through it. To override that text, set `ZMUIToolkitStrings.stringsBundle` to a bundle containing a `ZMUIToolkit.strings` table; the resolver looks each key up there and falls back to a built-in English default when the bundle is unset or a key is missing. It is a caseless `public enum` namespace — never instantiated.

## How to use
Set the bundle once, early (e.g. in `application(_:didFinishLaunchingWithOptions:)`), before any toolkit UI is shown:

```swift
ZMUIToolkitStrings.stringsBundle = Bundle.main
```

Then add a strings table named exactly `ZMUIToolkit.strings` to that bundle and add entries only for the keys you want to override — omitted keys fall back to built-in English:

```
/* ZMUIToolkit.strings */
"leave_session"       = "Quitter la session";
"end_session_for_all" = "End for everyone";
"cancel"              = "Annuler";
"participants"        = "Participants";
"got_it"              = "OK";
```

The table name is fixed to `"ZMUIToolkit"` internally, so the file must be `ZMUIToolkit.strings`. The full key catalog (~77 unique keys across leave-session, participants, more, toasts, recording consent, share panel, search, settings, name edit, video settings, and the QoS panel) is documented in the API reference below — consult it for exact key names and default values.

## Limitations & restrictions
- If `stringsBundle` is `nil` (the default), every lookup returns its built-in English default; the toolkit is fully functional and English-only in this state.
- The resolver is `value == key ? default : value`. Because `NSBundle` returns the key itself for a missing key, if a key's localized value is identical to the key itself (e.g. `"cancel" = "cancel";`), the built-in default is used. Provide a localized value that differs from the key name.
- The table name is fixed to `"ZMUIToolkit"` and is set internally (not configurable through the public API); the file must be named `ZMUIToolkit.strings`.
- Keys must match exactly — a typo'd or absent key falls back to the English default.
- Set `stringsBundle` once during app startup on the main thread. Text already rendered is not retroactively re-localized.
- Some keys are shared across files (`cancel`, `confirm`, `settings`, `statistics`, the `qos_*` keys) — a single override affects every usage.

## Related components
- **references/leave-session-btn.md** — its action-sheet and confirm-dialog text (`leave_session`, `end_session_for_all`, `cancel`, `confirm`, etc.) is localized through this.
- **references/more-btn.md** — its menu, recording, and pause/resume-share text is localized through this.
- **references/participants-btn.md** — its panel, per-participant actions, and rename dialog text is localized through this.
- **references/setup.md** — overall framework integration; configure `stringsBundle` during app startup alongside SDK/toolkit init.
- **references/manager.md** — toolkit lifecycle; `ZMUIToolkitStrings` is independent of it and can be set before or after `initialize`.
- **references/audio-btn.md**, **references/video-btn.md**, **references/share-btn.md**, **references/pause-share-btn.md**, **references/icon-btn.md**, **references/root-video-view.md**, **references/composite-delegate.md** — other toolkit components; any user-facing text they render also resolves through this.

## Full API reference
- `docs/api/ZMUIToolkitStrings-API.md`
- `docs/api/ZMUIToolkitStrings-API.json`
