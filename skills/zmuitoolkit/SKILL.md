---
name: zmuitoolkit
description: Use when working on the ZMUIToolkit iOS framework, including setup and integration, ZMUIToolKitManager lifecycle, the root video surface, delegate fan-out, built-in toolbar buttons, and localization. Before making changes, read the relevant reference file in references/ for the component being changed.
---

# ZMUIToolkit

This skill covers the ZMUIToolkit iOS framework and how its main components fit together.

Read only the reference files directly relevant to the current task.

## Routing

- For SDK initialization, toolkit initialization, session join flow, teardown, or screen share integration:
  `references/setup.md`

- For `ZMUIToolKitManager`, toolkit lifecycle, shared state, config flags, or cleanup behavior:
  `references/manager.md`

- For `CompositeZoomVideoSDKDelegate`, delegate ownership, or adding SDK observers through the toolkit:
  `references/composite-delegate.md`

- For `ZMUIToolkitRootVideoView`, gallery layout, active share handling, self overlay, PiP, recording badge, or video rendering behavior:
  `references/root-video-view.md`

- For the shared base button class and visual/button configuration APIs:
  `references/icon-btn.md`

- For the audio button:
  `references/audio-btn.md`

- For the video button:
  `references/video-btn.md`

- For the share button:
  `references/share-btn.md`

- For pause/resume share behavior:
  `references/pause-share-btn.md`

- For the overflow or more button:
  `references/more-btn.md`

- For the participants button and participant count badge:
  `references/participants-btn.md`

- For leaving or ending the session:
  `references/leave-session-btn.md`

- For localization, string overrides, or `ZMUIToolkitStrings`:
  `references/strings.md`

## Working Rules

- Verify behavior against the actual source in `ZMUIToolkit/` before changing code.
- Use the sample app only as a reference, not as proof that a host app is wired the same way.
- When a change spans multiple components, read all relevant reference files before editing.
- Prefer preserving the toolkit's existing public API and UI behavior unless the task explicitly asks for a behavior change.
