﻿// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Text;

using AE.BuildSettings;
using AE.Extensions;
using AE.Misc;
using AE.Projects;

namespace AE.ProjectFiles.VisualStudio;

public class VisualStudioSolution : Solution
{
    private readonly string SolutionGuid;
    private readonly string SolutionFile;

    public VisualStudioSolution(Workspace InWorkspace, string? ProjectFile) : base(InWorkspace)
    {
        string SolutionName;
        if (ProjectFile == null)
        {
            SolutionName = "Engine";
        }
        else
        {
            SolutionName = Path.GetFileNameWithoutExtension(ProjectFile);
        }

        string SolutionGuidKey = "Solution_" + Path.GetFileNameWithoutExtension(ProjectFile);
        SolutionGuid = CRC32.GenerateGuid(SolutionGuidKey).ToString().ToUpper();
        SolutionFile = Path.Combine(InWorkspace.CurrentTarget.Root, Path.ChangeExtension(SolutionName, ".sln"));
    }

    public async override Task GenerateProjectFilesAsync(CancellationToken SToken = default)
    {
        List<IVisualStudioProject> VSProjects = new();
        foreach (var CSModule in workspace.GetCSModules())
        {
            VSProjects.Add(new VisualCSharpProject(CSModule, false, false));
        }

        foreach (var CSModule in workspace.GetScriptModules())
        {
            VSProjects.Add(new VisualCSharpProject(CSModule, true, false));
            VSProjects.Add(new VisualCSharpProject(CSModule, true, true));
        }

        VisualCXXProject? EngineCXXProject = null;
        VisualCXXProject? GameCXXProject = null;
        foreach (var CXXModule in workspace.GetCXXModules())
        {
            if (CXXModule.IsInProgramsDirectory)
            {
                VSProjects.Add(new VisualCXXProject(workspace, CXXModule.ProjectDirectory, CXXModule.ModuleName, CXXModule.SourcePath, "Programs"));
            }
            else
            {
                string SourceDirectory = CXXModule.ProjectDirectory.Source.Root;
                if (CXXModule.ProjectDirectory.Root == Global.EngineDirectory.Root)
                {
                    if (EngineCXXProject == null)
                    {
                        EngineCXXProject = new VisualCXXProject(workspace, CXXModule.ProjectDirectory, "AE", SourceDirectory, "Engine");
                        VSProjects.Add(EngineCXXProject);
                    }
                }
                else
                {
                    if (GameCXXProject == null)
                    {
                        GameCXXProject = new VisualCXXProject(workspace, CXXModule.ProjectDirectory, CXXModule.ProjectDirectory.Name, SourceDirectory, "Game");
                        VSProjects.Add(GameCXXProject);
                    }
                }
            }
        }

        List<Task> Tasks = new();
        foreach (var VSProject in VSProjects)
        {
            VSProject.ResolveDependencies(VSProjects);
            Tasks.Add(VSProject.GenerateProjectFilesAsync(SToken));
        }

        Tasks.Add(IOExtensions.CompareAndWriteAsync(SolutionFile, GenerateSolution(VSProjects), SToken));
        await Task.WhenAll(Tasks);
    }

    private string GenerateSolution(List<IVisualStudioProject> VSProjects)
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

        foreach (var FilterPath in VSProjects.Select(p => p.FilterPath).Distinct())
        {
            AddFilter(FilterPath);
        }

        void AddVisualStudioProject(IVisualStudioProject VSProject)
        {
            string TypeGuid = VSProject switch
            {
                VisualCSharpProject => VisualCSharpGUID,
                VisualCXXProject => VisualCPPGUID,
                _ => throw new InvalidOperationException("Internal error.")
            };

            Builder.AppendLine($"Project(\"{{{TypeGuid}}}\") = \"{VSProject.TargetName}\", \"{VSProject.ProjectFile}\", \"{{{VSProject.ProjectGuid}}}\"");
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

                    Builder.AppendLine($"\t\t{{{Project.ProjectGuid}}}.{Configuration}{App}|{Platform}.ActiveCfg = {Mapped.Item1}|{Mapped.Item2}");
                    Builder.AppendLine($"\t\t{{{Project.ProjectGuid}}}.{Configuration}{App}|{Platform}.Build.0 = {Mapped.Item1}|{Mapped.Item2}");
                });
            }
            Builder.AppendLine("\tEndGlobalSection");

            Builder.AppendLine("\tGlobalSection(NestedProjects) = preSolution");
            foreach (var Project in VSProjects)
            {
                Project.TryGetFilterPaths(out string FilterName, out _);
                if (Filters.TryGetValue(FilterName, out string? FilterGuid))
                {
                    Builder.AppendLine($"\t\t{{{Project.ProjectGuid}}} = {{{FilterGuid}}}");
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
            Builder.AppendLine($"\t\tSolutionGuid = {{{SolutionGuid.ToString().ToUpper()}}}");
            Builder.AppendLine("\tEndGlobalSection");
        }
        Builder.AppendLine("EndGlobal");

        return Builder.ToString();
    }
}
