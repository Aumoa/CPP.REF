// Copyright 2020-2024 Aumoa.lib. All right reserved.

using System.Text;

namespace AylaEngine;

public class VisualStudioSolution
{
    private const string FormatVersion = "12.00";
    private const string VisualStudioVersion = "17.2.32602.215";
    private const string MinimumVisualStudioVersion = "10.0.40219.1";

    private const string FilterGuid = "2150E333-8FDC-42A3-9474-1A3956D46DE8";
    private const string VisualCppGuid = "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942";
    private const string VisualCSharpGuid = "9A19103F-16F7-4668-BE54-9A1E7A4F7556";

    public static async Task GenerateProjectFilesAsync(CancellationToken cancellationToken = default)
    {
        string solutionName;
        if (Workspace.IsTargetEngine)
        {
            solutionName = "Engine";
        }
        else
        {
            solutionName = Workspace.ProjectFile.Name;
        }

        string solutionGuid = VSUtility.MakeGUID($"Solution_{solutionName}");
        FileReference solutionFile = Workspace.ProjectTarget.Root.GetFile(solutionName).WithExtensions("sln");

        List<VisualStudioProject> vsProjects = new();
        foreach (var csModule in Workspace.CSharpAssemblies)
        {
            vsProjects.Add(new VisualCSharpProject(csModule));
        }
        foreach (var cppModule in Workspace.CppAssemblies)
        {
            vsProjects.Add(new VisualCppProject(cppModule));
        }
        foreach (var interopModule in Workspace.InteropAssemblies)
        {
            vsProjects.Add(new VisualInteropProject(interopModule));
        }

        List<Task> tasks = new();
        foreach (var vsProject in vsProjects)
        {
            tasks.Add(vsProject.GenerateProjectFilesAsync(vsProjects, cancellationToken));
        }

        tasks.Add(IOUtility.CompareAndWriteAsync(solutionFile, GenerateSolution(vsProjects, solutionGuid), cancellationToken));
        await Task.WhenAll(tasks);
    }

    private static void AddProject(StringBuilder projects, string typeGuid, string name, string path, string guid)
    {
        projects.AppendLine($"Project(\"{{{typeGuid}}}\") = \"{name}\", \"{path}\", \"{{{guid}}}\"");
        projects.AppendLine("EndProject");
    }

    private static void AddNested(StringBuilder builder, string guid, string parentGuid)
    {
        builder.AppendLine($"    {{{guid}}} = {{{parentGuid}}}");
    }

    private static string GetFilterGuid(string filterPath, StringBuilder projects, StringBuilder nestedProjects, Dictionary<string, string> projectGuids)
    {
        filterPath = filterPath.Replace("/.", string.Empty).Replace("./", string.Empty);

        if (projectGuids.TryGetValue(filterPath, out string? guid) == false)
        {
            guid = VSUtility.MakeGUID(filterPath);
            VSUtility.TryGetFilterPaths(filterPath, out string name, out string parent);
            AddProject(projects, FilterGuid, name, name, guid);
            if (string.IsNullOrEmpty(parent) == false)
            {
                if (projectGuids.TryGetValue(parent, out var parentGuid) == false)
                {
                    parentGuid = GetFilterGuid(parent, projects, nestedProjects, projectGuids);
                }
                AddNested(nestedProjects, guid, parentGuid);
            }

            projectGuids.Add(filterPath, guid);
        }

        return guid;
    }

    private static string GenerateSolution(List<VisualStudioProject> projects, string solutionGuid)
    {
        string headerSection = $"""
Microsoft Visual Studio Solution File, Format Version {FormatVersion}
VisualStudioVersion = {VisualStudioVersion}
MinimumVisualStudioVersion = {MinimumVisualStudioVersion}
""";
        StringBuilder projectsSection = new();
        StringBuilder nestedProjects = new();
        StringBuilder solutionConfigurationPlatforms = new();
        StringBuilder projectConfigurationPlatforms = new();
        Dictionary<string, string> projectGuids = new();

        foreach (var project in projects.OrderByDescending(p => p.Name == Workspace.Current.Name))
        {
            string typeGuid = project switch
            {
                VisualCSharpProject or VisualInteropProject => VisualCSharpGuid,
                VisualCppProject => VisualCppGuid,
                _ => throw new InvalidOperationException("Internal error.")
            };

            AddProject(projectsSection, typeGuid, project.Name, project.Project.AbsolutePath, project.GUID);
            AddNested(nestedProjects, project.GUID, GetFilterGuid(project.Filter, projectsSection, nestedProjects, projectGuids));
        }

        BuildConfiguration.ForEach((Configuration, bEditor, Platform) =>
        {
            string App = bEditor ? " Editor" : string.Empty;
            solutionConfigurationPlatforms.AppendLine($"\t\t{Configuration}{App}|{Platform} = {Configuration}{App}|{Platform}");
        });

        foreach (var project in projects)
        {
            BuildConfiguration.ForEach((Configuration, bEditor, Platform) =>
            {
                string App = bEditor ? " Editor" : string.Empty;
                var Mapped = project.MapConfiguration(Configuration, bEditor, Platform);

                projectConfigurationPlatforms.AppendLine($"\t\t{{{project.GUID}}}.{Configuration}{App}|{Platform}.ActiveCfg = {Mapped.Item1}|{Mapped.Item2}");
                projectConfigurationPlatforms.AppendLine($"\t\t{{{project.GUID}}}.{Configuration}{App}|{Platform}.Build.0 = {Mapped.Item1}|{Mapped.Item2}");
            });
        }

        StringBuilder builder = new();
        builder.AppendLine(headerSection);
        builder.Append(projectsSection);

        builder.AppendLine("Global");
        {
            builder.AppendLine("\tGlobalSection(SolutionConfigurationPlatforms) = postSolution");
            builder.Append(solutionConfigurationPlatforms);
            builder.AppendLine("\tEndGlobalSection");

            builder.AppendLine("\tGlobalSection(ProjectConfigurationPlatforms) = postSolution");
            builder.Append(projectConfigurationPlatforms);
            builder.AppendLine("\tEndGlobalSection");

            builder.AppendLine("\tGlobalSection(NestedProjects) = preSolution");
            builder.Append(nestedProjects);
            builder.AppendLine("\tEndGlobalSection");

            builder.AppendLine("\tGlobalSection(ExtensibilityGlobals) = postSolution");
            builder.AppendLine($"\t\tSolutionGuid = {{{solutionGuid.ToString().ToUpper()}}}");
            builder.AppendLine("\tEndGlobalSection");
        }
        builder.AppendLine("EndGlobal");

        return builder.ToString();
    }

    private static TargetInfo? s_TargetInfo;

    public static TargetInfo TargetInfo
    {
        get
        {
            s_TargetInfo ??= new TargetInfo()
            {
                BuildConfiguration = new()
                {
                    Configuration = Configuration.Development,
                    Platform = TargetPlatform.Current
                }
            };

            return s_TargetInfo;
        }
    }
}
