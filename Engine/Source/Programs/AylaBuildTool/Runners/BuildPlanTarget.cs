namespace AylaEngine;

internal sealed class BuildPlanTarget
{
    private readonly ModuleProject[] m_TargetProjects;
    private readonly BuildRunner.ModuleTask[] m_ModuleTasks;
    private readonly BuildRunner.ScriptTask[] m_ScriptTasks;
    private readonly BuildRunner.ShaderCompileTask[] m_ShaderTasks;

    public BuildPlanTarget(
        TargetInfo targetInfo,
        IEnumerable<ModuleProject> targetProjects,
        IEnumerable<BuildRunner.ModuleTask> moduleTasks,
        IEnumerable<BuildRunner.ScriptTask> scriptTasks,
        IEnumerable<BuildRunner.ShaderCompileTask> shaderTasks)
    {
        TargetInfo = targetInfo;
        m_TargetProjects = targetProjects.ToArray();
        m_ModuleTasks = moduleTasks.ToArray();
        m_ScriptTasks = scriptTasks.ToArray();
        m_ShaderTasks = shaderTasks.ToArray();
    }

    public TargetInfo TargetInfo { get; }

    public IReadOnlyList<ModuleProject> TargetProjects => m_TargetProjects;

    public IReadOnlyList<BuildRunner.ModuleTask> ModuleTasks => m_ModuleTasks;

    public IReadOnlyList<BuildRunner.ScriptTask> ScriptTasks => m_ScriptTasks;

    public IReadOnlyList<BuildRunner.ShaderCompileTask> ShaderTasks => m_ShaderTasks;
}
