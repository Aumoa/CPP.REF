# Build Configuration And PCH Plan

Keep this plan sorted by priority. When an item is completed, remove it from this file instead of leaving a completed entry.

## Scope

This plan starts after source group ownership, module-specific build profiles, effective profile routing, and module PCH support have landed. It tracks only the remaining work needed to stabilize the build configuration model.

## Priority 0 - Validate Generated IDE Project Metadata

1. Validate generated IDE project metadata.

   Regenerate Visual Studio and Visual Studio Code project files for `SampleGame` when practical, then inspect the generated DebugGame metadata:

   - Native output and intermediate directories.
   - Debugger command, working directory, and game assembly arguments.
   - `UseDebugLibraries` and equivalent IntelliSense/runtime metadata.
   - `DO_CHECK` and other configuration-sensitive preprocessor definitions.

   Generated IDE projects must continue to behave as navigation and diagnostic entry points while AylaBuildTool remains the actual native build entry point.

## Priority 1 - Reduce Future Misuse

2. Review the legacy target-wide profile API.

   `Configuration.GetTargetProfile()` remains available for compatibility with the previous target-wide behavior. Review whether it should stay as-is, be renamed to make legacy use explicit, or be restricted to call sites that truly need target-wide semantics.

   New build pipeline code should prefer module effective profiles over target-wide configuration profiles.

3. Clarify output path terminology if ambiguity remains.

   If future changes keep mixing requested target configuration paths with effective module profile paths, introduce clearer helper names or wrapper methods, such as module output versus target output concepts.

   The goal is to make it difficult to accidentally place module artifacts in requested-configuration folders when the effective profile differs.
