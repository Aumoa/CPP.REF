namespace AylaEngine;

internal sealed class BuildPlanBuilder
{
    private readonly ModuleProject[] m_TargetProjects;
    private readonly BuildRunner.ModuleTask[] m_ModuleTasks;
    private readonly BuildRunner.ScriptTask[] m_ScriptTasks;
    private readonly BuildRunner.ShaderCompileTask[] m_ShaderTasks;
    private readonly Installation m_Installation;
    private readonly TargetInfo m_TargetInfo;
    private readonly ScriptProjectFactory m_ScriptProjectFactory;

    public BuildPlanBuilder(
        IEnumerable<ModuleProject> targetProjects,
        IEnumerable<BuildRunner.ModuleTask> moduleTasks,
        IEnumerable<BuildRunner.ScriptTask> scriptTasks,
        IEnumerable<BuildRunner.ShaderCompileTask> shaderTasks,
        Installation installation,
        TargetInfo targetInfo,
        ScriptProjectFactory scriptProjectFactory)
    {
        m_TargetProjects = targetProjects.ToArray();
        m_ModuleTasks = moduleTasks.ToArray();
        m_ScriptTasks = scriptTasks.ToArray();
        m_ShaderTasks = shaderTasks.ToArray();
        m_Installation = installation;
        m_TargetInfo = targetInfo;
        m_ScriptProjectFactory = scriptProjectFactory;
    }

    public BuildPlan Build()
    {
        List<BuildAction> buildActions = [];
        EnqueueCompileActions(buildActions);
        EnqueueShaderCompileActions(buildActions);
        EnqueueLinkActions(buildActions);
        EnqueueScriptCompileActions(buildActions);
        return new BuildPlan(buildActions);
    }

    private void EnqueueCompileActions(List<BuildAction> buildActions)
    {
        var allCompiles = m_ModuleTasks.SelectMany(p => p.NeedCompileTasks).ToArray();
        foreach (var compileTask in allCompiles)
        {
            buildActions.Add(new BuildAction(
                ct => compileTask.CompileAsync(m_Installation, m_TargetInfo, ct),
                output => string.Join('\n', [compileTask.Command.SourceCode.FilePath, .. output.Logs.Select(l => l.Value)])));
        }
    }

    private void EnqueueShaderCompileActions(List<BuildAction> buildActions)
    {
        foreach (var shaderTask in m_ShaderTasks)
        {
            buildActions.Add(new BuildAction(
                ct => shaderTask.CompileAsync(m_ModuleTasks, m_Installation, ct),
                _ => $"Compiling shaders for {shaderTask.Group.Name}"));
        }
    }

    private void EnqueueLinkActions(List<BuildAction> buildActions)
    {
        foreach (var moduleTask in m_ModuleTasks)
        {
            if (moduleTask.NeedLink(m_TargetInfo))
            {
                buildActions.Add(new BuildAction(
                    ct => moduleTask.LinkAsync(m_ModuleTasks, m_Installation, m_TargetInfo, ct),
                    output => string.Join('\n', output.Logs.Select(p => p.Value))));
            }
            else
            {
                moduleTask.SetComplete();
            }
        }
    }

    private void EnqueueScriptCompileActions(List<BuildAction> buildActions)
    {
        Dictionary<string, CSProject> virtualProjects = m_TargetProjects
            .Where(p => p.GetRule(m_TargetInfo).Script.Enabled)
            .ToDictionary(p => p.ScriptProjectFileName, p => m_ScriptProjectFactory.GetScriptProject(p));

        Task? previousScriptTask = null;
        foreach (var scriptTask in SortScriptTasksByDependency())
        {
            var prerequisiteTask = previousScriptTask;
            buildActions.Add(new BuildAction(
                async ct =>
                {
                    if (prerequisiteTask != null)
                    {
                        await prerequisiteTask;
                    }

                    return await scriptTask.BuildAsync(m_ScriptTasks, virtualProjects, m_TargetInfo, ct);
                },
                output => string.Join('\n', output.Logs.Select(p => p.Value))));
            previousScriptTask = scriptTask.Task;
        }
    }

    private List<BuildRunner.ScriptTask> SortScriptTasksByDependency()
    {
        Dictionary<string, BuildRunner.ScriptTask> taskByName = m_ScriptTasks.ToDictionary(p => p.Resolver.Name, StringComparer.OrdinalIgnoreCase);
        Dictionary<BuildRunner.ScriptTask, bool> resolvedTasks = [];
        HashSet<BuildRunner.ScriptTask> resolvingTasks = [];
        List<BuildRunner.ScriptTask> sortedTasks = [];

        foreach (var scriptTask in m_ScriptTasks)
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
