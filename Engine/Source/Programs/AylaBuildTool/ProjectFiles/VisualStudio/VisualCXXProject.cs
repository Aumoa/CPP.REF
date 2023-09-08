// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Xml;

using AE.BuildSettings;
using AE.Misc;
using AE.Platform;
using AE.Platform.Windows;
using AE.Projects;
using AE.Rules;
using AE.Source;

namespace AE.ProjectFiles.VisualStudio;

public class VisualCXXProject : IVisualStudioProject
{
    public required Workspace Workspace { get; init; }

    public required string TargetName { get; init; }

    public required string ProjectGuid { get; init; }

    public required string FilterPath { get; init; }

    public required string ProjectFile { get; init; }

    private ProjectDirectory ProjectDirectory { get; init; }

    private string SourceDirectory { get; init; }

    [SetsRequiredMembers]
    public VisualCXXProject(Workspace InWorkspace, ProjectDirectory ProjectDirectory, string ProjectName, string SourceDirectory, string FilterPath)
    {
        this.Workspace = InWorkspace;
        this.TargetName = ProjectName;
        this.ProjectGuid = CRC32.GenerateGuid(FilterPath + '/' + ProjectName).ToString().ToUpper();
        this.FilterPath = FilterPath;
        this.ProjectFile = Path.Combine(InWorkspace.CurrentTarget.Intermediate.ProjectFiles, Path.ChangeExtension(ProjectName, ".vcxproj"));
        this.ProjectDirectory = ProjectDirectory;
        this.SourceDirectory = SourceDirectory;
    }

    public (string, string) MapConfiguration(Configuration Configuration, bool bEditor, TargetPlatform Platform)
    {
        return (Configuration.ToString() + (bEditor ? "_Editor" : string.Empty), Platform.Architecture == Architecture.x64 ? "x64" : "Win32");
    }

    public async Task GenerateProjectFilesAsync(CancellationToken SToken = default)
    {
        var (Vcxproj, VcxprojFilter, VcxprojUser) = GenerateXmlDocument();
        await Task.WhenAll(new[]
        {
            WriteXml(Vcxproj, ProjectFile, ".vcxproj", SToken),
            WriteXml(VcxprojFilter, ProjectFile, ".vcxproj.filters", SToken),
            WriteXml(VcxprojUser, ProjectFile, ".vcxproj.user", SToken)
        });
    }

    private static async Task WriteXml(XmlDocument Doc, string SaveToBase, string SaveToExt, CancellationToken SToken = default)
    {
        StringWriter Writer = new();
        XmlTextWriter XmlWriter = new(Writer);
        XmlWriter.Formatting = Formatting.Indented;
        Doc.WriteTo(XmlWriter);
        await Global.CompareAndWriteAsync(Path.ChangeExtension(SaveToBase, SaveToExt), Writer.ToString(), SToken);
    }

    public (XmlDocument, XmlDocument, XmlDocument) GenerateXmlDocument()
    {
        HashSet<string> SourceFiles = new();

        XmlDocument Doc = new();
        Doc.AddXmlDeclaration("1.0", "utf-8");
        XmlElement Project = Doc.AddElement("Project", "http://schemas.microsoft.com/developer/msbuild/2003");
        {
            Project.SetAttribute("DefaultTargets", "Build");
            var ProjectConfigurations = Project.AddElement("ItemGroup");
            {
                ProjectConfigurations.SetAttribute("Label", "ProjectConfigurations");

                BuildConfiguration.ForEach((Configuration, bEditor, Platform) =>
                {
                    string App = bEditor ? "_Editor" : string.Empty;
                    var (Name, VSTarget) = MapConfiguration(Configuration, bEditor, Platform);

                    var ProjectConfiguration = ProjectConfigurations.AddElement("ProjectConfiguration");
                    ProjectConfiguration.SetAttribute("Include", $"{Name}|{VSTarget}");

                    ProjectConfiguration.AddElement("Configuration").InnerText = Configuration.ToString() + App;
                    ProjectConfiguration.AddElement("Platform").InnerText = VSTarget;
                });
            }

            var Globals = Project.AddElement("PropertyGroup");
            {
                Globals.SetAttribute("Label", "Globals");
                Globals.AddElement("VCProjectVersion").InnerText = "16.0";
                Globals.AddElement("Keyword").InnerText = "Win32Proj";
                Globals.AddElement("ProjectGuid").InnerText = $"{{{ProjectGuid}}}";
                Globals.AddElement("RootNamespace").InnerText = FilterPath.Replace(Path.DirectorySeparatorChar, '.');
                Globals.AddElement("WindowsTargetPlatformVersion").InnerText = "10.0";
            }

            var Import = Project.AddElement("Import");
            {
                Import.SetAttribute("Project", "$(VCTargetsPath)\\Microsoft.Cpp.Default.props");
            }

            BuildConfiguration.ForEach((Configuration, bEditor, Platform) =>
            {
                string App = bEditor ? "_Editor" : string.Empty;
                var PropertyGroup = Project.AddElement("PropertyGroup");
                var (Name, VSTarget) = MapConfiguration(Configuration, bEditor, Platform);
                PropertyGroup.SetAttribute("Condition", $"'$(Configuration)|$(Platform)'=='{Name}|{VSTarget}'");
                PropertyGroup.SetAttribute("Label", "Configuration");

                PropertyGroup.AddElement("ConfigurationType").InnerText = "Makefile";
                PropertyGroup.AddElement("PlatformToolset").InnerText = "v143";
                PropertyGroup.AddElement("CharacterSet").InnerText = "Unicode";
            });

            Import = Project.AddElement("Import");
            Import.SetAttribute("Project", "$(VCTargetsPath)\\Microsoft.Cpp.props");

            var ToolChain = VisualStudioInstallation.FindVisualStudioInstallations(CompilerVersion.VisualStudio2022).First();

            BuildConfiguration.ForEach((Configuration, bEditor, Platform) =>
            {
                string App = bEditor ? "_Editor" : "";
                string TargetApp = bEditor ? "Editor" : "";

                // Foreach Configurations
                var PropertyGroup = Project.AddElement("PropertyGroup");
                var (Name, VSTarget) = MapConfiguration(Configuration, bEditor, Platform);
                PropertyGroup.SetAttribute("Condition", $"'$(Configuration)|$(Platform)'=='{Name}|{VSTarget}'");

                string BuildToolPath = Global.BuildToolPath;
                BuildToolPath = Path.ChangeExtension(BuildToolPath, ".exe");

                var IncludePaths = ToolChain.GetRequiredIncludePaths(Platform.Architecture);

                PropertyGroup.AddElement("NMakeBuildCommandLine").InnerText = $"{BuildToolPath} Build -Target {TargetName}{TargetApp} -Config {Configuration}";
                PropertyGroup.AddElement("NMakeReBuildCommandLine").InnerText = $"{BuildToolPath} Build -Clean -Target {TargetName}{TargetApp} -Config {Configuration}";
                PropertyGroup.AddElement("NMakeCleanCommandLine").InnerText = $"{BuildToolPath} Clean";
                PropertyGroup.AddElement("OutDir").InnerText = Path.Combine(ProjectDirectory.Binaries.Win64, Configuration.ToString());
                PropertyGroup.AddElement("IntDir").InnerText = ProjectDirectory.Intermediate.Unused;

                List<string> Macros = new();
                PlatformGroup.ForEach(p =>
                {
                    if (p == PlatformGroup.Windows)
                    {
                        Macros.Add($"{p.ToDefinition()}=1");
                    }
                    else
                    {
                        Macros.Add($"{p.ToDefinition()}=0");
                    }
                });

                if (bEditor)
                {
                    Macros.Add("WITH_EDITOR=1");
                }
                else
                {
                    Macros.Add("WITH_EDITOR=0");
                }

                Macros.AddRange(new[]
                {
                    "UNICODE",
                    "_UNICODE"
                });

                PropertyGroup.AddElement("NMakeIncludeSearchPath").InnerText = string.Join(';', IncludePaths);
                PropertyGroup.AddElement("AdditionalOptions").InnerText = "/std:c++20 /Zc:preprocessor";
                PropertyGroup.AddElement("NMakePreprocessorDefinitions").InnerText = string.Join(';', Macros);
                PropertyGroup.AddElement("UseStandardPreprocessor").InnerText = "true";
                if (Platform.Architecture == Architecture.x64)
                {
                    PropertyGroup.AddElement("ExecutablePath").InnerText = "$(VC_ExecutablePath_x64);$(CommonExecutablePath)";
                }
                
            });

            var ItemGroup = Project.AddElement("ItemGroup");

            XmlElement? AddSourceFile(ModuleInformation? Module, string Filename)
            {
                if (SourceFiles.Add(Filename) == false)
                {
                    return null;
                }

                string Extension = Path.GetExtension(Filename).ToLower();
                if (Global.IsSourceCode(Extension))
                {
                    var ClCompile = ItemGroup.AddElement("ClCompile");
                    ClCompile.SetAttribute("Include", Filename);

                    if (Module != null)
                    {
                        IEnumerable<string> Macros = Module.AdditionalMacros;
                        Macros = Macros.Concat(Module.DependModules.Select(p => $"{p.ToUpper()}_API=__declspec(dllimport)"));
                        Macros = Macros.Append($"{Module.Name.ToUpper()}_API=__declspec(dllexport)");

                        var PreprocessorDefinitions = ClCompile.AddElement("PreprocessorDefinitions");
                        PreprocessorDefinitions.InnerText = $"{string.Join(';', Macros)};%(PreprocessorDefinitions)";

                        string GeneratedInclude = Path.Combine(Module.GeneratedIncludePath, Module.Name);

                        var AdditionalIncludeDirectories = ClCompile.AddElement("AdditionalIncludeDirectories");
                        AdditionalIncludeDirectories.InnerText = $"{string.Join(';', Module.IncludePaths.Append(GeneratedInclude))};%(AdditionalIncludeDirectories)";

                        if (Module.DependModules.Contains("Core"))
                        {
                            var ForcedIncludeFiles = ClCompile.AddElement("ForcedIncludeFiles");
                            ForcedIncludeFiles.InnerText = "CoreMinimal.h";
                        }
                    }

                    return ClCompile;
                }
                else if (Global.IsHeaderFile(Extension))
                {
                    var ClInclude = ItemGroup.AddElement("ClInclude");
                    ClInclude.SetAttribute("Include", Filename);
                    return null;
                }
                else if (Global.IsRuleFile(Extension))
                {
                    var None = ItemGroup.AddElement("None");
                    None.SetAttribute("Include", Filename);
                    return null;
                }
                else if (Global.IsNatvisFile(Extension))
                {
                    var None = ItemGroup.AddElement("Natvis");
                    None.SetAttribute("Include", Filename);
                    return null;
                }

                return null;
            }

            void SearchDirectory(ModuleInformation? Module, string CurrentDirectory)
            {
                if (SourceDirectory.StartsWith(ProjectDirectory.Source.Programs) == false && CurrentDirectory.StartsWith(ProjectDirectory.Source.Programs))
                {
                    return;
                }

                string[] ModuleFile = Directory.GetFiles(CurrentDirectory, "*.Module.cs", SearchOption.TopDirectoryOnly);
                if (ModuleFile.Any())
                {
                    string ModuleName = Path.GetFileName(ModuleFile[0]).Replace(".Module.cs", "");
                    Dictionary<string, SearchedModule> SearchedModules = new();
                    var TargetRule = new TargetRules(new TargetInfo
                    {
                        BuildConfiguration = new()
                        {
                            Configuration = Configuration.Development,
                            Platform = TargetPlatform.Win64
                        }
                    }, ModuleName)
                    {
                        bEditor = true
                    };
                    Global.SearchCXXModulesRecursive(Workspace, TargetRule, SearchedModules, TargetRule.Name, TargetRule.TargetModuleName);

                    var Resolver = new ModuleDependenciesResolver(TargetRule, SearchedModules, ToolChain);
                    Resolver.Resolve();
                    Module = Resolver.GetDependencyCache(ModuleName);
                }

                foreach (var SourceFile in Directory.GetFiles(CurrentDirectory, "*", SearchOption.TopDirectoryOnly))
                {
                    AddSourceFile(Module, SourceFile);
                }

                foreach (var Subdirectory in Directory.GetDirectories(CurrentDirectory, "*", SearchOption.TopDirectoryOnly))
                {
                    SearchDirectory(Module, Subdirectory);
                }
            }

            SearchDirectory(null, SourceDirectory);

            Import = Project.AddElement("Import");
            Import.SetAttribute("Project", "$(VCTargetsPath)\\Microsoft.Cpp.targets");

            var ImportGroup = Project.AddElement("ImportGroup");
            ImportGroup.SetAttribute("Label", "ExtensionTargets");
        }
        var Vcxproj = Doc;

        Doc = new();
        Doc.AddXmlDeclaration("1.0", "utf-8");
        Project = Doc.AddElement("Project", "http://schemas.microsoft.com/developer/msbuild/2003");
        {
            Project.SetAttribute("ToolVersion", "4.0");
            var ItemGroup = Project.AddElement("ItemGroup");

            HashSet<string> Filters = new();
            foreach (var Filename in SourceFiles)
            {
                string? FilterPath = Path.GetRelativePath(SourceDirectory, Filename);
                FilterPath = Path.GetDirectoryName(FilterPath)!;
                if (SourceDirectory.StartsWith(ProjectDirectory.Source.Programs) == false)
                {
                    FilterPath = Path.Combine("Source", FilterPath ?? "/");
                }

                string[] Splices = FilterPath.Split(Path.DirectorySeparatorChar);
                string Composed = string.Empty;
                foreach (var Splice in Splices)
                {
                    Composed = Path.Combine(Composed, Splice);
                    Filters.Add(Composed);
                }

                string Extension = Path.GetExtension(Filename);
                XmlElement? InnerElement = null;
                if (Global.IsSourceCode(Extension))
                {
                    var ClCompile = ItemGroup.AddElement("ClCompile");
                    ClCompile.SetAttribute("Include", Filename);
                    InnerElement = ClCompile;
                }
                else if (Global.IsHeaderFile(Extension))
                {
                    var ClInclude = ItemGroup.AddElement("ClInclude");
                    ClInclude.SetAttribute("Include", Filename);
                    InnerElement = ClInclude;
                }
                else if (Global.IsRuleFile(Extension))
                {
                    var None = ItemGroup.AddElement("None");
                    None.SetAttribute("Include", Filename);
                    InnerElement = None;
                }
                else if (Global.IsNatvisFile(Extension))
                {
                    var None = ItemGroup.AddElement("Natvis");
                    None.SetAttribute("Include", Filename);
                    InnerElement = None;
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
        var VcxprojFilters = Doc;

        Doc = new();
        Doc.AddXmlDeclaration("1.0", "utf-8");
        Project = Doc.AddElement("Project", "http://schemas.microsoft.com/developer/msbuild/2003");
        Project.SetAttribute("ToolsVersion", "Current");
        {
            foreach (var Platform in BuildConfiguration.Platforms)
            {
                var PropertyGroup = Project.AddElement("PropertyGroup");
                PropertyGroup.SetAttribute("Condition", $"'$(Platform)'=='{Platform}'");

                bool bIsProgram = SourceDirectory != ProjectDirectory.Source.Root;
                string Executable;
                string? LaunchDLL = null;
                if (bIsProgram)
                {
                    Executable = Path.GetFileNameWithoutExtension(SourceDirectory);
                }
                else
                {
                    Executable = "Launch";
                    if (ProjectDirectory.Root != Global.EngineDirectory.Root)
                    {
                        LaunchDLL = Path.ChangeExtension(ProjectDirectory.Name, ".dll");
                    }
                }

                Executable = Path.ChangeExtension(Executable, ".exe");
                PropertyGroup.AddElement("LocalDebuggerCommand").InnerText = Executable;
                PropertyGroup.AddElement("LocalDebuggerCommandArguments").InnerText = LaunchDLL ?? "";

                string WorkingDirectory = Path.GetDirectoryName(Executable)!;
                PropertyGroup.AddElement("LocalDebuggerWorkingDirectory").InnerText = "$(OutDir)";

                PropertyGroup.AddElement("DebuggerFlavor").InnerText = "WindowsLocalDebugger";
                PropertyGroup.AddElement("LocalDebuggerDebuggerType").InnerText = "Auto";
            }
        }
        var VcxprojUser = Doc;

        return (Vcxproj, VcxprojFilters, VcxprojUser);
    }
}
