# Build Configuration Verification

## Scope

This note records build and IDE metadata verification for module-specific build profiles after source group ownership and effective profile routing landed.

The verification is tied to the `codex/build-config` PR branch as a change set, not to a single intermediate commit. This avoids stale provenance when review follow-up commits are added to the branch.

## Environment

- Branch: `codex/build-config`
- Host platform: Windows
- Native target: `SampleGame` `GameAssembly`
- Shader compilation: skipped with `--skip-shaders`
- Verification logs: `Intermediate\CodexVerification\BuildConfiguration`

The log directory is intentionally under `Intermediate` and is not part of the committed source tree.

## Commands

### Tool Refresh

```powershell
dotnet build Engine\Source\Programs\AylaBuildTool\AylaBuildTool.csproj
```

Result: succeeded with 0 warnings and 0 errors.

### DebugGame Rebuild

```powershell
dotnet Engine\Binaries\DotNET\AylaBuildTool.dll build --project SampleGame\SampleGame.aproject --target GameAssembly --config DebugGame --skip-shaders --clean Rebuild
```

The controlled rebuild reached `[220/220]` in the captured log, but the command wrapper timed out before it returned an exit code. The log still showed the expected artifact layout:

- Engine compile and link outputs used `Engine\Binaries\Win64\Release` and `Engine\Intermediate\...\Release`.
- No engine output used `DebugGame` directories.
- Project outputs used `SampleGame\Binaries\Win64\DebugGame` and `SampleGame\Intermediate\GameAssembly\Win64\DebugGame`.
- `GameAssembly.dll` was produced under the project `DebugGame` output directory.

A follow-up incremental check used the same configuration without `--clean Rebuild`:

```powershell
dotnet Engine\Binaries\DotNET\AylaBuildTool.dll build --project SampleGame\SampleGame.aproject --target GameAssembly --config DebugGame --skip-shaders
```

Result: succeeded with exit code 0.

### Development Build

```powershell
dotnet Engine\Binaries\DotNET\AylaBuildTool.dll build --project SampleGame\SampleGame.aproject --target GameAssembly --config Development --skip-shaders
```

The first run produced only four project-side actions for `SampleGame\Binaries\Win64\Development` and `SampleGame\Intermediate\GameAssembly\Win64\Development`. It did not schedule engine compile or link actions.

A follow-up incremental run completed with exit code 0.

### Shipping Build

```powershell
dotnet Engine\Binaries\DotNET\AylaBuildTool.dll build --project SampleGame\SampleGame.aproject --target GameAssembly --config Shipping --skip-shaders
```

Result: succeeded with exit code 0. The build produced only four project-side actions for `SampleGame\Binaries\Win64\Release` and `SampleGame\Intermediate\GameAssembly\Win64\Release`. It did not schedule engine compile or link actions.

### Generated IDE Metadata

Visual Studio and Visual Studio Code metadata were regenerated and inspected for `SampleGame`.

```powershell
dotnet Engine\Binaries\DotNET\AylaBuildTool.dll generate --project SampleGame\SampleGame.aproject
dotnet Engine\Binaries\DotNET\AylaBuildTool.dll generate --project SampleGame\SampleGame.aproject --generator VisualStudioCode
```

The generated DebugGame metadata used the expected mixed-profile layout:

- Visual Studio DebugGame metadata uses engine `Release` output and intermediate paths for engine modules.
- Visual Studio DebugGame metadata uses project `DebugGame` output and intermediate paths for `GameAssembly`.
- Visual Studio DebugGame metadata keeps `UseDebugLibraries` disabled for release-profile engine artifacts.
- DebugGame preprocessor metadata includes configuration-sensitive values such as `CONFIG_STRING=TEXT("DebugGame")` and `DO_CHECK=1`.
- Visual Studio Code DebugGame launch metadata runs `Launch` from `Engine\Binaries\Win64\Release`.
- Visual Studio Code DebugGame launch metadata passes `GameAssembly` from `SampleGame\Binaries\Win64\DebugGame`.
- Visual Studio Code Shipping launch metadata passes `GameAssembly` from `SampleGame\Binaries\Win64\Release`.
- Visual Studio Code task metadata no longer emits empty argument entries for non-editor targets.

## Findings

- `DebugGame` routes engine modules to Release-profile artifacts.
- `DebugGame` routes project modules to DebugGame-profile artifacts.
- Engine Release-profile artifacts were reused by later `Development` and `Shipping` project builds when inputs were unchanged.
- Project `Debug` artifact non-reuse is covered by the focused build profile and folder policy tests. A full `Debug` native build was not run in this pass.
- Linking the project `DebugGame` module against engine Release-profile modules produced no link-time runtime library or ABI mismatch. This does not replace future runtime validation.
- Generated IDE metadata matches the effective module profile routing used by AylaBuildTool builds.
