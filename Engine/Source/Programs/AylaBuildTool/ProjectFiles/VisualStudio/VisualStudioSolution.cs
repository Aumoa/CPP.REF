// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Text;

using AE.BuildSettings;
using AE.Extensions;
using AE.Misc;
using AE.Projects;
using AE.System;

namespace AE.ProjectFiles.VisualStudio;

public class VisualStudioSolution : Solution
{
    private readonly string solutionGuid;
    private readonly string solutionFile;

    public VisualStudioSolution(Workspace workspace, string? projectFile) : base(workspace)
    {
        string SolutionName;
        if (projectFile == null)
        {
            SolutionName = "Engine";
        }
        else
        {
            SolutionName = Path.GetFileNameWithoutExtension(projectFile);
        }

        string SolutionGuidKey = "Solution_" + Path.GetFileNameWithoutExtension(projectFile);
        solutionGuid = CRC32.GenerateGuid(SolutionGuidKey).ToString().ToUpper();
        solutionFile = Path.Combine(workspace.CurrentTarget.Root, Path.ChangeExtension(SolutionName, ".sln"));
    }

    public async override Task GenerateProjectFilesAsync(CancellationToken cancellationToken = default)
    {
        List<VisualStudioProject> vsProjects = new();
        foreach (var csModule in workspace.GetCSModules())
        {
            vsProjects.Add(new VisualCSharpProject(csModule, false, false));
        }

        VisualCXXProject? engineCppProject = null;
        VisualCXXProject? gameCppProject = null;

        List<SearchedModule> modules = new();
        foreach (var cppModule in workspace.GetCXXModules())
        {
            if (cppModule.IsInProgramsDirectory)
            {
                vsProjects.Add(new VisualCXXProject(workspace, cppModule.ProjectDirectory, cppModule.Name, cppModule.SourcePath, "Programs"));
            }
            else
            {
                string SourceDirectory = cppModule.ProjectDirectory.Source.Root;
                if (cppModule.ProjectDirectory.Root == Global.EngineDirectory.Root)
                {
                    if (engineCppProject == null)
                    {
                        engineCppProject = new VisualCXXProject(workspace, cppModule.ProjectDirectory, "AE", SourceDirectory, "Engine");
                        vsProjects.Add(engineCppProject);
                    }
                }
                else
                {
                    if (gameCppProject == null)
                    {
                        gameCppProject = new VisualCXXProject(workspace, cppModule.ProjectDirectory, cppModule.ProjectDirectory.Name, SourceDirectory, "Game");
                        vsProjects.Add(gameCppProject);
                    }
                }

                modules.Add(new SearchedModule
                {
                    Rules = cppModule.GenerateModuleRule(Solution.Rules),
                    ProjectDirectory = cppModule.ProjectDirectory,
                    SourcePath = cppModule.SourcePath
                });
            }

            var modulesDict = modules.ToDictionary(p => p.Rules.Name, p => p);
            foreach (var innerCppModule in workspace.GetCXXModules())
            {
                ModuleDependencyCache.BuildCache(innerCppModule.Name);
            }
        }

        List<Task> tasks = new();
        foreach (var vsProject in vsProjects)
        {
            vsProject.ResolveDependencies(vsProjects);
            tasks.Add(vsProject.GenerateProjectFilesAsync(cancellationToken));
        }

        tasks.Add(IOUtility.CompareAndWriteAsync(solutionFile, GenerateSolution(vsProjects), cancellationToken));
        await Task.WhenAll(tasks);
    }

    private string GenerateSolution(List<VisualStudioProject> VSProjects)
    {
        const string FilterGUID = "2150E333-8FDC-42A3-9474-1A3956D46DE8";
        const string VisualCPPGUID = "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942";
        const string VisualCSharpGUID = "9A19103F-16F7-4668-BE54-9A1E7A4F7556";

        StringBuilder Builder = new();
        Builder.AppendLine("Microsoft Visual Studio Solution File, Format Version 12.00");
        Builder.AppendLine("# Visual Studio Version 17");
        Builder.AppendLine("VisualStudioVersion = 17.2.32602.215");
        Builder.AppendLine("MinimumVisualStudioVersion = 10.0.40219.1");

        Dictionary<string, string> Filters = new();
        Dictionary<string, string> FilterDepends = new();
        void AddFilter(string FilterPath)
        {
            bool bHasParent = VisualStudioProjectExtensions.TryGetFilterPaths(FilterPath, out string Name, out string ParentPath);

            string Guid = CRC32.GenerateGuid(Name).ToString().ToUpper();
            Filters.Add(Name, Guid);
            Builder.AppendLine($"Project(\"{{{FilterGUID}}}\") = \"{Name}\", \"{Name}\", \"{{{Guid}}}\"");
            Builder.AppendLine("EndProject");

            if (bHasParent)
            {
                FilterDepends.Add(Name, ParentPath);
            }
        }

        foreach (var FilterPath in VSProjects.Select(p => p.filter).Distinct())
        {
            AddFilter(FilterPath);
        }

        void AddVisualStudioProject(VisualStudioProject VSProject)
        {
            string TypeGuid = VSProject switch
            {
                VisualCSharpProject => VisualCSharpGUID,
                VisualCXXProject => VisualCPPGUID,
                _ => throw new InvalidOperationException("Internal error.")
            };

            Builder.AppendLine($"Project(\"{{{TypeGuid}}}\") = \"{VSProject.name}\", \"{VSProject.project}\", \"{{{VSProject.guid}}}\"");
            Builder.AppendLine("EndProject");
        }

        foreach (var CSProject in VSProjects)
        {
            AddVisualStudioProject(CSProject);
        }

        Builder.AppendLine("Global");
        {
            Builder.AppendLine("\tGlobalSection(SolutionConfigurationPlatforms) = preSolution");
            BuildConfiguration.ForEach((Configuration, bEditor, Platform) =>
            {
                string App = bEditor ? " Editor" : string.Empty;
                Builder.AppendLine($"\t\t{Configuration}{App}|{Platform} = {Configuration}{App}|{Platform}");
            });
            Builder.AppendLine("\tEndGlobalSection");

            Builder.AppendLine("\tGlobalSection(ProjectConfigurationPlatforms) = postSolution");
            foreach (var Project in VSProjects)
            {
                BuildConfiguration.ForEach((Configuration, bEditor, Platform) =>
                {
                    string App = bEditor ? " Editor" : string.Empty;
                    var Mapped = Project.MapConfiguration(Configuration, bEditor, Platform);

                    Builder.AppendLine($"\t\t{{{Project.guid}}}.{Configuration}{App}|{Platform}.ActiveCfg = {Mapped.Item1}|{Mapped.Item2}");
                    Builder.AppendLine($"\t\t{{{Project.guid}}}.{Configuration}{App}|{Platform}.Build.0 = {Mapped.Item1}|{Mapped.Item2}");
                });
            }
            Builder.AppendLine("\tEndGlobalSection");

            Builder.AppendLine("\tGlobalSection(NestedProjects) = preSolution");
            foreach (var Project in VSProjects)
            {
                Project.TryGetFilterPaths(out string FilterName, out _);
                if (Filters.TryGetValue(FilterName, out string? FilterGuid))
                {
                    Builder.AppendLine($"\t\t{{{Project.guid}}} = {{{FilterGuid}}}");
                }
            }
            foreach (var (Name, Parent) in FilterDepends)
            {
                string MyGuid = Filters[Name];
                string ParentGuid = Filters[Parent];
                Builder.AppendLine($"\t\t{{{MyGuid}}} = {{{ParentGuid}}}");
            }
            Builder.AppendLine("\tEndGlobalSection");

            Builder.AppendLine("\tGlobalSection(ExtensibilityGlobals) = postSolution");
            Builder.AppendLine($"\t\tSolutionGuid = {{{solutionGuid.ToString().ToUpper()}}}");
            Builder.AppendLine("\tEndGlobalSection");
        }
        Builder.AppendLine("EndGlobal");

        return Builder.ToString();
    }
}
