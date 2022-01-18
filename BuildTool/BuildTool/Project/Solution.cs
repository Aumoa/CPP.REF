// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

using DotNETUtilities;

using System.Reflection;

namespace BuildTool;

class Solution : CompiledModuleCode<SolutionRule>
{
    public readonly DirectoryReference SolutionDir;

    public Solution(FileReference SolutionCode) : base(SolutionCode)
    {
        SolutionDir = SolutionCode.GetParent();
    }

    readonly List<Project> Projects = new();
    public IReadOnlyCollection<Project> AllProjects { get => Projects.AsReadOnly(); }

    public void SearchProjects()
    {
        Console.WriteLine("Search all projects.");

        List<FileReference> SolutionFiles = new();
        SolutionFiles.AddRange(SolutionDir.Move(CompiledRule.EngineRoot).Move("Source").GetAllFiles());
        SolutionFiles.AddRange(SolutionDir.Move(CompiledRule.ThirdPartyRoot).Move("Source").GetAllFiles());
        SolutionFiles.AddRange(SolutionDir.Move("Source").GetAllFiles());

        foreach (FileReference SolutionFile in SolutionFiles)
        {
            if (SolutionFile.FullPath.Contains("Module.cs"))
            {
                try
                {
                    Project FoundProject = new Project(SolutionFile);
                    Projects.Add(FoundProject);
                    Console.WriteLine("Found project: {0}", FoundProject.CompiledRule.Name);
                }
                catch (Exception E)
                {
                    Console.WriteLine("Warning: Couldn't parsing project declaration with error: {0}", E.Message);
                }
            }
        }
    }
}