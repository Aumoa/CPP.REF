namespace AylaEngine;

internal sealed class BuildPlanBuilder
{
    private readonly BuildPlanTarget[] m_Targets;
    private readonly Installation m_Installation;
    private readonly ScriptProjectFactory m_ScriptProjectFactory;
    private readonly HashSet<BuildRunner.CompileTask> m_EnqueuedCompileTasks = [];
    private readonly HashSet<BuildRunner.ModuleTask> m_EnqueuedLinkTasks = [];
    private readonly HashSet<BuildRunner.ScriptTask> m_EnqueuedScriptTasks = [];

    public BuildPlanBuilder(
        IEnumerable<BuildPlanTarget> targets,
        Installation installation,
        ScriptProjectFactory scriptProjectFactory)
    {
        m_Targets = targets.ToArray();
        m_Installation = installation;
        m_ScriptProjectFactory = scriptProjectFactory;
    }

    public BuildPlan Build()
    {
        List<BuildAction> buildActions = [];
        var shaderCompileWorkerTask = FindShaderCompileWorkerTask();
        foreach (var target in m_Targets)
        {
            EnqueueCompileActions(buildActions, target);
            EnqueueShaderCompileActions(buildActions, target, shaderCompileWorkerTask);
            EnqueueLinkActions(buildActions, target);
            EnqueueScriptCompileActions(buildActions, target);
        }

        return new BuildPlan(buildActions);
    }

    private BuildRunner.ModuleTask? FindShaderCompileWorkerTask()
    {
        return m_Targets
            .SelectMany(target => target.ModuleTasks)
            .FirstOrDefault(moduleTask => string.Equals(moduleTask.Resolver.Name, "ShaderCompileWorker", StringComparison.OrdinalIgnoreCase));
    }

    private void EnqueueCompileActions(List<BuildAction> buildActions, BuildPlanTarget target)
    {
        var allCompiles = target.ModuleTasks.SelectMany(p => p.NeedCompileTasks).ToArray();
        foreach (var compileTask in allCompiles)
        {
            if (m_EnqueuedCompileTasks.Add(compileTask) == false)
            {
                continue;
            }

            buildActions.Add(new BuildAction(
                ct => compileTask.CompileAsync(m_Installation, target.TargetInfo, ct),
                output => string.Join('\n', [compileTask.Command.SourceCode.FilePath, .. output.Logs.Select(l => l.Value)])));
        }
    }

    private void EnqueueShaderCompileActions(List<BuildAction> buildActions, BuildPlanTarget target, BuildRunner.ModuleTask? shaderCompileWorkerTask)
    {
        foreach (var shaderTask in target.ShaderTasks)
        {
            buildActions.Add(new BuildAction(
                async ct =>
                {
                    if (shaderCompileWorkerTask != null)
                    {
                        await shaderCompileWorkerTask.Task;
                    }

                    return await shaderTask.CompileAsync(target.ModuleTasks, m_Installation, ct);
                },
                _ => $"Compiling shaders for {shaderTask.Group.Name}"));
        }
    }

    private void EnqueueLinkActions(List<BuildAction> buildActions, BuildPlanTarget target)
    {
        foreach (var moduleTask in target.ModuleTasks)
        {
            if (m_EnqueuedLinkTasks.Add(moduleTask) == false)
            {
                continue;
            }

            if (moduleTask.NeedLink(target.TargetInfo))
            {
                buildActions.Add(new BuildAction(
                    ct => moduleTask.LinkAsync(target.ModuleTasks, m_Installation, target.TargetInfo, ct),
                    output => string.Join('\n', output.Logs.Select(p => p.Value))));
            }
            else
            {
                moduleTask.SetComplete();
            }
        }
    }

    private void EnqueueScriptCompileActions(List<BuildAction> buildActions, BuildPlanTarget target)
    {
        Dictionary<string, CSProject> virtualProjects = target.TargetProjects
            .Where(p => p.GetRule(target.TargetInfo).Script.Enabled)
            .ToDictionary(p => p.ScriptProjectFileName, p => m_ScriptProjectFactory.GetScriptProject(p));

        Task? previousScriptTask = null;
        foreach (var scriptTask in SortScriptTasksByDependency(target))
        {
            if (m_EnqueuedScriptTasks.Add(scriptTask) == false)
            {
                continue;
            }

            var prerequisiteTask = previousScriptTask;
            buildActions.Add(new BuildAction(
                async ct =>
                {
                    if (prerequisiteTask != null)
                    {
                        await prerequisiteTask;
                    }

                    return await scriptTask.BuildAsync(target.ScriptTasks, virtualProjects, target.TargetInfo, ct);
                },
                output => string.Join('\n', output.Logs.Select(p => p.Value))));
            previousScriptTask = scriptTask.Task;
        }
    }

    private static List<BuildRunner.ScriptTask> SortScriptTasksByDependency(BuildPlanTarget target)
    {
        Dictionary<string, BuildRunner.ScriptTask> taskByName = target.ScriptTasks.ToDictionary(p => p.Resolver.Name, StringComparer.OrdinalIgnoreCase);
        Dictionary<BuildRunner.ScriptTask, bool> resolvedTasks = [];
        HashSet<BuildRunner.ScriptTask> resolvingTasks = [];
        List<BuildRunner.ScriptTask> sortedTasks = [];

        foreach (var scriptTask in target.ScriptTasks)
        {
            Visit(scriptTask);
        }

        return sortedTasks;

        void Visit(BuildRunner.ScriptTask scriptTask)
        {
            if (resolvedTasks.ContainsKey(scriptTask))
            {
                return;
            }

            if (resolvingTasks.Add(scriptTask) == false)
            {
                throw new InvalidOperationException($"Cyclic script dependency detected at '{scriptTask.Resolver.Name}'.");
            }

            foreach (var dependencyName in scriptTask.Resolver.DependencyModuleNames)
            {
                if (taskByName.TryGetValue(dependencyName, out var dependencyTask))
                {
                    Visit(dependencyTask);
                }
            }

            resolvingTasks.Remove(scriptTask);
            resolvedTasks.Add(scriptTask, true);
            sortedTasks.Add(scriptTask);
        }
    }
}
