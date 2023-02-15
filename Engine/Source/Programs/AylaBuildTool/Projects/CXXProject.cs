// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Rules;

namespace AE.Projects;

public class CXXProject
{
    private readonly string TargetFile;
    private readonly List<string> Modules = new();

    public TargetRules Rules { get; }

    public string SourceCodeDirectory => Path.GetDirectoryName(TargetFile)!;

    public CXXProject(string TargetFile, TargetRules Rules)
    {
        this.TargetFile = TargetFile;
        this.Rules = Rules;

        string SourceDirectory = Path.GetDirectoryName(TargetFile)!;
        SearchModulesRecursive(SourceDirectory, true);
    }

    public string[] GetAllModuleSourceFiles()
    {
        HashSet<string> SourceFiles = new();

        foreach (var Module in Modules)
        {
            string ModuleDirectory = Path.GetDirectoryName(Module)!;
            foreach (var SourceFile in Directory.GetFiles(ModuleDirectory, "*.*", SearchOption.AllDirectories))
            {
                SourceFiles.Add(SourceFile);
            }
        }

        SourceFiles.Add(TargetFile);
        return SourceFiles.ToArray();
    }

    private void SearchModulesRecursive(string SourceDirectory, bool bFirst)
    {
        string? ModuleFile = null;

        foreach (var Path in Directory.GetFiles(SourceDirectory, "*.cs", SearchOption.TopDirectoryOnly))
        {
            if (Path.Contains(".Target.cs") && bFirst == false)
            {
                return;
            }

            if (Path.Contains(".Module.cs"))
            {
                if (ModuleFile != null)
                {
                    throw new InvalidOperationException("Two or more .Module.cs file detected.");
                }

                ModuleFile = Path;
            }
        }

        if (ModuleFile != null)
        {
            Modules.Add(ModuleFile);
            return;
        }

        foreach (var Subdirectory in Directory.GetDirectories(SourceDirectory, "*", SearchOption.TopDirectoryOnly))
        {
            SearchModulesRecursive(Subdirectory, false);
        }
    }
}
