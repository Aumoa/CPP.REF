using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using Microsoft.CodeAnalysis.CSharp.Syntax;

namespace AylaEngine;

internal class VSSolutionGenerator : Generator
{
    private static readonly Version SolutionVersion = new Version(12, 0);
    private static readonly Version VisualStudioVersion = new Version(17, 2, 32602, 215);
    private static readonly Version MinimumVisualStudioVersion = new Version(10, 0, 40219, 1);

    private static readonly Guid CSharpProjectGuid = Guid.Parse("9A19103F-16F7-4668-BE54-9A1E7A4F7556");
    private static readonly Guid FilterGuid = Guid.Parse("2150E333-8FDC-42A3-9474-1A3956D46DE8");
    private static readonly Guid CppProjectGuid = Guid.Parse("8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942");

    private static readonly Guid GameFilterGuid = Guid.Parse("284F6D27-09DC-6D2D-272C-0D08CC082D4C");
    private static readonly Guid EngineFilterGuid = Guid.Parse("A33C819E-1D5B-1772-8E80-2C824B1C2016");
    private static readonly Guid EngineRuntimeFilterGuid = Guid.Parse("87D7F3FA-72BF-49B6-EAB2-8586AF32A448");
    private static readonly Guid EngineEditorFilterGuid = Guid.Parse("F25589A2-561D-BD3C-A288-05D20D162C9C");
    private static readonly Guid ProgramFilterGuid = Guid.Parse("C39416C3-6B11-1C22-C316-84C2012A201C");

    public override async Task GenerateAsync(Solution solution, CancellationToken cancellationToken = default)
    {
        var solutionFileName = Path.Combine(solution.PrimaryGroup.RootDirectory, solution.PrimaryGroup.Name + ".sln");
        string? previousSolutionText = null;
        if (File.Exists(solutionFileName))
        {
            previousSolutionText = await File.ReadAllTextAsync(solutionFileName, cancellationToken);
        }

        Guid solutionGuid = default;
        if (previousSolutionText != null)
        {
            var match = Regex.Match(previousSolutionText, @"SolutionGuid = ([{(]?[0-9A-F]{8}[-]?(?:[0-9A-F]{4}[-]?){3}[0-9A-F]{12}[)}])?", RegexOptions.Multiline);
            if (match.Groups.Count > 1)
            {
                solutionGuid = Guid.Parse(match.Groups[1].Value);
            }
        }

        if (solutionGuid == default)
        {
            solutionGuid = Guid.NewGuid();
        }

        Dictionary<ModuleProject, string> vcxprojPaths = new();
        Dictionary<ModuleProject, string> scriptProjectPaths = new();
        List<Task> tasks = [];
        foreach (var project in solution.Projects.OfType<ModuleProject>())
        {
            tasks.Add(VSCppProjectGenerator.GenerateAsync(solution, vcxprojPaths, project, cancellationToken));
            tasks.Add(ScriptProjectGenerator.GenerateAsync(solution, scriptProjectPaths, project, cancellationToken));
        }

        await Task.WhenAll(tasks);

        var builder = new StringBuilder();
        PartOfHeader();
        PartOfProjects();
        PartOfGlobal();

        var solutionText = builder.ToString();
        if (previousSolutionText?.Replace("\r\n", "\n") != solutionText)
        {
            await File.WriteAllTextAsync(solutionFileName, solutionText.Replace("/", Environment.NewLine), cancellationToken);
        }

        return;

        void PartOfHeader()
        {
            builder.AppendFormat("Microsoft Visual Studio Solution File, Format Version {0:00}.{1:00}\n", SolutionVersion.Major, SolutionVersion.Minor);
            builder.AppendFormat("# Visual Studio Version {0}\n", VisualStudioVersion.ToString(1));
            builder.AppendFormat("VisualStudioVersion = {0}\n", VisualStudioVersion.ToString(4));
            builder.AppendFormat("MinimumVisualStudioVersion = {0}\n", MinimumVisualStudioVersion.ToString(4));
        }

        void PartOfProjects()
        {
            var primaryProject = solution.Projects.FirstOrDefault(p => p.Name == solution.PrimaryGroup.Name);
            if (primaryProject != null)
            {
                WriteProject(primaryProject);
            }

            foreach (var project in solution.Projects)
            {
                if (project.Name != solution.PrimaryGroup.Name)
                {
                    WriteProject(project);
                }
            }

            WriteFilter("Engine", EngineFilterGuid);
            WriteFilter("Runtime", EngineRuntimeFilterGuid);
            WriteFilter("Editor", EngineEditorFilterGuid);
            WriteFilter("Program", ProgramFilterGuid);
            if (solution.Projects.Any(p => p.IsEngine == false))
            {
                WriteFilter("Game", GameFilterGuid);
            }
        }

        void PartOfGlobal()
        {
            builder.AppendFormat("Global\n");
            PartOfGlobalSectionSolutionConfigurationPlatforms();
            PartOfGlobalSectionProjectConfigurationPlatforms();
            PartOfGlobalSectionSolutionProperties();
            PartOfGlobalSectionNestedProjects();
            PartOfGlobalSectionExtensibilityGlobals();
            builder.AppendFormat("EndGlobal\n");
        }

        void PartOfGlobalSectionSolutionConfigurationPlatforms()
        {
            builder.AppendFormat("\tGlobalSection(SolutionConfigurationPlatforms) = preSolution\n");
            foreach (var buildConfig in TargetInfo.GetAllTargets())
            {
                // Visual Studio only support Windows platform.
                if (buildConfig.Platform.Group != PlatformGroup.Windows)
                {
                    continue;
                }

                builder.AppendFormat("\t\t{0}|{1} = {0}|{1}\n", VSUtility.GetConfigName(buildConfig), GetPlatformName(buildConfig.Platform));
            }
            builder.AppendFormat("\tEndGlobalSection\n");
        }

        void PartOfGlobalSectionProjectConfigurationPlatforms()
        {
            builder.AppendFormat("\tGlobalSection(ProjectConfigurationPlatforms) = preSolution\n");
            foreach (var project in solution.Projects)
            {
                foreach (var buildConfig in TargetInfo.GetAllTargets())
                {
                    // Visual Studio only support Windows platform.
                    if (buildConfig.Platform.Group != PlatformGroup.Windows)
                    {
                        continue;
                    }

                    switch (project)
                    {
                        case ModuleProject mp:
                            builder.AppendFormat("\t\t{0}.{1}|{2}.ActiveCfg = {3}|{4}\n", mp.Decl.Guid.ToString("B").ToUpper(), VSUtility.GetConfigName(buildConfig), GetPlatformName(buildConfig.Platform), VSUtility.GetConfigName(buildConfig), VSUtility.GetArchitectureName(buildConfig));
                            builder.AppendFormat("\t\t{0}.{1}|{2}.Build.0 = {3}|{4}\n", mp.Decl.Guid.ToString("B").ToUpper(), VSUtility.GetConfigName(buildConfig), GetPlatformName(buildConfig.Platform), VSUtility.GetConfigName(buildConfig), VSUtility.GetArchitectureName(buildConfig));
                            builder.AppendFormat("\t\t{0}.{1}|{2}.ActiveCfg = {3}|{4}\n", mp.Decl.ScriptGuid.ToString("B").ToUpper(), VSUtility.GetConfigName(buildConfig), GetPlatformName(buildConfig.Platform), VSUtility.GetConfigName(buildConfig), VSUtility.GetArchitectureName(buildConfig));
                            builder.AppendFormat("\t\t{0}.{1}|{2}.Build.0 = {3}|{4}\n", mp.Decl.ScriptGuid.ToString("B").ToUpper(), VSUtility.GetConfigName(buildConfig), GetPlatformName(buildConfig.Platform), VSUtility.GetConfigName(buildConfig), VSUtility.GetArchitectureName(buildConfig));
                            break;
                        case ProgramProject:
                            builder.AppendFormat("\t\t{0}.{1}|{2}.ActiveCfg = {3}|Any CPU\n", project.Decl.Guid.ToString("B").ToUpper(), VSUtility.GetConfigName(buildConfig), GetPlatformName(buildConfig.Platform), GetCSharpConfigName(buildConfig.Config));
                            builder.AppendFormat("\t\t{0}.{1}|{2}.Build.0 = {3}|Any CPU\n", project.Decl.Guid.ToString("B").ToUpper(), VSUtility.GetConfigName(buildConfig), GetPlatformName(buildConfig.Platform), GetCSharpConfigName(buildConfig.Config));
                            break;
                    }
                }
            }
            builder.AppendFormat("\tEndGlobalSection\n");
        }

        void PartOfGlobalSectionSolutionProperties()
        {
            builder.AppendFormat("\tGlobalSection(SolutionProperties) = preSolution\n");
            builder.AppendFormat("\t\tHideSolutionNode = FALSE\n");
            builder.AppendFormat("\tEndGlobalSection\n");
        }

        void PartOfGlobalSectionNestedProjects()
        {
            builder.AppendFormat("\tGlobalSection(NestedProjects) = preSolution\n");
            AddNested(EngineEditorFilterGuid, EngineFilterGuid);
            AddNested(EngineRuntimeFilterGuid, EngineFilterGuid);
            foreach (var project in solution.Projects)
            {
                if (project is ProgramProject)
                {
                    AddNested(project.Decl.Guid, ProgramFilterGuid);
                }
                else if (project is ModuleProject mp)
                {
                    var directoryName = mp.SourceDirectory;
                    directoryName = directoryName.Replace(mp.Group.RootDirectory, string.Empty);
                    if (project.Group.Name == "Engine")
                    {
                        if (directoryName.Replace('\\', '/').Contains("/Editor/"))
                        {
                            AddNested(mp.Decl.Guid, EngineEditorFilterGuid);
                            AddNested(mp.Decl.ScriptGuid, EngineEditorFilterGuid);
                        }
                        else
                        {
                            AddNested(mp.Decl.Guid, EngineRuntimeFilterGuid);
                            AddNested(mp.Decl.ScriptGuid, EngineRuntimeFilterGuid);
                        }
                    }
                    else
                    {
                        AddNested(mp.Decl.Guid, GameFilterGuid);
                        AddNested(mp.Decl.ScriptGuid, GameFilterGuid);
                    }
                }
            }
            builder.AppendFormat("\tEndGlobalSection\n");

            return;

            void AddNested(Guid item, Guid parent)
            {
                builder.AppendFormat("\t\t{0} = {1}\n", item.ToString("B").ToUpper(), parent.ToString("B").ToUpper());
            }
        }

        void PartOfGlobalSectionExtensibilityGlobals()
        {
            builder.AppendFormat("\tGlobalSection(ExtensibilityGlobals) = postSolution\n");
            builder.AppendFormat("\t\tSolutionGuid = {0}\n", solutionGuid.ToString("B").ToUpper());
            builder.AppendFormat("\tEndGlobalSection\n");
        }

        void WriteProject(Project project)
        {
            switch (project)
            {
                case ProgramProject pp:
                    WriteProgramProject(pp);
                    break;
                case ModuleProject mp:
                    WriteModuleProject(mp);
                    break;
                default:
                    throw new InvalidOperationException();
            }
        }

        void WriteProgramProject(ProgramProject project)
        {
            builder.AppendFormat("Project(\"{0}\") = \"{1}\", \"{2}\", \"{3}\"\n", CSharpProjectGuid.ToString("B").ToUpper(), project.Name, project.ProjectFilePath.Replace('/', '\\'), project.Decl.Guid.ToString("B").ToUpper());
            builder.AppendFormat("EndProject\n");
        }

        void WriteModuleProject(ModuleProject project)
        {
            string projectFilePath;
            string? scriptFilePath;
            lock (vcxprojPaths)
            {
                projectFilePath = vcxprojPaths[project];
            }
            lock (scriptProjectPaths)
            {
                scriptProjectPaths.TryGetValue(project, out scriptFilePath);
            }

            builder.AppendFormat("Project(\"{0}\") = \"{1}\", \"{2}\", \"{3}\"\n", CppProjectGuid.ToString("B").ToUpper(), project.Name, projectFilePath.Replace('/', '\\'), project.Decl.Guid.ToString("B").ToUpper());
            List<string> projectDependencies = [];
            if (project.Name == "Launch")
            {
                var scriptingLaunch = solution.Projects.First(p => p.Name == "ScriptingLaunch");
                projectDependencies.Add(scriptingLaunch.Decl.Guid.ToString("B").ToUpper());
            }
            if (projectDependencies.Count > 0)
            {
                builder.AppendFormat("  ProjectSection(ProjectDependencies) = postProject\n");
                foreach (var dependency in projectDependencies)
                {
                    builder.AppendFormat("    {0} = {0}\n", dependency);
                }
                builder.AppendFormat("  EndProjectSection");
            }
            builder.AppendFormat("EndProject\n");

            if (scriptFilePath != null)
            {
                builder.AppendFormat("Project(\"{0}\") = \"{1}.Script\", \"{2}\", \"{3}\"\n", CSharpProjectGuid.ToString("B").ToUpper(), project.Name, scriptFilePath.Replace('/', '\\'), project.Decl.ScriptGuid.ToString("B").ToUpper());
                builder.AppendFormat("EndProject\n");
            }
        }

        void WriteFilter(string name, Guid guid)
        {
            builder.AppendFormat("Project(\"{0}\") = \"{1}\", \"{1}\", \"{2}\"\n", FilterGuid.ToString("B").ToUpper(), name, guid.ToString("B").ToUpper());
            builder.AppendFormat("EndProject\n");
        }

        static string GetPlatformName(PlatformInfo value)
        {
            if (value == PlatformInfo.Win64)
            {
                return "Win64";
            }
            else
            {
                throw new NotImplementedException();
            }
        }

        static string GetCSharpConfigName(Configuration value)
        {
            return value == Configuration.Debug ? "Debug" : "Release";
        }
    }
}
