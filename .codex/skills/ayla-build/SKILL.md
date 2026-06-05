---
name: ayla-build
description: Repository-local build and generation workflow for Ayla engine projects. Use when Codex needs to build, generate, or verify native C++ modules, C# projects, AylaBuildTool, SampleGame, Engine, shader/reflection generation, or related build diagnostics in this repository.
---

# Ayla Build

## Core Rules

- Drive native C++ builds through AylaBuildTool.
- Do not use generated C++ IDE projects as normal build entry points. Use them only for IDE navigation or compile diagnostics.
- Build C# projects with `dotnet build <project>.csproj`.
- Refresh AylaBuildTool first when it is missing, stale, or likely affected by current changes:

```powershell
dotnet build Engine/Source/Programs/AylaBuildTool/AylaBuildTool.csproj
```

## Native Build Checks

Use SampleGame when it exists:

```powershell
dotnet Engine/Binaries/DotNET/AylaBuildTool.dll build --project SampleGame/SampleGame.aproject --target GameAssembly
```

If `SampleGame/SampleGame.aproject` does not exist, build the engine target instead:

```powershell
dotnet Engine/Binaries/DotNET/AylaBuildTool.dll build --target Engine
```

When the current changes do not touch shader code, shader assets, ShaderCompileWorker, shader build rules, shader binary loading, or shader dependency handling, append `--skip-shaders` to native build checks so shaders are not compiled separately.

Preserve user-requested configuration, editor, platform, or target options when they are provided.

## Generation Checks

Use generation checks when changes affect project scanning, rules, generated code, reflection headers, shaders, or solution/materialization flows:

```powershell
dotnet Engine/Binaries/DotNET/AylaBuildTool.dll generate --project SampleGame/SampleGame.aproject
```

When SampleGame is absent, inspect the current repository targets and choose the engine project path that matches the requested workflow.

## Verification Notes

- Prefer focused checks that match the touched subsystem.
- Report when a build or generation command cannot be run.
- Keep generated C++ IDE projects out of normal native build verification unless the user asks for diagnostics from them.
