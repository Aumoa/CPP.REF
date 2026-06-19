# Build Configuration And PCH Plan

Keep this plan sorted by priority. When an item is completed, remove it from this file instead of leaving a completed entry.

## Scope

This plan starts after source group ownership, module-specific build profiles, and effective profile routing have landed. It tracks only the remaining work needed to stabilize the build configuration model and then add module PCH support.

## Priority 0 - Lock The Current Build Configuration Policy

1. Add focused build configuration policy tests.

   Validate that `BuildProfileResolver` maps each requested configuration by source group:

   - Engine: `Debug -> Debug`, `DebugGame -> Release`, `Development -> Release`, `Shipping -> Release`.
   - Project: `Debug -> Debug`, `DebugGame -> DebugGame`, `Development -> Development`, `Shipping -> Release`.

   Also validate the `BuildConfigurationProfile` properties that drive compiler and linker behavior: optimization, runtime library mode, assertion mode, and ABI mode.

   If there is no suitable C# test harness for deterministic AylaBuildTool or AylaBuildRules policy tests, add the smallest practical test project first.

2. Add effective-profile path policy tests.

   Validate that `FolderPolicy` uses the effective profile name for module artifacts such as intermediate directories, native output directories, generated headers, generated sources, scripts, shaders, and test executables.

   Cover editor suffix behavior as part of the path tests.

## Priority 1 - Validate Real DebugGame Behavior

3. Verify the intended DebugGame reuse and compatibility behavior.

   Run a clean or controlled build sequence that proves the actual artifact layout and incremental behavior:

   - Engine modules in `DebugGame` use Release-profile artifacts.
   - Project modules in `DebugGame` use DebugGame-profile artifacts.
   - Engine Release-profile artifacts can be shared by `DebugGame`, `Development`, and `Shipping` when inputs are unchanged.
   - Project `Debug` artifacts are not accidentally reused by `DebugGame`, because `DebugGame` intentionally uses release runtime and release ABI for compatibility with engine Release artifacts.
   - Linking Project DebugGame modules against Engine Release modules does not introduce runtime library or ABI mismatches.

4. Validate generated IDE project metadata.

   Regenerate Visual Studio and Visual Studio Code project files for `SampleGame` when practical, then inspect the generated DebugGame metadata:

   - Native output and intermediate directories.
   - Debugger command, working directory, and game assembly arguments.
   - `UseDebugLibraries` and equivalent IntelliSense/runtime metadata.
   - `DO_CHECK` and other configuration-sensitive preprocessor definitions.

   Generated IDE projects must continue to behave as navigation and diagnostic entry points while AylaBuildTool remains the actual native build entry point.

## Priority 2 - Reduce Future Misuse

5. Review the legacy target-wide profile API.

   `Configuration.GetTargetProfile()` remains available for compatibility with the previous target-wide behavior. Review whether it should stay as-is, be renamed to make legacy use explicit, or be restricted to call sites that truly need target-wide semantics.

   New build pipeline code should prefer module effective profiles over target-wide configuration profiles.

6. Clarify output path terminology if ambiguity remains.

   If future changes keep mixing requested target configuration paths with effective module profile paths, introduce clearer helper names or wrapper methods, such as module output versus target output concepts.

   The goal is to make it difficult to accidentally place module artifacts in requested-configuration folders when the effective profile differs.

## Priority 3 - Add Module PCH Support

7. Design the ModuleRules PCH API.

   Add an explicit ModuleRules-level way to opt into a PCH header. The API should keep the PCH decision local to the module, allow modules to use `CoreMinimal.h` or their own module-specific PCH, and avoid forcing a global engine-wide PCH policy too early.

8. Implement PCH build actions.

   Add PCH compile actions before source compile actions and route PCH artifacts through the module effective-profile intermediate directory.

   The implementation should cover:

   - MSVC PCH flags such as `/Yc`, `/Yu`, `/Fp`, and any required forced include behavior.
   - GCC and Clang PCH support.
   - Cache and dependency tracking that includes the selected PCH header and PCH compiler options.
   - Clean/rebuild behavior for PCH artifacts.

9. Validate `CoreMinimal.h` as the first practical PCH.

   Start with modules that naturally include `CoreMinimal.h`, verify that the PCH is used by source files that opt in, and confirm that it does not hide missing includes or introduce unwanted module dependencies.

   Compare build behavior before and after enabling the PCH so that correctness is verified before treating compile-time improvement as meaningful.
