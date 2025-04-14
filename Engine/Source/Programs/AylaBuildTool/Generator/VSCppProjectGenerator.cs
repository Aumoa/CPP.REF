// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Text;

namespace AylaEngine;

internal static class VSCppProjectGenerator
{
    private static readonly Version VCProjectVersion = new Version(17, 0);
    private static readonly Version WindowsTargetPlatformVersion = new Version(10, 0);
    private const string PlatformToolset = "v143";

    public static async Task GenerateAsync(Solution solution, Dictionary<ModuleProject, string> vcxprojPaths, ModuleProject project, CancellationToken cancellationToken)
    {
        var engineGroup = solution.EngineGroup;
        var primaryGroup = solution.PrimaryGroup;

        var group = project.Group;
        var projectFilesDirectory = Path.Combine(primaryGroup.IntermediateDirectory, "ProjectFiles");
        await Task.WhenAll(
            GenerateVcxprojAsync(),
            GenerateVcxprojFiltersAsync(),
            GenerateVcxprojUserAsync()
        );

        return;

        async Task GenerateVcxprojUserAsync()
        {
            var fileName = Path.Combine(projectFilesDirectory, project.Name + ".vcxproj.user");
            var builder = new StringBuilder();
            int indent = 0;

            AppendFormatLine("""<?xml version="1.0" encoding="utf-8"?>""");
            AppendFormatLine("""<Project ToolVersion="Current" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">""");
            Indent(() =>
            {
                foreach (var buildConfig in TargetInfo.GetAllTargets())
                {
                    // Visual Studio only support Windows platform.
                    if (buildConfig.Platform.Group != PlatformGroup.Windows)
                    {
                        continue;
                    }

                    AppendFormatLine("""<PropertyGroup Condition="'$(Configuration)|$(Platform)'=='{0}|{1}'">""", VSUtility.GetConfigName(buildConfig), VSUtility.GetArchitectureName(buildConfig));
                    Indent(() =>
                    {
                        var rules = ModuleRules.New(project.RuleType, new TargetInfo { Platform = buildConfig.Platform });
                        if (rules.Type == ModuleType.Game)
                        {
                            AppendFormatLine("""<LocalDebuggerCommand>{0}\DotNET\ScriptingLaunch.exe</LocalDebuggerCommand>""", engineGroup.BinariesDirectory);
                        }
                        AppendFormatLine("""<DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>""");
                        AppendFormatLine("""<LocalDebuggerWorkingDirectory>{0}</LocalDebuggerWorkingDirectory>""", engineGroup.Output(buildConfig, FolderPolicy.PathType.Current));
                        AppendFormatLine("""<LocalDebuggerDebuggerType>NativeWithManagedCore</LocalDebuggerDebuggerType>""");
                    });
                    AppendFormatLine("""</PropertyGroup>""");
                }
            });

            AppendFormatLine("""</Project>""");

            var currentXml = builder.ToString();
            await TextFileHelper.WriteIfChangedAsync(fileName, currentXml, cancellationToken);

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
        }

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

            var currentXml = builder.ToString();
            await TextFileHelper.WriteIfChangedAsync(fileName, currentXml, cancellationToken);

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
                    foreach (var buildConfig in TargetInfo.GetAllTargets())
                    {
                        // Visual Studio only support Windows platform.
                        if (buildConfig.Platform.Group != PlatformGroup.Windows)
                        {
                            continue;
                        }

                        var configName = VSUtility.GetConfigName(buildConfig);
                        var archName = VSUtility.GetArchitectureName(buildConfig);

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

                foreach (var buildConfig in TargetInfo.GetAllTargets())
                {
                    // Visual Studio only support Windows platform.
                    if (buildConfig.Platform.Group != PlatformGroup.Windows)
                    {
                        continue;
                    }

                    var configName = VSUtility.GetConfigName(buildConfig);
                    var archName = VSUtility.GetArchitectureName(buildConfig);

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

                string projectPath = string.Empty;
                if (project.Group.IsEngine == false)
                {
                    projectPath = $"--project \"{primaryGroup.RootDirectory}\\{primaryGroup.Name}.aproject\" ";
                }

                foreach (var buildTarget in TargetInfo.GetAllTargets())
                {
                    // Visual Studio only support Windows platform.
                    if (buildTarget.Platform.Group != PlatformGroup.Windows)
                    {
                        continue;
                    }

                    var configName = VSUtility.GetConfigName(buildTarget);
                    var archName = VSUtility.GetArchitectureName(buildTarget);
                    var outDir = group.Output(buildTarget, FolderPolicy.PathType.Windows);
                    var rules = project.GetRule(buildTarget);
                    var resolver = project.GetResolver(buildTarget);
                    var pps = GenerateProjectPreprocessorDefs(resolver, buildTarget);
                    var includes = GenerateIncludePaths(resolver);
                    var outputFileName = FolderPolicy.OutputFileName(project.Name, rules.Type);

                    AppendFormatLine("""<PropertyGroup Condition="'$(Configuration)|$(Platform)'=='{0}|{1}'">""", configName, archName);
                    Indent(() =>
                    {
                        var buildCommand = $"\"{engineGroup.BinariesDirectory}\\DotNET\\AylaBuildTool.dll\" build {projectPath}--target \"{project.Name}\" --config {buildTarget.Config} {(buildTarget.Editor ? "--editor " : string.Empty)}";
                        AppendFormatLine("""<AdditionalOptions>/std:c++20 /Zc:preprocessor</AdditionalOptions>""", outDir);
                        AppendFormatLine("""<NMakePreprocessorDefinitions>{0};PLATFORM_WINDOWS=1</NMakePreprocessorDefinitions>""", pps);
                        AppendFormatLine("""<NMakeBuildCommandLine>dotnet {0}</NMakeBuildCommandLine>""", buildCommand);
                        AppendFormatLine("""<NMakeReBuildCommandLine>dotnet {0} --clean Rebuild</NMakeReBuildCommandLine>""", buildCommand);
                        AppendFormatLine("""<NMakeCleanCommandLine>dotnet {0} --clean CleanOnly</NMakeCleanCommandLine>""", buildCommand);
                        AppendFormatLine("""<OutDir>{0}</OutDir>""", outDir);
                        AppendFormatLine("""<NMakeOutput>{0}</NMakeOutput>""", outputFileName);
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

                foreach (var buildTarget in TargetInfo.GetAllTargets())
                {
                    // Visual Studio only support Windows platform.
                    if (buildTarget.Platform.Group != PlatformGroup.Windows)
                    {
                        continue;
                    }

                    var configName = VSUtility.GetConfigName(buildTarget);
                    var archName = VSUtility.GetArchitectureName(buildTarget);
                    var resolver = project.GetResolver(buildTarget);
                    var depends = solution.FindDepends(resolver.DependencyModuleNames);
                    foreach (var depend in depends.OfType<ModuleProject>())
                    {
                        var rules = depend.GetRule(buildTarget);
                        if (rules.EnableScript)
                        {
                            AppendFormatLine("""<ItemGroup Condition="'$(Configuration)|$(Platform)'=='{0}|{1}'">""", configName, archName);
                            Indent(() =>
                            {
                                AppendFormatLine("""<ProjectReference Include="{0}">""", depend.GetScriptProjectName());
                                Indent(() =>
                                {
                                    AppendFormatLine("""<Project>{0}</Project>""", depend.Decl.ScriptGuid.ToString("B").ToUpper());
                                });
                                AppendFormatLine("""</ProjectReference>""");
                            });
                            AppendFormatLine("""</ItemGroup>""");
                        }
                    }
                }

                AppendFormatLine("""<Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />""");
                AppendFormatLine("""<ImportGroup Label="ExtensionTargets">""");
                AppendFormatLine("""</ImportGroup>""");
            });
            AppendFormatLine("""</Project>""");

            var currentXml = builder.ToString();
            await TextFileHelper.WriteIfChangedAsync(fileName, currentXml, cancellationToken);

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

            static bool LibraryDebugLevel(TargetInfo value) => value.Config switch
            {
                Configuration.Debug => true,
                Configuration.DebugGame => true,
                Configuration.Development => false,
                Configuration.Shipping => false,
                _ => throw new InvalidOperationException()
            };

            static string GenerateProjectPreprocessorDefs(ModuleRulesResolver resolver, TargetInfo targetInfo)
            {
                var additionalMacros = resolver.AdditionalMacros
                    .Concat(["UNICODE", "_UNICODE"]);
                if (targetInfo.Config != Configuration.Shipping)
                {
                    additionalMacros = additionalMacros.Append("DO_CHECK=1");
                }

                return string.Join(';', additionalMacros.Select(FormatDef));

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


        static string GetCompilerType(SourceCodeDescriptor descriptor)
        {
            return descriptor.Type switch
            {
                SourceCodeType.SourceCode => "ClCompile",
                SourceCodeType.Header or SourceCodeType.Declaration => "ClInclude",
                SourceCodeType.NativeVisualizer => "Natvis",
                _ => throw new NotSupportedException()
            };
        }
    }
}
