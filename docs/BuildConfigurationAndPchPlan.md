# Build Configuration And PCH Plan

Keep this plan sorted by priority. When an item is completed, remove it from this file instead of leaving a completed entry.

## Scope

This plan starts after source group ownership, module-specific build profiles, and effective profile routing have landed. It tracks only the remaining work needed to stabilize the build configuration model and then add module PCH support.

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

## Priority 2 - Add Module PCH Support

4. Design the ModuleRules PCH API.

   Add an explicit ModuleRules-level way to opt into a PCH header. The API should keep the PCH decision local to the module, allow modules to use `CoreMinimal.h` or their own module-specific PCH, and avoid forcing a global engine-wide PCH policy too early.

5. Implement PCH build actions.

   Add PCH compile actions before source compile actions and route PCH artifacts through the module effective-profile intermediate directory.

   The implementation should cover:

   - MSVC PCH flags such as `/Yc`, `/Yu`, `/Fp`, and any required forced include behavior.
   - GCC and Clang PCH support.
   - Cache and dependency tracking that includes the selected PCH header and PCH compiler options.
   - Clean/rebuild behavior for PCH artifacts.

6. Validate `CoreMinimal.h` as the first practical PCH.

   Start with modules that naturally include `CoreMinimal.h`, verify that the PCH is used by source files that opt in, and confirm that it does not hide missing includes or introduce unwanted module dependencies.

   Compare build behavior before and after enabling the PCH so that correctness is verified before treating compile-time improvement as meaningful.
