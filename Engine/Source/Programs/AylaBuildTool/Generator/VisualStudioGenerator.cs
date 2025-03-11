using System.Text;
using System.Text.RegularExpressions;
using Microsoft.CodeAnalysis.CSharp.Syntax;

namespace AylaEngine;

internal class VisualStudioGenerator : Generator
{
    private static readonly Version SolutionVersion = new Version(12, 0);
    private static readonly Version VisualStudioVersion = new Version(17, 2, 32602, 215);
    private static readonly Version MinimumVisualStudioVersion = new Version(10, 0, 40219, 1);
    private static readonly Version VCProjectVersion = new Version(17, 0);
    private static readonly Version WindowsTargetPlatformVersion = new Version(10, 0);
    private const string PlatformToolset = "v143";

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
        foreach (var project in solution.AllProjects.OfType<ModuleProject>())
        {
            await GenerateCppProjectAsync(vcxprojPaths, solution.PrimaryGroup, project, cancellationToken);
        }

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
            var primaryProject = solution.AllProjects.FirstOrDefault(p => p.Name == solution.PrimaryGroup.Name);
            if (primaryProject != null)
            {
                WriteProject(primaryProject);
            }

            foreach (var project in solution.AllProjects)
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
            if (solution.GameProjects.Any())
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
            foreach (var buildConfig in BuildConfiguration.GetConfigurations())
            {
                // Visual Studio only support Windows platform.
                if (buildConfig.Platform.Group != PlatformGroup.Windows)
                {
                    continue;
                }

                builder.AppendFormat("\t\t{0}|{1} = {0}|{1}\n", GetConfigName(buildConfig), GetPlatformName(buildConfig.Platform));
            }
            builder.AppendFormat("\tEndGlobalSection\n");
        }

        void PartOfGlobalSectionProjectConfigurationPlatforms()
        {
            builder.AppendFormat("\tGlobalSection(ProjectConfigurationPlatforms) = preSolution\n");
            foreach (var project in solution.AllProjects)
            {
                foreach (var buildConfig in BuildConfiguration.GetConfigurations())
                {
                    // Visual Studio only support Windows platform.
                    if (buildConfig.Platform.Group != PlatformGroup.Windows)
                    {
                        continue;
                    }

                    switch (project)
                    {
                        case ModuleProject:
                            builder.AppendFormat("\t\t{0}.{1}|{2}.ActiveCfg = {3}|{4}\n", project.Decl.Guid.ToString("B").ToUpper(), GetConfigName(buildConfig), GetPlatformName(buildConfig.Platform), GetCppConfigName(buildConfig), GetArchitectureName(buildConfig));
                            builder.AppendFormat("\t\t{0}.{1}|{2}.Build.0 = {3}|{4}\n", project.Decl.Guid.ToString("B").ToUpper(), GetConfigName(buildConfig), GetPlatformName(buildConfig.Platform), GetCppConfigName(buildConfig), GetArchitectureName(buildConfig));
                            break;
                        case ProgramProject:
                            builder.AppendFormat("\t\t{0}.{1}|{2}.ActiveCfg = {3}|Any CPU\n", project.Decl.Guid.ToString("B").ToUpper(), GetConfigName(buildConfig), GetPlatformName(buildConfig.Platform), GetCSharpConfigName(buildConfig.Config));
                            builder.AppendFormat("\t\t{0}.{1}|{2}.Build.0 = {3}|Any CPU\n", project.Decl.Guid.ToString("B").ToUpper(), GetConfigName(buildConfig), GetPlatformName(buildConfig.Platform), GetCSharpConfigName(buildConfig.Config));
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
            foreach (var project in solution.AllProjects)
            {
                if (project is ProgramProject)
                {
                    AddNested(project.Decl.Guid, ProgramFilterGuid);
                }
                else if (project is ModuleProject mp)
                {
                    var directoryName = mp.SourceDirectory;
                    directoryName = directoryName.Replace(mp.Descriptor.RootDirectory, string.Empty);
                    if (project.Descriptor.Name == "Engine")
                    {
                        if (directoryName.Replace('\\', '/').Contains("/Editor/"))
                        {
                            AddNested(project.Decl.Guid, EngineEditorFilterGuid);
                        }
                        else
                        {
                            AddNested(project.Decl.Guid, EngineRuntimeFilterGuid);
                        }
                    }
                    else
                    {
                        AddNested(project.Decl.Guid, GameFilterGuid);
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
                    WriteCSProject(pp);
                    break;
                case ModuleProject mp:
                    WriteCppProject(mp);
                    break;
                default:
                    throw new InvalidOperationException();
            }

        }

        void WriteCSProject(ProgramProject project)
        {
            builder.AppendFormat("Project(\"{0}\") = \"{1}\", \"{2}\", \"{3}\"\n", CSharpProjectGuid.ToString("B").ToUpper(), project.Name, project.ProjectFilePath.Replace('/', '\\'), project.Decl.Guid.ToString("B").ToUpper());
            builder.AppendFormat("EndProject\n");
        }

        void WriteCppProject(ModuleProject project)
        {
            string projectFilePath;
            lock (vcxprojPaths)
            {
                projectFilePath = vcxprojPaths[project];
            }
            builder.AppendFormat("Project(\"{0}\") = \"{1}\", \"{2}\", \"{3}\"\n", CppProjectGuid.ToString("B").ToUpper(), project.Name, projectFilePath.Replace('/', '\\'), project.Decl.Guid.ToString("B").ToUpper());
            builder.AppendFormat("EndProject\n");
        }

        void WriteFilter(string name, Guid guid)
        {
            builder.AppendFormat("Project(\"{0}\") = \"{1}\", \"{1}\", \"{2}\"\n", FilterGuid.ToString("B").ToUpper(), name, guid.ToString("B").ToUpper());
            builder.AppendFormat("EndProject\n");
        }

        static string GetArchitectureName(BuildConfiguration value) => value.Platform.Architecture switch
        {
            Architecture.X64 => "x64",
            _ => throw new InvalidOperationException()
        };

        async Task GenerateCppProjectAsync(Dictionary<ModuleProject, string> vcxprojPaths, GroupDescriptor primaryGroup, ModuleProject project, CancellationToken cancellationToken)
        {
            var group = project.Descriptor;
            var projectFilesDirectory = Path.Combine(primaryGroup.IntermediateDirectory, "ProjectFiles");
            await Task.WhenAll(
                GenerateVcxprojAsync(),
                GenerateVcxprojFiltersAsync()
            );

            return;

            async Task GenerateVcxprojFiltersAsync()
            {
                var fileName = Path.Combine(projectFilesDirectory, project.Name + ".vcxproj.filters");
                var builder = new StringBuilder();
                int indent = 0;

                AppendFormatLine("""<?xml version="1.0" encoding="utf-8"?>""");
                AppendFormatLine("""<Project ToolVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">""");
                Indent(() =>
                {
                    AppendFormatLine("""<ItemGroup>""");

                    Dictionary<string, Guid> filters = new();
                    Dictionary<string, SourceCodeDescriptor> sourceFileToFilter = new();
                    foreach (var descriptor in project.GetSourceCodes())
                    {
                        var relativePath = descriptor.RelativeDirectory;
                        var guid = StringToGuid(relativePath);
                        filters.TryAdd(relativePath, guid);
                        sourceFileToFilter.TryAdd(descriptor.FilePath, descriptor);

                        while (true)
                        {
                            var lastIndexOf = relativePath.LastIndexOf('\\');
                            if (lastIndexOf == -1)
                            {
                                break;
                            }

                            relativePath = relativePath[..lastIndexOf];
                            if (relativePath == ".")
                            {
                                break;
                            }

                            guid = StringToGuid(relativePath);
                            filters.TryAdd(relativePath, guid);
                        }
                    }

                    Indent(() =>
                    {
                        foreach (var (name, guid) in filters)
                        {
                            if (string.IsNullOrEmpty(name))
                            {
                                continue;
                            }

                            AppendFormatLine("""<Filter Include="{0}">""", name);
                            Indent(() =>
                            {
                                AppendFormatLine("""<UniqueIdentifier>{0}</UniqueIdentifier>""", guid.ToString("B").ToUpper());
                            });
                            AppendFormatLine("""</Filter>""");
                        }
                    });

                    AppendFormatLine("""</ItemGroup>""");

                    AppendFormatLine("""<ItemGroup>""");

                    Indent(() =>
                    {
                        foreach (var (source, descriptor) in sourceFileToFilter)
                        {
                            Indent(() =>
                            {
                                var cl = GetCompilerType(descriptor);
                                AppendFormatLine("""<{0} Include="{1}">""", cl, source);
                                Indent(() =>
                                {
                                    AppendFormatLine("""<Filter>{0}</Filter>""", descriptor.RelativeDirectory);
                                });
                                AppendFormatLine("""</{0}>""", cl);
                            });
                        }
                    });

                    AppendFormatLine("""</ItemGroup>""");
                });

                AppendFormatLine("""</Project>""");

                string? previousXml = null;
                if (File.Exists(fileName))
                {
                    previousXml = await File.ReadAllTextAsync(fileName, cancellationToken);
                }

                var currentXml = builder.ToString();
                if (previousXml != currentXml)
                {
                    await File.WriteAllTextAsync(fileName, currentXml, cancellationToken);
                }

                return;

                void AppendFormatLine(string format, params ReadOnlySpan<object?> args)
                {
                    var indentStr = new string(' ', indent * 2);
                    builder.AppendFormat(indentStr + format + '\n', args);
                }

                void Indent(Action inner)
                {
                    ++indent;
                    inner();
                    --indent;
                }

                static Guid StringToGuid(string value)
                {
                    ulong c64 = CRC64.Generate64(value);
                    var bytes = BitConverter.GetBytes(c64);
                    return new Guid(bytes.Concat(bytes).ToArray());
                }
            }

            async Task GenerateVcxprojAsync()
            {
                var fileName = Path.Combine(projectFilesDirectory, project.Name + ".vcxproj");
                var builder = new StringBuilder();
                int indent = 0;

                lock (vcxprojPaths)
                {
                    vcxprojPaths.Add(project, fileName);
                }

                AppendFormatLine("""<?xml version="1.0" encoding="utf-8"?>""");
                AppendFormatLine("""<Project DefaultTargets="Build" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">""");
                Indent(() =>
                {
                    AppendFormatLine("""<ItemGroup Label="ProjectConfigurations">""");
                    Indent(() =>
                    {
                        foreach (var buildConfig in BuildConfiguration.GetConfigurations())
                        {
                            // Visual Studio only support Windows platform.
                            if (buildConfig.Platform.Group != PlatformGroup.Windows)
                            {
                                continue;
                            }

                            var configName = GetConfigName(buildConfig);
                            var archName = GetArchitectureName(buildConfig);

                            AppendFormatLine("""<ProjectConfiguration Include="{0}|{1}">""", configName, archName);
                            Indent(() =>
                            {
                                AppendFormatLine("""<Configuration>{0}</Configuration>""", configName);
                                AppendFormatLine("""<Platform>{0}</Platform>""", archName);
                            });
                            AppendFormatLine("""</ProjectConfiguration>""");
                        }
                    });
                    AppendFormatLine("""</ItemGroup>""");

                    AppendFormatLine("""<PropertyGroup Label="Globals">""");
                    Indent(() =>
                    {
                        AppendFormatLine("""<VCProjectVersion>{0}</VCProjectVersion>""", VCProjectVersion);
                        AppendFormatLine("""<Keyword>Win32Proj</Keyword>""");
                        AppendFormatLine("""<ProjectGuid>{0}</ProjectGuid>""", project.Decl.Guid.ToString("B").ToUpper());
                        AppendFormatLine("""<WindowsTargetPlatformVersion>{0}</WindowsTargetPlatformVersion>""", WindowsTargetPlatformVersion);
                    });
                    AppendFormatLine("""</PropertyGroup>""");

                    AppendFormatLine("""<Import Project="$(VCTargetsPath)\Microsoft.Cpp.Default.props" />""");

                    foreach (var buildConfig in BuildConfiguration.GetConfigurations())
                    {
                        // Visual Studio only support Windows platform.
                        if (buildConfig.Platform.Group != PlatformGroup.Windows)
                        {
                            continue;
                        }

                        var configName = GetConfigName(buildConfig);
                        var archName = GetArchitectureName(buildConfig);

                        AppendFormatLine("""<PropertyGroup Condition="'$(Configuration)|$(Platform)'=='{0}|{1}'" Label="Configuration">""", configName, archName);
                        Indent(() =>
                        {
                            AppendFormatLine("""<ConfigurationType>Makefile</ConfigurationType>""");
                            AppendFormatLine("""<PlatformToolset>{0}</PlatformToolset>""", PlatformToolset);
                            AppendFormatLine("""<CharacterSet>Unicode</CharacterSet>""");
                            AppendFormatLine("""<UseDebugLibraries>{0}</UseDebugLibraries>""", LibraryDebugLevel(buildConfig).ToString().ToLower());
                        });
                        AppendFormatLine("""</PropertyGroup>""");
                    }

                    AppendFormatLine("""<Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />""");

                    foreach (var buildConfig in BuildConfiguration.GetConfigurations())
                    {
                        // Visual Studio only support Windows platform.
                        if (buildConfig.Platform.Group != PlatformGroup.Windows)
                        {
                            continue;
                        }

                        var configName = GetConfigName(buildConfig);
                        var archName = GetArchitectureName(buildConfig);
                        var outDir = Path.Combine(group.BinariesDirectory, buildConfig.Platform.Name, buildConfig.Config.ToString());
                        var intDir = Path.Combine(group.IntermediateDirectory, "Unused");
                        var resolver = new ModuleRulesResolver(solution, ModuleRules.New(project.RuleType, new TargetInfo { Platform = buildConfig.Platform }));
                        var pps = GenerateProjectPreprocessorDefs(resolver);
                        var includes = GenerateIncludePaths(resolver);

                        AppendFormatLine("""<PropertyGroup Condition="'$(Configuration)|$(Platform)'=='{0}|{1}'">""", configName, archName);
                        Indent(() =>
                        {
                            AppendFormatLine("""<AdditionalOptions>/std:c++20</AdditionalOptions>""", outDir);
                            AppendFormatLine("""<NMakePreprocessorDefinitions>{0};PLATFORM_WINDOWS=1</NMakePreprocessorDefinitions>""", pps);
                            AppendFormatLine("""<IncludePath>{0};$(IncludePath)</IncludePath>""", includes);
                        });
                        AppendFormatLine("""</PropertyGroup>""");
                    }

                    AppendFormatLine("""<ItemGroup>""");
                    Indent(() =>
                    {
                        foreach (var descriptor in project.GetSourceCodes())
                        {
                            var cl = GetCompilerType(descriptor);
                            AppendFormatLine("""<{0} Include="{1}" />""", cl, descriptor.FilePath);
                        }
                    });
                    AppendFormatLine("""</ItemGroup>""");

                    AppendFormatLine("""<Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />""");
                    AppendFormatLine("""<ImportGroup Label="ExtensionTargets">""");
                    AppendFormatLine("""</ImportGroup>""");
                });
                AppendFormatLine("""</Project>""");

                string? previousXml = null;
                if (File.Exists(fileName))
                {
                    previousXml = await File.ReadAllTextAsync(fileName, cancellationToken);
                }

                var currentXml = builder.ToString();
                if (previousXml != currentXml)
                {
                    await File.WriteAllTextAsync(fileName, currentXml, cancellationToken);
                }

                return;

                void Indent(Action inner)
                {
                    ++indent;
                    inner();
                    --indent;
                }

                void AppendFormatLine(string format, params ReadOnlySpan<object?> args)
                {
                    var indentStr = new string(' ', indent * 2);
                    builder.AppendFormat(indentStr + format + '\n', args);
                }

                string GenerateIncludePaths(ModuleRulesResolver resolver)
                {
                    List<string> includes = new();
                    foreach (var includePath in resolver.IncludePaths)
                    {
                        var fullPath = Path.Combine(project.SourceDirectory, includePath);
                        includes.Add(fullPath);
                    }

                    return string.Join(';', includes);
                }

                static string GetConfigName(BuildConfiguration value) => value.Config + (value.Editor ? "_Editor" : string.Empty);

                static bool LibraryDebugLevel(BuildConfiguration value) => value.Config switch
                {
                    Configuration.Debug => true,
                    Configuration.DebugGame => true,
                    Configuration.Development => false,
                    Configuration.Shipping => false,
                    _ => throw new InvalidOperationException()
                };

                static string GenerateProjectPreprocessorDefs(ModuleRulesResolver resolver)
                {
                    return string.Join(';', resolver.AdditionalMacros.Select(FormatDef));

                    static string FormatDef(MacroSet value)
                    {
                        if (value.Value == null)
                        {
                            return value.VarName;
                        }
                        else
                        {
                            return $"{value.VarName}={value.Value}";
                        }
                    }
                }
            }
        }

        static string GetCompilerType(SourceCodeDescriptor descriptor)
        {
            return descriptor.Type switch
            {
                SourceCodeType.SourceCode => "ClCompile",
                SourceCodeType.Header or SourceCodeType.Declaration => "ClInclude",
                _ => throw new NotSupportedException()
            };
        }

        static string GetConfigName(BuildConfiguration value)
        {
            return value.Config.ToString() + (value.Editor ? " Editor" : string.Empty);
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

        static string GetCppConfigName(BuildConfiguration value)
        {
            return value.Config.ToString() + (value.Editor ? "_Editor" : string.Empty);
        }

        static string GetCSharpConfigName(Configuration value)
        {
            return value == Configuration.Debug ? "Debug" : "Release";
        }
    }
}
