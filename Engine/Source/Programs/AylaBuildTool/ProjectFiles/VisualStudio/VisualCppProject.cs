﻿// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Xml;

using AE.Assemblies;
using AE.BuildSettings;
using AE.CompilerServices;
using AE.Extensions;
using AE.IO;
using AE.Misc;
using AE.Platform;
using AE.Projects;
using AE.Source;

namespace AE.ProjectFiles.VisualStudio;

public class VisualCppProject : VisualStudioProject
{
    public override string Name { get; }

    public override string GUID { get; }

    public override string Filter { get; }

    public override FileReference Project { get; }

    public override ProjectDirectory TargetDirectory { get; }

    public override DirectoryReference SourceDirectory { get; }

    private CppAssembly assembly;
    private ModuleInformation moduleInfo;

    public VisualCppProject(CppAssembly assembly)
    {
        this.Name = assembly.Name;
        this.Filter = assembly.RelativeDirectory.Replace(Path.DirectorySeparatorChar, '/');
        this.GUID = VSUtility.MakeGUID(ProjectPath);
        this.Project = Workspace.Current.Intermediate.ProjectFiles.GetFile($"{Name}.vcxproj");
        this.TargetDirectory = assembly.ProjectDirectory;
        this.SourceDirectory = assembly.SourceDirectory;
        this.assembly = assembly;
        this.moduleInfo = ModuleDependencyCache.GetCached(assembly.Name, string.Empty);
    }

    public override (string, string) MapConfiguration(Configuration configuration, bool isEditor, TargetPlatform platform)
    {
        return (configuration.ToString() + (isEditor ? "_Editor" : string.Empty), platform.ToString());
    }

    public override async Task GenerateProjectFilesAsync(CancellationToken cancellationToken = default)
    {
        GenerateXmlDocument(out var vcxproj, out var vcxprojFilter, out var vcxprojUser);
        await Task.WhenAll(new[]
        {
            WriteXml(vcxproj, Project, ".vcxproj", cancellationToken),
            WriteXml(vcxprojFilter, Project, ".vcxproj.filters", cancellationToken),
            WriteXml(vcxprojUser, Project, ".vcxproj.user", cancellationToken)
        });
    }

    private static async Task WriteXml(XmlDocument Doc, string SaveToBase, string SaveToExt, CancellationToken SToken = default)
    {
        StringWriter Writer = new();
        XmlTextWriter XmlWriter = new(Writer);
        XmlWriter.Formatting = Formatting.Indented;
        Doc.WriteTo(XmlWriter);
        await IOUtility.CompareAndWriteAsync(Path.ChangeExtension(SaveToBase, SaveToExt), Writer.ToString(), SToken);
    }

    private void GenerateXmlDocument(out XmlDocument vcxproj, out XmlDocument vcxprojFilter, out XmlDocument vcxprojUser)
    {
        HashSet<FileReference> sourceFiles = new();

        XmlDocument doc = new();
        doc.AddXmlDeclaration("1.0", "utf-8");
        XmlElement project = doc.AddElement("Project", "http://schemas.microsoft.com/developer/msbuild/2003");
        {
            project.SetAttribute("DefaultTargets", "Build");
            var projectConfigurations = project.AddElement("ItemGroup");
            {
                projectConfigurations.SetAttribute("Label", "ProjectConfigurations");

                BuildConfiguration.ForEach((configuration, isEditor, platform) =>
                {
                    string app = isEditor ? "_Editor" : string.Empty;
                    var (Name, VSTarget) = MapConfiguration(configuration, isEditor, platform);

                    var ProjectConfiguration = projectConfigurations.AddElement("ProjectConfiguration");
                    ProjectConfiguration.SetAttribute("Include", $"{Name}|{VSTarget}");

                    ProjectConfiguration.AddElement("Configuration").InnerText = configuration.ToString() + app;
                    ProjectConfiguration.AddElement("Platform").InnerText = VSTarget;
                });
            }

            var globals = project.AddElement("PropertyGroup");
            {
                globals.SetAttribute("Label", "Globals");
                globals.AddElement("VCProjectVersion").InnerText = "16.0";
                globals.AddElement("Keyword").InnerText = "Win32Proj";
                globals.AddElement("ProjectGuid").InnerText = $"{{{GUID}}}";
                globals.AddElement("RootNamespace").InnerText = Filter.Replace(Path.DirectorySeparatorChar, '.');
                globals.AddElement("WindowsTargetPlatformVersion").InnerText = "10.0";
            }

            var import = project.AddElement("Import");
            {
                import.SetAttribute("Project", "$(VCTargetsPath)\\Microsoft.Cpp.Default.props");
            }

            BuildConfiguration.ForEach((configuration, isEditor, platform) =>
            {
                string app = isEditor ? "_Editor" : string.Empty;
                var propertyGroup = project.AddElement("PropertyGroup");
                var (name, vsTarget) = MapConfiguration(configuration, isEditor, platform);
                propertyGroup.SetAttribute("Condition", $"'$(Configuration)|$(Platform)'=='{name}|{vsTarget}'");
                propertyGroup.SetAttribute("Label", "Configuration");

                propertyGroup.AddElement("ConfigurationType").InnerText = "Makefile";
                propertyGroup.AddElement("PlatformToolset").InnerText = "v143";
                propertyGroup.AddElement("CharacterSet").InnerText = "Unicode";
            });

            import = project.AddElement("Import");
            import.SetAttribute("Project", "$(VCTargetsPath)\\Microsoft.Cpp.props");

            BuildConfiguration.ForEach((configuration, isEditor, platform) =>
            {
                string editorArgs = isEditor ? "-Editor" : string.Empty;

                // Foreach Configurations
                var propertyGroup = project.AddElement("PropertyGroup");
                var (name, vsTarget) = MapConfiguration(configuration, isEditor, platform);
                propertyGroup.SetAttribute("Condition", $"'$(Configuration)|$(Platform)'=='{name}|{vsTarget}'");

                string buildToolPath = Global.BuildToolPath;
                buildToolPath = Path.ChangeExtension(buildToolPath, ".exe");

                IEnumerable<string> includePaths = ToolChain.GetRequiredIncludePaths(platform.Architecture)
                    .Concat(moduleInfo.PrivateIncludePaths)
                    .Append(Path.Combine(moduleInfo.GeneratedIncludePath, moduleInfo.Name))
                    .Append(moduleInfo.GeneratedShaderPath);
                var extensions = assembly.Rules.Type == Rules.ModuleRules.ModuleType.Library ? ".dll" : ".exe";

                var projectFile = string.Empty;
                if (Workspace.IsTargetEngine == false)
                {
                    projectFile = $"-ProjectFile \"{Workspace.ProjectFile}\"";
                }

                propertyGroup.AddElement("NMakeBuildCommandLine").InnerText = $"{buildToolPath} Build {projectFile} -Target {this.Name} -Config {configuration} {editorArgs}";
                propertyGroup.AddElement("NMakeReBuildCommandLine").InnerText = $"{buildToolPath} Build {projectFile} -Clean -Target {this.Name} -Config {configuration} {editorArgs}";
                propertyGroup.AddElement("NMakeCleanCommandLine").InnerText = $"{buildToolPath} Clean";
                propertyGroup.AddElement("NMakeOutput").InnerText = TargetDirectory.Binaries.BinariesOut(platform, configuration).GetFile(assembly.Name + extensions);
                propertyGroup.AddElement("OutDir").InnerText = TargetDirectory.Binaries.BinariesOut(platform, configuration);
                propertyGroup.AddElement("IntDir").InnerText = TargetDirectory.Intermediate.Unused;

                List<string> macros = new();
                PlatformGroup.ForEach(p =>
                {
                    if (p == PlatformGroup.Windows)
                    {
                        macros.Add($"{p.ToDefinition()}=1");
                    }
                    else
                    {
                        macros.Add($"{p.ToDefinition()}=0");
                    }
                });

                bool isShipping = configuration == Configuration.Shipping;
                bool doCheck = !isShipping;
                macros.AddRange(moduleInfo.PrivateAdditionalMacros
                    .Concat(new[] { $"WITH_EDITOR={BoolToStr(isEditor)}", $"SHIPPING={BoolToStr(isShipping)}", $"DO_CHECK={BoolToStr(doCheck)}", "UNICODE", "_UNICODE" })
                    .Concat(moduleInfo.DependModules.Select(p => $"{p.ToUpper()}_API=__declspec(dllimport)"))
                    .Append($"{moduleInfo.Name.ToUpper()}_API=__declspec(dllexport)")
                    .Append($"PLATFORM_STRING=TEXT(\"{platform.ToString()}\")")
                    .Append($"CONFIG_STRING=TEXT(\"{configuration}\")")
                    );

                propertyGroup.AddElement("NMakeIncludeSearchPath").InnerText = string.Join(';', includePaths);
                propertyGroup.AddElement("AdditionalOptions").InnerText = "/std:c++20 /Zc:preprocessor";
                propertyGroup.AddElement("NMakePreprocessorDefinitions").InnerText = string.Join(';', macros);
                propertyGroup.AddElement("UseStandardPreprocessor").InnerText = "true";
                if (platform.Architecture == Architecture.x64)
                {
                    propertyGroup.AddElement("ExecutablePath").InnerText = "$(VC_ExecutablePath_x64);$(CommonExecutablePath)";
                }

                if (moduleInfo.DependModules.Contains("Core"))
                {
                    var ForcedIncludeFiles = propertyGroup.AddElement("ForcedIncludeFiles");
                    ForcedIncludeFiles.InnerText = "CoreMinimal.h";
                }
            });

            var ItemGroup = project.AddElement("ItemGroup");

            XmlElement? AddSourceFile(ModuleInformation? module, FileReference file)
            {
                if (sourceFiles.Add(file) == false)
                {
                    return null;
                }

                if (file.IsSourceCode())
                {
                    var ClCompile = ItemGroup.AddElement("ClCompile");
                    ClCompile.SetAttribute("Include", file);
                    return ClCompile;
                }
                else if (file.IsHeaderFile())
                {
                    var ClInclude = ItemGroup.AddElement("ClInclude");
                    ClInclude.SetAttribute("Include", file);
                    return null;
                }
                else if (file.IsRuleFile())
                {
                    var None = ItemGroup.AddElement("None");
                    None.SetAttribute("Include", file);
                    return null;
                }
                else if (file.IsNatvisFile())
                {
                    var None = ItemGroup.AddElement("Natvis");
                    None.SetAttribute("Include", file);
                    return null;
                }
                else if (file.IsShaderCode())
                {
                    var Shader = ItemGroup.AddElement("FxCompile");
                    Shader.SetAttribute("Include", file);
                    return null;
                }

                return null;
            }

            void SearchDirectory(ModuleInformation? module, DirectoryReference currentDirectory)
            {
                if (SourceDirectory.Includes(TargetDirectory.Source.Programs) == false &&
                    currentDirectory.Includes(TargetDirectory.Source.Programs))
                {
                    return;
                }

                foreach (var FileName in currentDirectory.GetFiles())
                {
                    if (FileName.Extensions.Equals(".csproj", StringComparison.OrdinalIgnoreCase))
                    {
                        return;
                    }
                }

                FileReference[] ModuleFile = currentDirectory.GetFiles("*.Module.cs");
                if (ModuleFile.Any())
                {
                    string ModuleName = Path.GetFileName(ModuleFile[0]).Replace(".Module.cs", "");
                    module = ModuleDependencyCache.GetCached(ModuleName, string.Empty);
                }

                foreach (var SourceFile in currentDirectory.GetFiles())
                {
                    AddSourceFile(module, SourceFile);
                }

                foreach (var Subdirectory in currentDirectory.GetDirectories())
                {
                    SearchDirectory(module, Subdirectory);
                }
            }

            SearchDirectory(null, SourceDirectory);
            if (SourceDirectory == TargetDirectory.Source.Root)
            {
                SearchDirectory(null, TargetDirectory.Shaders);
            }

            import = project.AddElement("Import");
            import.SetAttribute("Project", "$(VCTargetsPath)\\Microsoft.Cpp.targets");

            var ImportGroup = project.AddElement("ImportGroup");
            ImportGroup.SetAttribute("Label", "ExtensionTargets");
        }
        vcxproj = doc;

        doc = new();
        doc.AddXmlDeclaration("1.0", "utf-8");
        project = doc.AddElement("Project", "http://schemas.microsoft.com/developer/msbuild/2003");
        {
            project.SetAttribute("ToolVersion", "4.0");
            var ItemGroup = project.AddElement("ItemGroup");

            HashSet<string> Filters = new();
            foreach (var file in sourceFiles)
            {
                string? FilterPath;

                if (SourceDirectory.Includes(TargetDirectory.Source.Programs) == false)
                {
                    FilterPath = Path.GetRelativePath(SourceDirectory, file);
                    FilterPath = Path.GetDirectoryName(FilterPath)!;
                    FilterPath = Path.Combine("Source", FilterPath ?? "/");
                }
                else
                {
                    FilterPath = Path.GetRelativePath(SourceDirectory, file);
                    FilterPath = Path.GetDirectoryName(FilterPath)!;
                    FilterPath = Path.Combine("Source", FilterPath ?? "/");
                }

                string[] Splices = FilterPath.Split(Path.DirectorySeparatorChar);
                string Composed = string.Empty;
                foreach (var Splice in Splices)
                {
                    Composed = Path.Combine(Composed, Splice);
                    Filters.Add(Composed);
                }

                XmlElement? InnerElement = null;
                if (file.IsSourceCode())
                {
                    var ClCompile = ItemGroup.AddElement("ClCompile");
                    ClCompile.SetAttribute("Include", file);
                    InnerElement = ClCompile;
                }
                else if (file.IsHeaderFile())
                {
                    var ClInclude = ItemGroup.AddElement("ClInclude");
                    ClInclude.SetAttribute("Include", file);
                    InnerElement = ClInclude;
                }
                else if (file.IsRuleFile())
                {
                    var None = ItemGroup.AddElement("None");
                    None.SetAttribute("Include", file);
                    InnerElement = None;
                }
                else if (file.IsNatvisFile())
                {
                    var None = ItemGroup.AddElement("Natvis");
                    None.SetAttribute("Include", file);
                    InnerElement = None;
                }
                else if (file.IsShaderCode())
                {
                    var Shader = ItemGroup.AddElement("FxCompile");
                    Shader.SetAttribute("Include", file);
                    InnerElement = Shader;
                }

                if (InnerElement != null && string.IsNullOrEmpty(FilterPath) == false)
                {
                    InnerElement.AddElement("Filter").InnerText = FilterPath;
                }
            }

            foreach (var FilterPath in Filters)
            {
                if (string.IsNullOrEmpty(FilterPath) == false)
                {
                    var Filter = ItemGroup.AddElement("Filter");
                    Filter.SetAttribute("Include", FilterPath);
                    Filter.AddElement("UniqueIdentifier").InnerText = $"{{{CRC32.GenerateGuid(FilterPath).ToString().ToUpper()}}}";
                }
            }
        }
        vcxprojFilter = doc;

        doc = new();
        doc.AddXmlDeclaration("1.0", "utf-8");
        project = doc.AddElement("Project", "http://schemas.microsoft.com/developer/msbuild/2003");
        project.SetAttribute("ToolsVersion", "Current");
        {
            foreach (var Platform in BuildConfiguration.Platforms)
            {
                foreach (var Config in BuildConfiguration.Configurations)
                {
                    foreach (var bEditor in new[] { false, true })
                    {
                        var (ConfigStr, PlatformStr) = MapConfiguration(Config, bEditor, Platform);

                        var PropertyGroup = project.AddElement("PropertyGroup");
                        PropertyGroup.SetAttribute("Condition", $"'$(Configuration)|$(Platform)'=='{ConfigStr}|{PlatformStr}'");

                        string executable;
                        string commandArgs;

                        switch (assembly.Rules.Type)
                        {
                            case Rules.ModuleRules.ModuleType.ConsoleApplication:
                            case Rules.ModuleRules.ModuleType.Application:
                                string executableName = Path.GetFileNameWithoutExtension(SourceDirectory);
                                executable = TargetDirectory.Binaries.BinariesOut(TargetPlatform.Win64, Config).GetFile(executableName).WithExtensions("exe");
                                commandArgs = string.Empty;
                                break;
                            case Rules.ModuleRules.ModuleType.Library:
                                executable = Global.EngineDirectory.Binaries.Root.GetHierarchy("Win64", Config.IsDebug() ? "Debug" : "Development").GetFile("Launch").WithExtensions(".exe");
                                commandArgs = $"-GameAssembly {assembly.Name}";
                                break;
                            default:
                                continue;
                        }

                        PropertyGroup.AddElement("LocalDebuggerCommand").InnerText = executable;
                        PropertyGroup.AddElement("LocalDebuggerCommandArguments").InnerText = commandArgs;
                        PropertyGroup.AddElement("LocalDebuggerWorkingDirectory").InnerText = TargetDirectory.Root;

                        PropertyGroup.AddElement("DebuggerFlavor").InnerText = "WindowsLocalDebugger";
                        PropertyGroup.AddElement("LocalDebuggerDebuggerType").InnerText = "Auto";
                    }
                }
            }
        }

        vcxprojUser = doc;
    }

    private static string BoolToStr(bool b) => b ? "1" : "0";
}
