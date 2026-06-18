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

## Test Checks

- When a dedicated unit test target, test command, or focused executable test exists for the touched subsystem, run it after refreshing AylaBuildTool if the build tool changed.
- Add or update focused tests for deterministic logic that is practical to isolate, such as math utilities, parsers, serializers, build graph decisions, module rule resolution, reflection metadata, interop marshalling, lifetime management, and state transformations.
- If a change is not practical to cover with focused tests, use the nearest build, generation, integration, runtime, or platform check and report that rationale.
- When tests exist and current changes do not touch shader-related code or assets, keep `--skip-shaders` on test-driven AylaBuildTool checks when the command supports it.

## Generation Checks

Use generation checks when changes affect project scanning, rules, generated code, reflection headers, shaders, or solution/materialization flows:

```powershell
dotnet Engine/Binaries/DotNET/AylaBuildTool.dll generate --project SampleGame/SampleGame.aproject
```

When SampleGame is absent, inspect the current repository targets and choose the engine project path that matches the requested workflow.

## Verification Notes

- Prefer focused checks that match the touched subsystem.
- Before committing feature work with tests, run the relevant tests when practical and report the result.
- Report when a build or generation command cannot be run.
- Keep generated C++ IDE projects out of normal native build verification unless the user asks for diagnostics from them.
- When `gh` is available and a relevant GitHub Actions run exists, verify compile results for Windows, macOS, and Linux at the job level.
- If `gh` or Actions access is unavailable, report that Actions validation was skipped and perform a stricter source and workflow review for Windows, macOS, and Linux support.
- Before using GitHub credentials for remote validation, branch publication, pull request updates, or bot-authored validation comments, follow `.codex/skills/github-app-credential-policy/SKILL.md`.
- On shared working branches such as `dev`, `master`, `main`, or release branches, do not push only to create or test an Actions run unless the user explicitly approves that shared-state action first.
- On clearly isolated task branches, especially branches whose names start with `codex/`, Codex may push that branch at its own discretion when doing so is useful for build or Actions validation.
- Destructive or broad shared-state actions still require explicit user approval. If branch ownership is unclear, treat the branch as shared.
- Branch-local workflow or CI trigger changes may be used for validation on isolated task branches, but remove or revert those temporary settings before merging into protected shared branches.
- Permanent CI policy changes, such as enabling pull request compile checks for protected branch targets, are allowed when intentionally requested and should be reviewed as normal workflow changes.
