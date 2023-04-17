// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Source;

namespace AE.Projects;

public class Workspace
{
    private readonly List<ACXXModule> CXXModules = new();
    private readonly List<ACSModule> CSModules = new();

    public Workspace()
    {
    }

    public ProjectDirectory CurrentTarget { get; private set; }

    public async Task ConfigureWorkspaceAsync(ProjectDirectory Target, bool bEngineCode, CancellationToken SToken = default)
    {
        await CollectModuleInSubdirectoryRecursiveAsync(Target, Target.Source.Root, SToken);
        CurrentTarget = Target;
    }

    private static bool IsSourceCode(string Filename)
    {
        string Ext = Path.GetExtension(Filename).ToLower();
        return Ext == ".cpp" || Ext == ".h" || Ext == ".ixx" || Ext == ".cs";
    }

    private async Task CollectModuleInSubdirectoryRecursiveAsync(ProjectDirectory Target, string InSubdirectory, CancellationToken SToken = default)
    {
        string[] ModulePath = Directory.GetFiles(InSubdirectory, "*.Module.cs");
        if (ModulePath.Any())
        {
            var CXXModule = new ACXXModule(Target, Path.GetRelativePath(Target.Source.Root, InSubdirectory));
            await CXXModule.ConfigureAsync(SToken);
            CXXModules.Add(CXXModule);
            return;
        }

        ModulePath = Directory.GetFiles(InSubdirectory, "*.csproj");
        if (ModulePath.Any())
        {
            CSModules.Add(new ACSModule(Target, InSubdirectory));
            return;
        }

        foreach (var Subdirectory in Directory.GetDirectories(InSubdirectory, "*", SearchOption.TopDirectoryOnly))
        {
            await CollectModuleInSubdirectoryRecursiveAsync(Target, Subdirectory, SToken);
        }
    }

    public IEnumerable<ACSModule> GetCSModules() => CSModules;

    public IEnumerable<ACXXModule> GetCXXModules() => CXXModules;
}
