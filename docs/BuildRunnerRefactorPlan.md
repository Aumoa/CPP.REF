# BuildRunner Refactor Plan

This document records the current responsibilities and separation goals for AylaBuildTool's `BuildRunner` before the refactor starts. Future refactoring should proceed in small commits that preserve the behavior listed here.

## Current BuildRunner Responsibilities

`BuildRunner.RunAsync` currently performs most of the build pipeline directly.

- Creates the default `TargetInfo` from `BuildOptions` and scans the `Solution`.
- Resolves an explicit build target when one is provided, and implicitly includes `Engine`, `Launch`, `Direct3D12`, and `WindowsAPI` for game modules.
- Branches directly between `CleanOnly`, `GenerateOnly`, and normal build flows.
- Runs RHT header parsing and generates `.gen.h`, `.gen.cpp`, and C# bindings.
- Compares C++ source cache and deps files to select required compile tasks.
- Finds HLSL shader files, creates `ShaderCompileTask` instances, and builds `ShaderCompileWorker` first when needed.
- Runs CMake builds for ThirdParty modules.
- Schedules C++ compile, native link, script compile, and shader compile work through `TaskCompletionSource` based tasks.
- Manages progress prefixes and duplicate terminal failure log suppression.

This keeps the current behavior easy to trace in one file, but each new build step or target policy expands the branches and local functions inside `BuildRunner`.

## Behavior To Preserve

The following behavior must be preserved during the refactor.

- When no explicit target is provided, all `ModuleProject` instances are built.
- Game module targets include the existing implicit dependencies: `Engine`, `Launch`, `Direct3D12`, and `WindowsAPI`.
- `CleanOnly` removes intermediate artifacts and output directories, then runs reflection header generation and project file generation.
- `GenerateOnly` runs only reflection header generation and project file generation.
- Normal builds run reflection generation first and include generated `.gen.cpp` files in the C++ compile set.
- If any shader task exists, `ShaderCompileWorker` is built in `Development` configuration first and its executable is verified.
- ThirdParty CMake builds run before compile/link dispatch.
- Link tasks wait for C++ compile tasks and native module dependency tasks.
- Script tasks wait for script dependency tasks.
- Failed terminal executions are not printed more than once for the same command and exit code combination.

## Problems

### Concentrated Responsibilities

`BuildRunner` owns request interpretation, graph construction, dirty checks, action creation, execution, and logging. Reasons for change are mixed across several axes, so the impact of a feature addition is difficult to predict.

### Inline Implicit Policies

Policies such as game module required dependencies, `ShaderCompileWorker` prebuilds, and `GenerateOnly` handling are embedded directly in execution code. This makes them hard to test and hard to reuse from another runner.

### Build Graph And Execution Graph Are Mixed

`ModuleTask`, `ScriptTask`, `ShaderCompileTask`, and `CompileTask` currently describe actions and hold execution state at the same time. The step that explains what work is needed is not separated from the step that runs the work.

### Reflection Generation Coupling

RHT parsing, type collection, file generation, and generated source tracking live inside a `RunAsync` local function. Reflection feature expansion therefore keeps requiring changes to `BuildRunner`.

### Progress And Failure Logging Coupling

Progress calculation and failure logging are tied to dispatch code. Changing execution policy can also disturb output policy.

## Target Shape

In the long term, `BuildRunner` should become a thin entry point.

```csharp
public static async ValueTask RunAsync(BuildOptions options, CancellationToken cancellationToken)
{
    var request = BuildRequest.From(options);
    var workspace = await m_WorkspaceLoader.LoadAsync(request, cancellationToken);
    var targetGraph = m_TargetGraphBuilder.Build(workspace, request);
    var plan = await m_BuildPlanBuilder.BuildAsync(workspace, targetGraph, request, cancellationToken);
    var result = await m_BuildScheduler.ExecuteAsync(plan, cancellationToken);
    m_BuildReporter.Report(result);
}
```

This code is an example of the intended direction, not a required immediate end state. The actual implementation should move in stages while respecting the current static structure and local style.

## Responsibilities To Separate

### BuildRequest

Normalizes `BuildOptions` into a request model that is easier for the build system to consume. It contains execution conditions such as `TargetInfo`, clean mode, generator type, and target name.

### BuildWorkspace

Collects external state shared across the build, such as `Solution`, `Installation`, and primary/engine groups. The first step can start with helper methods before introducing a dedicated type.

### TargetGraphBuilder

Resolves explicit and full-solution build targets, then applies module dependency and implicit dependency policies. Its result should purely describe which modules are part of this build.

### ReflectionGenerationPipeline

Owns RHT header parsing, `TypeNames` collection, `.gen.h`, `.gen.cpp`, C# binding generation, and generated source list output.

### BuildPlanBuilder

Reads the target graph and cache state to create compile, link, script, shader, and CMake action lists. This stage should decide what must run without running it whenever possible.

### BuildScheduler

Manages actual execution order from `BuildPlan` action dependencies. `TaskCompletionSource` should be hidden inside this layer, and action models should not carry execution state.

### BuildReporter

Owns progress prefixes, success logs, and duplicate failure log suppression. This separates execution policy changes from output formatting.

## Suggested Commit Units

1. Document the current structure and transition plan.
2. Introduce `BuildRequest` and move `BuildOptions` normalization out of `BuildRunner`.
3. Move target selection and implicit dependency handling into `TargetGraphBuilder`.
4. Move the RHT generation flow into `ReflectionGenerationPipeline`.
5. Add a `BuildPlan` model and express existing task creation results as plan data.
6. Move dirty checks and action creation into `BuildPlanBuilder`.
7. Move dispatch local functions into `BuildScheduler`.
8. Move progress and failure log handling into `BuildReporter`.

Each commit must keep the project buildable, and each behavior-moving commit should include verification that the existing behavior is preserved.
