﻿// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.Diagnostics;
using AE.Misc;
using AE.Projects;
using AE.Source;

using System;
using System.Text;
using System.Xml;

namespace AE.ProjectFiles.VisualStudio;

public class VisualStudioSolution : ISolution
{
    private readonly Workspace TargetWorkspace;
    private readonly List<VisualCXXProject> CXXProjects = new();
    private readonly List<VisualCSharpProject> CSProjects = new();
    private readonly Guid SolutionGuid;

    public VisualStudioSolution(Workspace InWorkspace)
    {
        TargetWorkspace = InWorkspace;
        SolutionGuid = CRC32.GenerateGuid("Solution_" + InWorkspace.TargetName);
    }

    public void AddProject(IProject Project)
    {
        if (Project is VisualCXXProject VCXProj)
        {
            CXXProjects.Add(VCXProj);
        }
        else if (Project is VisualCSharpProject CSProj)
        {
            CSProjects.Add(CSProj);
        }
        else
        {
            throw new InvalidOperationException(CoreStrings.Errors.InvalidProjectFormat);
        }
    }

    public async Task GenerateProjectFilesAsync(string ProjectPath, CancellationToken CToken = default)
    {
        SourceCodeDirectory TargetDirectory = TargetWorkspace.TargetDirectory;

        foreach (var Project in CXXProjects)
        {
            string VcxprojName = Path.ChangeExtension(Project.Name, ".vcxproj");
            string VcxprojFiltersName = Path.ChangeExtension(Project.Name, ".vcxproj.filters");
            Project.GenerateXmlDocument(TargetWorkspace, out XmlDocument Vcxproj, out XmlDocument VcxprojFilters);
            await CompareAndWriteAsync(Path.Combine(TargetDirectory.ProjectFilesDirectory, VcxprojName), Vcxproj.Serialize(true), CToken);
            await CompareAndWriteAsync(Path.Combine(TargetDirectory.ProjectFilesDirectory, VcxprojFiltersName), VcxprojFilters.Serialize(true), CToken);
        }

        string SolutionFilename = Path.ChangeExtension(TargetWorkspace.TargetName, ".sln");
        await CompareAndWriteAsync(Path.Combine(ProjectPath, SolutionFilename), GenerateSolution(), CToken);
    }

    private static async Task CompareAndWriteAsync(string Filename, string Text, CancellationToken CToken)
    {
        if (File.Exists(Filename) == false)
        {
            await File.WriteAllTextAsync(Filename, Text, CToken);
            return;
        }

        string PreviousText = await File.ReadAllTextAsync(Filename, CToken);
        if (PreviousText.Trim() == Text.Trim())
        {
            return;
        }

        await File.WriteAllTextAsync(Filename, Text, CToken);
    }

    private string GenerateSolution()
    {
        StringBuilder Builder = new();
        Builder.AppendLine("Microsoft Visual Studio Solution File, Format Version 12.00");
        Builder.AppendLine("# Visual Studio Version 17");
        Builder.AppendLine("VisualStudioVersion = 17.2.32602.215");
        Builder.AppendLine("MinimumVisualStudioVersion = 10.0.40219.1");

        Dictionary<string, string> Filters = new();
        foreach (var Project in CXXProjects)
        {
            Filters.TryAdd(Project.FilterPath, CRC32.GenerateGuid(Project.FilterPath).ToString().ToUpper());
        }
        foreach (var Project in CSProjects)
        {
            Filters.TryAdd(Project.FilterPath, CRC32.GenerateGuid(Project.FilterPath).ToString().ToUpper());
        }

        const string FilterGUID = "2150E333-8FDC-42A3-9474-1A3956D46DE8";
        const string VisualCPPGUID = "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942";
        const string VisualCSharpGUID = "9A19103F-16F7-4668-BE54-9A1E7A4F7556";

        foreach (var (Path, GUID) in Filters)
        {
            Builder.AppendLine($"Project(\"{{{FilterGUID}}}\") = \"{Path}\", \"{Path}\", \"{{{GUID}}}\"");
            Builder.AppendLine("EndProject");
        }

        foreach (var Project in CXXProjects)
        {
            string Filename = Path.ChangeExtension(Project.Name, ".vcxproj");
            Filename = Path.Combine(TargetWorkspace.TargetDirectory.ProjectFilesDirectory, Filename);
            Builder.AppendLine($"Project(\"{{{VisualCPPGUID}}}\") = \"{Project.Name}\", \"{Filename}\", \"{{{Project.ProjectGuid}}}\"");
            Builder.AppendLine("EndProject");
        }

        foreach (var Project in CSProjects)
        {
            string Filename = Project.ProjectFile;
            Builder.AppendLine($"Project(\"{{{VisualCSharpGUID}}}\") = \"{Project.Name}\", \"{Filename}\", \"{{{Project.ProjectGuid}}}\"");
            Builder.AppendLine("EndProject");
        }

        Builder.AppendLine("Global");

        Builder.AppendLine("\tGlobalSection(SolutionConfigurationPlatforms) = preSolution");
        BuildConfiguration.ForEach((Configuration, Platform) =>
        {
            Builder.AppendLine($"\t\t{Configuration}|{Platform} = {Configuration}|{Platform}");
        });
        Builder.AppendLine("\tEndGlobalSection");

        Builder.AppendLine("\tGlobalSection(ProjectConfigurationPlatforms) = postSolution");
        foreach (var Project in CXXProjects)
        {
            BuildConfiguration.ForEach((Configuration, Platform) =>
            {
                Builder.AppendLine($"\t\t{{{Project.ProjectGuid}}}.{Configuration}|{Platform}.ActiveCfg = {Configuration}|{Platform}");
                Builder.AppendLine($"\t\t{{{Project.ProjectGuid}}}.{Configuration}|{Platform}.Build.0 = {Configuration}|{Platform}");
            });
        }
        foreach (var Project in CSProjects)
        {
            BuildConfiguration.ForEach((Configuration, Platform) =>
            {
                string ActualConf = Configuration == Configuration.Debug ? "Debug" : "Release";
                Builder.AppendLine($"\t\t{{{Project.ProjectGuid}}}.{Configuration}|{Platform}.ActiveCfg = {ActualConf}|Any CPU");
                Builder.AppendLine($"\t\t{{{Project.ProjectGuid}}}.{Configuration}|{Platform}.Build.0 = {ActualConf}|Any CPU");
            });
        }
        Builder.AppendLine("\tEndGlobalSection");

        Builder.AppendLine("\tGlobalSection(SolutionProperties) = preSolution");
        Builder.AppendLine("\t\tHideSolutionNode = FALSE");
        Builder.AppendLine("\tEndGlobalSection");

        Builder.AppendLine("\tGlobalSection(NestedProjects) = preSolution");
        foreach (var Project in CXXProjects)
        {
            if (Filters.TryGetValue(Project.FilterPath, out string? FilterGuid))
            {
                Builder.AppendLine($"\t\t{{{Project.ProjectGuid}}} = {{{FilterGuid}}}");
            }
        }
        foreach (var Project in CSProjects)
        {
            if (Filters.TryGetValue(Project.FilterPath, out string? FilterGuid))
            {
                Builder.AppendLine($"\t\t{{{Project.ProjectGuid}}} = {{{FilterGuid}}}");
            }
        }
        Builder.AppendLine("\tEndGlobalSection");

        Builder.AppendLine("\tGlobalSection(ExtensibilityGlobals) = postSolution");
        Builder.AppendLine($"\t\tSolutionGuid = {{{SolutionGuid.ToString().ToUpper()}}}");
        Builder.AppendLine("\tEndGlobalSection");

        Builder.AppendLine("EndGlobal");

        return Builder.ToString();
    }
}