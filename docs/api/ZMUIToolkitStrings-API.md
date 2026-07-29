# ZMUIToolkitStrings API Documentation

## Module Information
- **Module**: ZMUIToolkitStrings
- **Platform**: iOS
- **Language**: Swift
- **Version**: 1.0
- **Framework**: ZMUIToolkit
- **Kind**: `public enum` (used as a namespace — it has no cases and is never instantiated)
- **Concurrency**: No `@MainActor` annotation. Configure `stringsBundle` once during app startup on the main thread.

## Overview

`ZMUIToolkitStrings` is the **localization configuration point** for the entire ZMUIToolkit framework. Every piece of user-facing text the toolkit renders (button menus, action sheets, dialogs, toasts, settings screens, the share panel, and the QoS/statistics panel) is resolved through its internal `string(for:default:)` resolver.

By default the toolkit ships with hard-coded **English fallback strings** baked into each call site. To localize or override that text, an integrator sets `ZMUIToolkitStrings.stringsBundle` to a bundle that contains a `ZMUIToolkit.strings` table. The resolver then looks each key up in that table, falling back to the built-in default whenever the bundle is unset or the key is missing.

There is exactly one public member:

| Member | Kind | Access | Purpose |
|--------|------|--------|---------|
| `stringsBundle` | `static var Bundle?` | `public` | The bundle to load the `ZMUIToolkit` strings table from. `nil` by default. |
| `stringsTable` | `static var String` | `internal` | The `.strings` table name to look up. Hard-coded to `"ZMUIToolkit"`. Not part of the public API. |
| `string(for:default:)` | `static func` | `internal` | The resolver used by toolkit components. Not callable by integrators. |

## Lifecycle

### Prerequisites
There are **no SDK or session prerequisites**. `ZMUIToolkitStrings` is pure configuration and is independent of `ZMUIToolKitManager.initialize(videoSDK:bundleId:)`. It can be configured before or after toolkit/SDK initialization.

### Setup (integrator)
1. Add a strings table named **`ZMUIToolkit.strings`** to your app target (or any bundle you control).
2. Add entries for the keys you want to override (see the Localized Strings Catalog below). You only need to provide the keys you actually want to change — any key not present in the table falls back to the built-in English default.
3. Set the bundle once, early (e.g. in `application(_:didFinishLaunchingWithOptions:)`), **before** any toolkit UI is shown:
   ```swift
   ZMUIToolkitStrings.stringsBundle = Bundle.main
   ```

### Default behavior (no setup)
If `stringsBundle` is never set (remains `nil`), every lookup returns its built-in English default. The toolkit is fully functional and English-only in this state.

## Resolver Behavior (`string(for:default:)`)

```swift
internal static func string(for key: String, default defaultValue: String) -> String {
    guard let bundle = stringsBundle else {
        return defaultValue
    }
    let value = bundle.localizedString(forKey: key, value: nil, table: stringsTable)
    return value == key ? defaultValue : value
}
```

The resolver applies two distinct fallbacks:

1. **Nil-bundle fallback** — If `stringsBundle` is `nil`, the function returns `defaultValue` immediately and never touches a bundle. This is the out-of-the-box English behavior.
2. **Missing-key fallback** — If a bundle *is* set, it calls `bundle.localizedString(forKey: key, value: nil, table: "ZMUIToolkit")`. `NSBundle`'s contract is that when a key is **not found** in the table, it returns the *key itself*. The resolver detects this with the check `value == key` and, when true, returns `defaultValue` instead. So a missing key in a configured bundle still falls back to the built-in English default.

### Behavior notes
- **Provide a localized value that differs from the key name.** Because the resolver detects a missing entry by comparing `value == key`, if a key's localized value is identical to the key itself (e.g. `"cancel" = "cancel";`), the built-in default is used (`"Cancel"`). Localize each value to a string that differs from its key.
- **The table name is fixed.** Lookups always use the table `"ZMUIToolkit"` (the internal `stringsTable`). The file in your bundle must therefore be named `ZMUIToolkit.strings`. The table name is set internally and is not configurable through the public API.
- **`value: nil` is passed**, so `NSBundle` returns the key when the key is not found, which is what the `value == key` check relies on.

## APIs

### `stringsBundle`
```swift
public static var stringsBundle: Bundle?
```
The bundle from which the toolkit loads the `ZMUIToolkit` strings table. `nil` by default.
- **Set it to** `Bundle.main` (most common) or any bundle containing a `ZMUIToolkit.strings` table to localize/override toolkit text.
- **Leave it `nil`** to use the built-in English defaults.
- **Side effects**: None on assignment. The new value takes effect on the *next* `string(for:default:)` lookup. Text already rendered is not retroactively updated — set this before presenting toolkit UI.
- **Threading**: Configure during app startup on the main thread.

### `stringsTable` *(internal)*
```swift
internal static var stringsTable: String = "ZMUIToolkit"
```
The `.strings` table name used for all lookups. Internal — not part of the public surface; documented for completeness. Determines that your localization file must be `ZMUIToolkit.strings`.

### `string(for:default:)` *(internal)*
```swift
internal static func string(for key: String, default defaultValue: String) -> String
```
The resolver used by toolkit components to fetch a localized string. Returns the localized value from `stringsBundle`'s `ZMUIToolkit` table, or `defaultValue` if the bundle is `nil` or the key is absent. Internal — integrators do not call this directly; they influence it only by setting `stringsBundle` and supplying a strings table.
- **Parameters**: `key` — the lookup key in the `ZMUIToolkit` table; `default` (`defaultValue`) — the built-in English fallback used when the bundle is nil or the key is missing.
- **Returns**: The resolved string.

## Callbacks

`ZMUIToolkitStrings` defines and uses **no callbacks or delegates**. It is a synchronous configuration/resolver namespace.

## Localized Strings Catalog

All toolkit text is resolved through `ZMUIToolkitStrings.string(for:default:)`. To override any entry, set `ZMUIToolkitStrings.stringsBundle` and add the key to your bundle's `ZMUIToolkit.strings` table. Keys not present in the table use the built-in default shown here.

Keys are listed grouped by the source file that uses them. Some keys (e.g. `cancel`, `confirm`, `settings`) are shared across multiple files; the first occurrence lists the default and shared keys are cross-referenced.

### Leave-session button (`ZMUIToolkitLeaveSessionBtn.swift`)
| Key | Default value | Used for |
|-----|---------------|----------|
| `leave_session` | "Leave session" | First action sheet item |
| `end_session_for_all` | "End session for all" | Host-only action sheet item |
| `cancel` | "Cancel" | Action sheet + confirm dialog cancel |
| `end_session_alert_title` | "End session for all" | Confirm dialog title |
| `end_session_alert_message` | "Are you sure you want to end the session for all participants?" | Confirm dialog message |
| `confirm` | "Confirm" | Confirm dialog confirm button |
| `assign_host` | "Assign a host" | Assign-host panel title |
| `assign_and_leave` | "Assign and Leave" | Assign-host panel primary button |

### Participants button (`ZMUIToolkitParticipantsBtn.swift`)
| Key | Default value | Used for |
|-----|---------------|----------|
| `participants` | "Participants" | Participants panel title |
| `mute_all` | "Mute all" | Host action |
| `ask_all_to_unmute` | "Ask all to unmute" | Host action |
| `ask_to_unmute_audio` | "Ask to unmute audio" | Per-participant action |
| `mute_audio` | "Mute audio" | Per-participant action |
| `make_host` | "Make host" | Per-participant action |
| `make_co_host` | "Make co-host" | Per-participant action |
| `rename` | "Rename" | Per-participant action |
| `remove_from_session` | "Remove from session" | Per-participant action |
| `cancel` | "Cancel" | Action sheet cancel (shared) |
| `change_name_in_session` | "Change name in session" | Rename dialog title |
| `enter_new_name` | "Enter new name" | Rename text field placeholder |
| `confirm` | "Confirm" | Rename dialog confirm (shared) |

### More button (`ZMUIToolkitMoreBtn.swift`)
| Key | Default value | Used for |
|-----|---------------|----------|
| `more` | "More" | Menu title |
| `settings` | "Settings" | Menu item |
| `cancel` | "Cancel" | Menu cancel (shared) |
| `record` | "Record" | Menu item |
| `stop_recording` | "Stop recording" | Menu item |
| `pause_recording` | "Pause recording" | Menu item |
| `resume_recording` | "Resume recording" | Menu item |
| `start_recording_alert_title` | "Start recording" | Start-recording confirm dialog title |
| `start_recording_alert_message` | "After the recording starts, all participants will be informed and the meeting audio, video and shared screen view will be recorded to the cloud." | Start-recording confirm dialog message |
| `start` | "Start" | Start-recording confirm button |
| `stop_recording_alert_title` | "Stop Recording" | Stop-recording confirm dialog title |
| `stop_recording_alert_message` | "You will receive an email notification when the cloud recording is ready." | Stop-recording confirm dialog message |
| `stop` | "Stop" | Stop-recording confirm button |
| `pause_share` | "Pause sharing" | Menu item |
| `resume_share` | "Resume sharing" | Menu item |

### Toasts (`ZMToastManager.swift`)
| Key | Default value | Used for |
|-----|---------------|----------|
| `became_host` | "You've become the host." | Toast |
| `became_co_host` | "You've become the co-host." | Toast |
| `all_muted` | "All current and new participants are muted." | Toast |
| `request_sent` | "Your request has been sent." | Toast |
| `name_changed` | "Your name has been changed." | Toast |
| `ended_by_host` | "The session was ended by the host." | Toast |
| `kicked_by_host` | "You were kicked from the session by the host." | Toast |
| `network_error` | "A network error occurred, causing you to leave the session." | Toast |
| `unknown_error` | "An unknown issue has occurred, causing you to leave the session." | Toast |
| `recording_stopped` | "Recording stopped" | Toast |
| `recording_paused` | "Recording paused" | Toast |
| `recording_resumed` | "Recording resumed" | Toast |

### Recording consent (`ZMRecordingManager.swift`)
| Key | Default value | Used for |
|-----|---------------|----------|
| `recording_consent_title` | "This meeting is being recorded" | Consent dialog title |
| `recording_consent_message` | "By staying in this meeting, you consent to being recorded." | Consent dialog message |
| `leave_meeting` | "Leave meeting" | Consent dialog action |
| `got_it` | "Got it" | Consent dialog action |
| `do_not_ask_again` | "Do not ask me again." | Consent dialog checkbox |

### Share panel (`ZMUIToolkitSharePanelView.swift`)
| Key | Default value | Used for |
|-----|---------------|----------|
| `share_panel_message` | "You are sharing your screen." | Share panel message label |
| `share_panel_device_audio` | "Share device audio" | Share device-audio toggle title |
| `share_panel_stop_button` | "Stop Sharing" | Stop-sharing button |

### Search bar (`ZMUIToolkitSearchBar.swift`)
| Key | Default value | Used for |
|-----|---------------|----------|
| `search_for_participants` | "Search for participants" | Search bar placeholder |

### Participants cell (`ZMUIToolkitParticipantsCell.swift`)
| Key | Default value | Used for |
|-----|---------------|----------|
| `host_label` | "Host" | Role badge |
| `co_host_label` | "Co-host" | Role badge |
| `me_label` | "me" | Self badge |

### Settings (`SettingsContentView.swift`)
| Key | Default value | Used for |
|-----|---------------|----------|
| `settings` | "Settings" | Settings screen title (shared) |
| `my_name` | "My Name" | Settings row |
| `general` | "General" | Settings section header |
| `statistics` | "Statistics" | Settings row / QoS panel title (shared) |

### Name edit (`NameEditContentView.swift`)
| Key | Default value | Used for |
|-----|---------------|----------|
| `edit_your_name` | "Edit Your Name" | Screen title |
| `save` | "Save" | Save button |
| `name_placeholder` | "Enter your name" | Text field placeholder |

### Video settings (`VideoSettingsContentView.swift`)
| Key | Default value | Used for |
|-----|---------------|----------|
| `video` | "Video" | Section title |
| `enable_pip_mode` | "Enable PiP Mode" | Settings toggle |

### QoS / Statistics panel (`ZMUIToolkitQoSPanel.swift`)
This view resolves all keys through a private wrapper `L(key, fallback)` that delegates to `ZMUIToolkitStrings.string(for:default:)`. The `qos_*` keys below are shared across the Audio, Video, and Screen-sharing tabs (e.g. `qos_latency`, `qos_jitter`, `qos_packet_loss`, `qos_resolution`, `qos_fps` reused per tab).
| Key | Default value | Used for |
|-----|---------------|----------|
| `statistics` | "Statistics" | Panel title (shared with Settings) |
| `qos_tab_audio` | "Audio" | Tab label |
| `qos_tab_video` | "Video" | Tab label |
| `qos_tab_screen_sharing` | "Screen sharing" | Tab label |
| `qos_item_name` | "Item name" | Table column header |
| `qos_send` | "Send" | Table column header |
| `qos_receive` | "Receive" | Table column header |
| `qos_frequency` | "Frequency" | Audio metric row |
| `qos_latency` | "Latency" | Metric row (all tabs) |
| `qos_jitter` | "Jitter" | Metric row (all tabs) |
| `qos_packet_loss` | "Packet loss - avg(max)" | Metric row (all tabs) |
| `qos_resolution` | "Resolution" | Metric row (video, screen-sharing) |
| `qos_fps` | "Frames per second" | Metric row (video, screen-sharing) |

### Shared keys summary
These keys appear in more than one file and a single override affects all of them:
- `cancel` — `ZMUIToolkitLeaveSessionBtn`, `ZMUIToolkitParticipantsBtn`, `ZMUIToolkitMoreBtn`
- `confirm` — `ZMUIToolkitLeaveSessionBtn`, `ZMUIToolkitParticipantsBtn`
- `settings` — `ZMUIToolkitMoreBtn`, `SettingsContentView`
- `statistics` — `SettingsContentView`, `ZMUIToolkitQoSPanel`

> **Count**: 77 unique localization keys across 11 source files (95 total call sites, including shared keys such as `cancel`/`confirm`/`settings`/`statistics` and the `qos_*` keys reused across QoS tabs).

## Error Handling

`ZMUIToolkitStrings` performs **no error handling, no throwing, and no return-value inspection**. The resolver cannot fail: it always returns a non-optional `String`. "Failure" modes degrade gracefully to the built-in English default:

| Situation | Result |
|-----------|--------|
| `stringsBundle == nil` | Returns the built-in `default` (English). |
| Bundle set, but no `ZMUIToolkit.strings` table | `localizedString(forKey:value:table:)` returns the key → `value == key` → returns the built-in `default`. |
| Bundle + table set, key missing | Same as above → returns the built-in `default`. |
| Bundle + table set, key present | Returns the localized value. |
| Localized value equals the key string | Treated as missing → returns the built-in `default`. (See Resolver Behavior.) |

There is no retry or recovery to implement; correctness is entirely a matter of providing a correctly named `ZMUIToolkit.strings` table with the desired keys.

## Rules

- **REQUIRED (to localize)**: Provide a strings table file named exactly **`ZMUIToolkit.strings`** in the bundle assigned to `stringsBundle`. The table name is fixed internally and cannot be changed via the public API.
- **RECOMMENDED**: Set `stringsBundle` once, early during app startup (on the main thread), before any toolkit UI is presented. Values already rendered are not retroactively re-localized.
- **OPTIONAL**: Override only the keys you want; omitted keys fall back to built-in English defaults.
- **AVOID**: Localizing any key to a value that is byte-identical to its key name — the resolver treats `value == key` as "not found" and returns the built-in default instead. Provide a value that differs from the key.
- **NOTE**: `ZMUIToolkitStrings` is a caseless `enum` used as a namespace; do not attempt to instantiate it.
- **NOTE**: `string(for:default:)` and `stringsTable` are `internal`; integrators interact only via `stringsBundle` plus the `.strings` table contents.

## Examples

### Example 1: Use built-in English defaults (no setup)
```swift
// Do nothing. stringsBundle stays nil; all toolkit text renders in built-in English.
let leaveButton = ZMUIToolkitLeaveSessionBtn() // menu reads "Leave session", "Cancel", etc.
```

### Example 2: Localize toolkit text from the app bundle
```swift
// In AppDelegate.application(_:didFinishLaunchingWithOptions:)
ZMUIToolkitStrings.stringsBundle = Bundle.main
// Add a ZMUIToolkit.strings file to the app target with the keys you want to override:
//   "leave_session" = "Quitter la session";
//   "cancel"        = "Annuler";
//   "participants"  = "Participants";
// Any key not present here falls back to the built-in English default.
```

### Example 3: Override text from a custom (non-main) bundle
```swift
// Resources packaged in a separate bundle that contains ZMUIToolkit.strings
if let url = Bundle.main.url(forResource: "ToolkitLocalization", withExtension: "bundle"),
   let customBundle = Bundle(url: url) {
    ZMUIToolkitStrings.stringsBundle = customBundle
}
```

### Example 4: Partial override (only the keys you care about)
```swift
// ZMUIToolkit.strings — only these two are overridden; everything else stays English.
// "end_session_for_all" = "End for everyone";
// "got_it"              = "OK";
ZMUIToolkitStrings.stringsBundle = Bundle.main
```

## Code Snippets (Reusable Patterns)

### Configure localization at startup
```swift
// Set once, early, on the main thread. Effective on the next string lookup.
ZMUIToolkitStrings.stringsBundle = Bundle.main
```

### Mirror the resolver's fallback logic (for reference)
```swift
func resolve(_ key: String, default def: String, bundle: Bundle?) -> String {
    guard let bundle = bundle else { return def }
    let value = bundle.localizedString(forKey: key, value: nil, table: "ZMUIToolkit")
    return value == key ? def : value   // missing key -> NSBundle returns the key -> fall back
}
```

### Minimal `ZMUIToolkit.strings` file
```
/* ZMUIToolkit.strings */
"leave_session"        = "Leave session";
"end_session_for_all"  = "End session for all";
"cancel"               = "Cancel";
"confirm"              = "Confirm";
"participants"         = "Participants";
```

## Type Definitions

| Type | Kind | Notes |
|------|------|-------|
| `ZMUIToolkitStrings` | `public enum` | Caseless namespace; the framework's localization configuration point. |
| `Bundle` | Foundation class | Type of `stringsBundle`; source of the `ZMUIToolkit` strings table. `nil` means "use defaults." |
| `stringsTable` (`String`) | internal static | Fixed table name `"ZMUIToolkit"`; implies the file must be `ZMUIToolkit.strings`. |

## Metadata
- **Source file**: `ZMUIToolkit/ZMUIToolkit/Sources/ZMUIToolkitStrings.swift`
- **Generated for**: AI-assisted code generation and integration
- **Verification**: Behaviors documented above were verified directly against the implementation source. The nil-bundle fallback (`guard let bundle = stringsBundle else { return defaultValue }`), the missing-key fallback (`value == key ? defaultValue : value`), and the fixed strings table name (`"ZMUIToolkit"`) were read from `ZMUIToolkitStrings.swift`. The complete key catalog was compiled by grepping `string(for:` across all `*.swift` files under `Sources` (including the `L(key, fallback)` wrapper in `ZMUIToolkitQoSPanel.swift`); defaults are quoted verbatim from those call sites.
