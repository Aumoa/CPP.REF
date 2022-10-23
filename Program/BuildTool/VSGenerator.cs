// Copyright 2020-2022 Aumoa.lib. All right reserved.

using DotNETUtilities;

using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Xml;

namespace BuildTool;

/// <summary>
/// Visual Studio 프로젝트를 생성합니다.
/// </summary>
public partial class VSGenerator : ISolutionGenerator
{
    // See VSGenerator.StaticConfigurations.cs
    static private readonly BuildTarget Debug;
    static private readonly BuildTarget DebugGame;
    static private readonly BuildTarget Development;
    static private readonly BuildTarget Shipping;

    private readonly CRC32 _hashGenerator = new();
    private readonly Solution _solution;
    private readonly VisualStudioVersion _version;

    private readonly string[] ApplicationMacros;

    /// <summary>
    /// 개체의 새 인스턴스를 초기화합니다.
    /// </summary>
    /// <param name="solution"> 생성할 솔루션 개체를 전달합니다. </param>
    /// <param name="version"> 생성할 버전을 전달합니다. </param>
    public VSGenerator(Solution solution, VisualStudioVersion version)
    {
        _solution = solution;
        _version = version;

        ApplicationMacros = PlatformStatics.Platforms_Macro.Select(p =>
        {
            if (p == "PLATFORM_WINDOWS")
            {
                return $"{p}=1";
            }
            else
            {
                return $"{p}=0";
            }
        }).ToArray();

        if (!_solution.Rule.StaticLibraries)
        {
            ApplicationMacros = ApplicationMacros.Concat(new string[] { "PLATFORM_DYNAMIC_LIBRARY=1" }).ToArray();
        }
    }

    struct SolutionDirectoryPair
    {
        public string RecursivePath;
        public string Guid;
    }

    /// <inheritdoc/>
    public void Generate()
    {
        // Generate XML projects.
        var ProgressBar = ConsoleProgressBar.Create("Generate Projects");
        int Number = 0;
        foreach (var project in _solution.Projects)
        {
            GenerateProject(project);
            ProgressBar.SetProgress(++Number, _solution.Projects.Length);
        }

        StringBuilder Builder = new();

        const string CppProjectGuid = "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942";
        const string SolutionDirectoryGuid = "2150E333-8FDC-42A3-9474-1A3956D46DE8";
        const string CSharpProjectGuid = "9A19103F-16F7-4668-BE54-9A1E7A4F7556";

        Builder.AppendLine("Microsoft Visual Studio Solution File, Format Version 12.00");
        Builder.AppendLine("# Visual Studio Version 17");
        Builder.AppendLine("VisualStudioVersion = 17.2.32602.215");
        Builder.AppendLine("MinimumVisualStudioVersion = 10.0.40219.1");

        Dictionary<Project, string> CachedSlicedSolutionDirectory = new();

        Dictionary<string, SolutionDirectoryPair> SolutionDirectories = new();
        foreach (var (Key, _) in _generatedProjects)
        {
            var Rule = Key.Rule;
            string FilterPath = Rule.FilterPath ?? Rule.Category.ToString();
            string[] RecursivePathSlice = FilterPath.Split(".", StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);

            string RecursivePath = "";
            foreach (var Slice in RecursivePathSlice)
            {
                RecursivePath += Slice;
                SolutionDirectories.TryAdd(Slice, new SolutionDirectoryPair
                {
                    RecursivePath = RecursivePath,
                    Guid = GenerateProjectGuid(RecursivePath).ToString()
                });

                RecursivePath += ".";
            }

            CachedSlicedSolutionDirectory[Key] = RecursivePathSlice[^1];
        }

        IEnumerable<FileReference> Programs;
        if (_solution.ProgramsDirectory != null)
        {
            Programs = _solution.ProgramsDirectory.GetAllFiles().Where(p => p.CompareExtension(".csproj"));
            SolutionDirectories.Add("Program", new SolutionDirectoryPair
            {
                RecursivePath = "Program",
                Guid = GenerateProjectGuid("Program").ToString()
            });
        }
        else
        {
            Programs = Enumerable.Empty<FileReference>();
        }

        foreach (var (Key, Value) in SolutionDirectories)
        {
            Builder.AppendLine(string.Format("Project(\"{{{0}}}\") = \"{1}\", \"{1}\", \"{{{2}}}\"", SolutionDirectoryGuid, Key, Value.Guid.ToString().ToUpper()));
            Builder.AppendLine("EndProject");
        }

        // Generate C++ projects.
        List<string> ProjectsSort = new();
        foreach (var (Key, Value) in _generatedProjects)
        {
            string ProjectDeclare = string.Format("Project(\"{{{0}}}\") = \"{1}\", \"{2}\", \"{{{3}}}\"\nEndProject", CppProjectGuid, Key, Value.File.FullPath, Value.ProjectGuid.ToString().ToUpper());
            ProjectsSort.Add(ProjectDeclare);
        }

        List<Guid> ProgramsGuid = new();

        // Generate C# projects.
        foreach (FileReference CSProj in Programs)
        {
            string ProjectName = CSProj.Name;
            Guid ProjectGuid = GenerateProjectGuid(ProjectName);
            string ProjectDeclare = string.Format("Project(\"{{{0}}}\") = \"{1}\", \"{2}\", \"{{{3}}}\"\nEndProject", CSharpProjectGuid, ProjectName, CSProj.FullPath, ProjectGuid.ToString().ToUpper());
            ProjectsSort.Add(ProjectDeclare);
            ProgramsGuid.Add(ProjectGuid);
        }

        foreach (var Declare in ProjectsSort)
        {
            Builder.AppendLine(Declare);
        }

        string[] Configurations = new[] { "Debug", "DebugGame", "Development", "Shipping" };
        Builder.AppendLine("Global");
        {
            // SolutionConfigurationPlatforms
            Builder.AppendLine("\tGlobalSection(SolutionConfigurationPlatforms) = preSolution");
            {
                foreach (var Config in Configurations)
                {
                    Builder.AppendLine(string.Format("\t\t{0}|x64 = {0}|x64", Config));
                }
            }
            Builder.AppendLine("\tEndGlobalSection");

            // ProjectConfigurationPlatforms
            Dictionary<string, string> ManagedConfigs = new()
            {
                ["Debug"] = "Debug",
                ["DebugGame"] = "Debug",
                ["Development"] = "Release",
                ["Shipping"] = "Release"
            };
            Builder.AppendLine("\tGlobalSection(ProjectConfigurationPlatforms) = postSolution");
            {
                foreach (var (Key, Project) in _generatedProjects)
                {
                    foreach (var Config in Configurations)
                    {
                        Builder.AppendLine(string.Format("\t\t{{{0}}}.{1}|x64.ActiveCfg = {1}|x64", Project.ProjectGuid.ToString().ToUpper(), Config));
                        Builder.AppendLine(string.Format("\t\t{{{0}}}.{1}|x64.Build.0 = {1}|x64", Project.ProjectGuid.ToString().ToUpper(), Config));
                    }
                }

                foreach (var ProjectGuid in ProgramsGuid)
                {
                    foreach (var Config in Configurations)
                    {
                        Builder.AppendLine(string.Format("\t\t{{{0}}}.{1}|x64.ActiveCfg = {2}|Any CPU", ProjectGuid.ToString().ToUpper(), Config, ManagedConfigs[Config]));
                        Builder.AppendLine(string.Format("\t\t{{{0}}}.{1}|x64.Build.0 = {2}|Any CPU", ProjectGuid.ToString().ToUpper(), Config, ManagedConfigs[Config]));
                    }
                }
            }
            Builder.AppendLine("\tEndGlobalSection");

            // SolutionProperties
            Builder.AppendLine("\tGlobalSection(SolutionProperties) = preSolution");
            {
                Builder.AppendLine("\t\tHideSolutionNode = FALSE");
            }
            Builder.AppendLine("\tEndGlobalSection");

            // NestedProjects
            Builder.AppendLine("\tGlobalSection(NestedProjects) = preSolution");
            {
                foreach (var (_, Value) in SolutionDirectories)
                {
                    string[] Splits = Value.RecursivePath.Split(".");
                    if (Splits.Length > 1)
                    {
                        string LastParent = Splits[^2];
                        SolutionDirectoryPair LastParentInfo = SolutionDirectories[LastParent];
                        Builder.AppendLine(string.Format("\t\t{{{0}}} = {{{1}}}", Value.Guid.ToString().ToUpper(), LastParentInfo.Guid.ToString().ToUpper()));
                    }
                }

                foreach (var (Key, Project) in _generatedProjects)
                {
                    SolutionDirectoryPair ParentInfo = SolutionDirectories[CachedSlicedSolutionDirectory[Key]];
                    Builder.AppendLine(string.Format("\t\t{{{0}}} = {{{1}}}", Project.ProjectGuid.ToString().ToUpper(), ParentInfo.Guid.ToString().ToUpper()));
                }

                foreach (var ProjectGuid in ProgramsGuid)
                {
                    SolutionDirectoryPair ParentInfo = SolutionDirectories["Program"];
                    Builder.AppendLine(string.Format("\t\t{{{0}}} = {{{1}}}", ProjectGuid.ToString().ToUpper(), ParentInfo.Guid.ToString().ToUpper()));
                }
            }
            Builder.AppendLine("\tEndGlobalSection");

            // ExtensibilityGlobals
            Builder.AppendLine("\tGlobalSection(ExtensibilityGlobals) = postSolution");
            {
                Builder.AppendLine($"\t\tSolutionGuid = {{{GenerateProjectGuid(_solution.Rule.SolutionName)}}}");
            }
            Builder.AppendLine("\tEndGlobalSection");
        }
        Builder.AppendLine("EndGlobal");

        // Save all documents.
        foreach (var (_, generatedProject) in _generatedProjects)
        {
            generatedProject.File.GetParent().CreateIfNotExists();
            generatedProject.ProjectXml.SaveIfChanged(generatedProject.File);
            generatedProject.FilterXml.SaveIfChanged(generatedProject.File.ChangeExtensions(".vcxproj.filters"));
            generatedProject.UserXml.SaveIfChanged(generatedProject.File.ChangeExtensions(".vcxproj.user"));
            if (generatedProject.VcpkgModule != null)
            {
                generatedProject.File.GetParent().GetFile("vcpkg.json").WriteAllTextIfChanged(generatedProject.VcpkgModule);
            }
        }

        FileReference SolutionFile = _solution.RuleDirectory.GetFile(_solution.Rule.SolutionName + ".sln");
        SolutionFile.WriteAllTextIfChanged(Builder.ToString());
    }

    /// <inheritdoc/>
    public string GenerateAPI(ModuleRule rule, bool isExport)
    {
        if (_solution.Rule.StaticLibraries)
        {
            return rule.SafeProjectName.ToUpper() + "_API=";
        }
        else
        {
            return rule.SafeProjectName.ToUpper() + $"_API=__declspec({(isExport ? "dllexport" : "dllimport")})";
        }
    }

    private Guid GenerateProjectGuid(string seed1)
    {
        uint left = _hashGenerator.Generate32(seed1);
        uint right = left ^ 0xABCDEF01;

        unchecked
        {
            return new Guid(left, (ushort)(right >> 16), (ushort)(right), 0xAE, 0x76, 0xBD, 0x40, 0xA7, 0xB9, 0x2B, 0xCB);
        }
    }

    private string GetConfigurationType(Project project)
    {
        return project.Rule.ModuleType switch
        {
            ModuleType.ConsoleApplication => "Application",
            ModuleType.Application => "Application",
            ModuleType.Library => _solution.Rule.StaticLibraries ? "StaticLibrary" : "DynamicLibrary",
            _ => "Utility"
        };
    }

    private static string GetSubsystemString(Project project)
    {
        return project.Rule.ModuleType switch
        {
            ModuleType.Application => "Windows",
            ModuleType.ConsoleApplication => "Console",
            ModuleType.Library => "Windows",
            _ => "Windows"
        };
    }

    private string PlatformToolset => _version switch
    {
        VisualStudioVersion.VS2019 => "v142",
        VisualStudioVersion.VS2022 => "v143",
        _ => "v143"
    };

    private void GenerateProject(Project project)
    {
        List<BuildConfiguration> Configurations = new();
        switch (project.Rule.Category)
        {
            case ModuleCategory.Game:
                Configurations.Add(Debug.Game);
                Configurations.Add(DebugGame.Game);
                Configurations.Add(Development.Game);
                Configurations.Add(Shipping.Game);
                break;
            case ModuleCategory.Engine:
                Configurations.Add(Debug.Engine);
                Configurations.Add(DebugGame.Engine);
                Configurations.Add(Development.Engine);
                Configurations.Add(Shipping.Engine);
                break;
            case ModuleCategory.ThirdParty:
                Configurations.Add(Debug.ThirdParty);
                Configurations.Add(DebugGame.ThirdParty);
                Configurations.Add(Development.ThirdParty);
                Configurations.Add(Shipping.ThirdParty);
                break;
        }

        Guid projectGuid = GenerateProjectGuid(project.Rule.ProjectName);

        // Begin of document.
        XmlDocument Doc = new();
        Doc.NewDeclaration();

        // Begin <Project>
        XmlElement Project = Doc.NewElement("Project");
        {
            Project.SetAttribute("DefaultTargets", "Build");
            Project.SetAttribute("xmlns", "http://schemas.microsoft.com/developer/msbuild/2003");

            XmlElement ItemGroup = Project.NewElementSingleAttr("ItemGroup", "Label", "ProjectConfigurations");
            {
                foreach (var Config in Configurations)
                {
                    ItemGroup.NewProjectConfiguration(Config.Name + "|x64", Config.Configuration, "x64");
                }
            }

            XmlElement PropertyGroup = Project.NewElementPropertyGroup("", "Globals");
            {
                PropertyGroup.NewElement("VCProjectVersion", "16.0");
                PropertyGroup.NewElement("Keyword", "Win32Proj");
                PropertyGroup.NewElement("ProjectGuid", string.Format("{{{0}}}", projectGuid.ToString()));
                PropertyGroup.NewElement("RootNamespace", project.Rule.ProjectName);
                PropertyGroup.NewElement("WindowsTargetPlatformVersion", "10.0");

                if (project.Rule.SupportsVcpkg)
                {
                    string Trip = _solution.Rule.StaticLibraries ? "-static" : "";

                    PropertyGroup.NewElement("VcpkgEnabled", "true");
                    PropertyGroup.NewElement("VcpkgTriplet", $"x64-windows{Trip}");
                    PropertyGroup.NewElement("VcpkgEnableManifest", "true");
                }
            }

            Project.NewElementImport("$(VCTargetsPath)\\Microsoft.Cpp.Default.props");

            foreach (var Config in Configurations)
            {
                string Condition = $"'$(Configuration)|$(Platform)'=='{Config.Name}|x64'";

                XmlElement InnerPropGroup = Project.NewElementPropertyGroup(Condition, "Configuration");
                {
                    InnerPropGroup.NewElement("ConfigurationType", GetConfigurationType(project));
                    InnerPropGroup.NewElement("UseDebugLibraries", Config.bUseDebugLibrary.ToString());
                    InnerPropGroup.NewElement("PlatformToolset", PlatformToolset);
                    InnerPropGroup.NewElement("WholeProgramOptimization", Config.bWholeProgramOptimization.ToString());
                    InnerPropGroup.NewElement("CharacterSet", "Unicode");
                    InnerPropGroup.NewElement("EnableUnitySupport", (!project.Rule.NonUnityBuild).ToString());
                }
            }

            Project.NewElementImport("$(VCTargetsPath)\\Microsoft.Cpp.props");
            Project.NewElementImportGroup("ExtensionSettings");
            Project.NewElementImportGroup("Shared");

            foreach (var Config in Configurations)
            {
                string Condition = $"'$(Configuration)|$(Platform)'=='{Config.Name}|x64'";

                XmlElement ImportGroup = Project.NewElementImportGroup("PropertySheets", Condition);
                {
                    ImportGroup.NewElementImport("$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props", "exists(\'$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\')", "LocalAppDataPlatform");
                }
            }

            Project.NewElementPropertyGroup("", "UserMacros");
            foreach (var Config in Configurations)
            {
                string Condition = $"'$(Configuration)|$(Platform)'=='{Config.Name}|x64'";

                XmlElement InnerPropGroup = Project.NewElementPropertyGroup(Condition);
                {
                    string BuildDir = project.SolutionDirectory.Build.FullPath;
                    string IntermediateDir = project.SolutionDirectory.Intermediate.FullPath;

                    InnerPropGroup.NewElement("LinkIncremental", Config.bLinkIncremental.ToString());
                    InnerPropGroup.NewElement("OutDir", $"{BuildDir}\\Win64\\$(Configuration)\\");
                    InnerPropGroup.NewElement("IntDir", $"{IntermediateDir}\\$(Configuration)\\$(ProjectName)\\");
                    InnerPropGroup.NewElement("TargetName", project.Rule.TargetName ?? project.Rule.ProjectName);
                    if (project.Rule.SupportsVcpkg)
                    {
                        InnerPropGroup.NewElement("VcpkgUseStatic", _solution.Rule.StaticLibraries.ToString());
                        InnerPropGroup.NewElement("VcpkgConfiguration", Config.bUseDebugLibrary ? "Debug" : "Release");
                    }
                }
            }

            foreach (var Config in Configurations)
            {
                string Condition = $"'$(Configuration)|$(Platform)'=='{Config.Name}|x64'";

                XmlElement ItemDefinitionGroup = Project.NewElementItemDefinitionGroup(Condition);
                {
                    string AdditionalIncludeDirectories = string.Join(";", project.IncludePaths);

                    XmlElement ClCompile = ItemDefinitionGroup.NewElement("ClCompile");
                    {
                        ClCompile.NewElement("WarningLevel", "Level3");
                        ClCompile.NewElement("FunctionLevelLinking", Config.bFunctionLevelLinking.ToString());
                        ClCompile.NewElement("IntrinsicFunctions", Config.bIntrinsicFunctions.ToString());
                        ClCompile.NewElement("SDLCheck", "true");
                        ClCompile.NewElement("ConformanceMode", "true");
                        ClCompile.NewElement("LanguageStandard", "stdcpplatest");
                        ClCompile.NewElement("MultiProcessorCompilation", "true");
                        ClCompile.NewElement("AdditionalIncludeDirectories", AdditionalIncludeDirectories);

                        List<string> Macros = new() { Config.Macros };
                        Macros.AddRange(Enumerable.Concat(ApplicationMacros, project.AdditionalMacros));
                        Macros.Add("%(PreprocessorDefinitions)");
                        Macros.AddRange(project.GenerateAPIs(this));
                        ClCompile.NewElement("PreprocessorDefinitions", string.Join(";", Macros));

                        ClCompile.NewElement("UseStandardPreprocessor", "true");
                        ClCompile.NewElement("DisableSpecificWarnings", string.Join(";", project.DisableWarnings.Select(p => p.ToString())));
                        ClCompile.NewElement("Optimization", Config.Optimization);
                        ClCompile.NewElement("BasicRuntimeChecks", Config.BasicRuntimeChecks);
                        ClCompile.NewElement("DebugInformationFormat", Config.DebugInformationFormat);
                        if (!string.IsNullOrEmpty(project.Rule.PrivatePCHHeader))
                        {
                            string PCHHeader = Path.ChangeExtension(project.Rule.PrivatePCHHeader, ".h");
                            ClCompile.NewElement("ForcedIncludeFiles", PCHHeader);
                            ClCompile.NewElement("PrecompiledHeader", "Use");
                            ClCompile.NewElement("PrecompiledHeaderFile", PCHHeader);
                        }
                        ClCompile.NewElement("AdditionalOptions", "/bigobj %(AdditionalOptions)");
                        ClCompile.NewElement("RuntimeLibrary", Config.RuntimeLibrary + (_solution.Rule.StaticLibraries ? "" : "DLL"));
                        ClCompile.NewElement("ExceptionHandling", Config.ExceptionHandling);
                    }

                    XmlElement Link = ItemDefinitionGroup.NewElement("Link");
                    {
                        Link.NewElement("SubSystem", GetSubsystemString(project));
                        Link.NewElement("EnableCOMDATFolding", Config.bEnableCOMDATFolding.ToString());
                        Link.NewElement("OptimizeReferences", Config.bOptimizeReferences.ToString());
                        Link.NewElement("GenerateDebugInformation", "true");
                        Link.NewElement("EnableUAC", "false");
                        Link.NewElement("AdditionalDependencies", string.Join(";", project.AdditionalLibraries) + ";$(AdditionalDependencies)");
                    }

                    XmlElement PreBuildEvent = ItemDefinitionGroup.NewElement("PreBuildEvent");
                    {
                        FileReference ReflectionHeaderTool = _solution.ProgramsDirectory.GetParent().Move("Binaries\\ReflectionHeaderTool").GetFile("ReflectionHeaderTool.dll");
                        PreBuildEvent.NewElement("Command", $"dotnet \"{ReflectionHeaderTool}\" -s \"{project.Directory}\" -o \"{project.IntermediateIncludePath}\"");
                    }

                    XmlElement FxCompile = ItemDefinitionGroup.NewElement("FxCompile");
                    {
                        FxCompile.NewElement("VariableName", "HLSL_%(Filename)");
                        FxCompile.NewElement("HeaderFileOutput", $"{project.IntermediateIncludePath}\\%(Filename).generated.h");
                        FxCompile.NewElement("ShaderModel", "5.1");
                        FxCompile.NewElement("AdditionalIncludeDirectories", AdditionalIncludeDirectories);
                    }
                }
            }

            ItemGroup = Project.NewElement("ItemGroup");
            {
                foreach (var IncludeItem in project.Directory.GetAllFiles())
                {
                    string IncludePath = IncludeItem.FullPath;
                    string Extension = Path.GetExtension(IncludePath);

                    if (project.Rule.SourceCodeExtensions.Contains(Extension))
                    {
                        var ClCompile = ItemGroup.NewElementItemInclude("ClCompile", IncludePath);

                        if (!string.IsNullOrEmpty(project.Rule.PrivatePCHHeader))
                        {
                            string PCHCreate = Path.GetFileNameWithoutExtension(project.Rule.PrivatePCHHeader);
                            string IncludeName = Path.GetFileNameWithoutExtension(IncludePath);

                            if (PCHCreate == IncludeName)
                            {
                                ClCompile.NewElement("PrecompiledHeader", "Create");
                            }
                        }
                    }
                    else if (project.Rule.HeaderExtensions.Contains(Extension))
                    {
                        ItemGroup.NewElementItemInclude("ClInclude", IncludePath);
                    }
                    else if (project.Rule.NonBuildExtensions.Contains(Extension))
                    {
                        ItemGroup.NewElementItemInclude("None", IncludePath);
                    }

                    // VisualStudio Specialization.
                    else if (IncludeItem.CompareExtension(".natvis"))
                    {
                        ItemGroup.NewElementItemInclude("Natvis", IncludePath);
                    }
                    else if (Extension == ".hlsli")
                    {
                        ItemGroup.NewElementItemInclude("None", IncludePath);
                    }
                    else if (Extension == ".hlsl")
                    {
                        XmlElement FxCompile = ItemGroup.NewElementItemInclude("FxCompile", IncludePath);

                        var Filename = Path.GetFileNameWithoutExtension(IncludePath);
                        if (Filename.EndsWith("VertexShader"))
                        {
                            FxCompile.NewElement("ShaderType", "Vertex");
                            FxCompile.NewElement("EntryPointName", "VSMain");
                        }
                        else if (Filename.EndsWith("PixelShader"))
                        {
                            FxCompile.NewElement("ShaderType", "Pixel");
                            FxCompile.NewElement("EntryPointName", "PSMain");
                        }
                        else if (Filename.EndsWith("ShaderLibrary"))
                        {
                            FxCompile.NewElement("ShaderType", "Library");
                            FxCompile.NewElement("ShaderModel", "6.3");
                            FxCompile.NewElement("EntryPointName", "");
                        }
                    }
                }
            }

            ItemGroup = Project.NewElement("ItemGroup");
            {
                void ProjectReferenceBody(Project It)
                {
                    ModuleRule CompiledRule = It.Rule;
                    if (CompiledRule.ModuleType != ModuleType.None)
                    {
                        DirectoryReference ProjectFileDir = GetProjectFileIntermediateDirectory(It);
                        string ProjectName = It.Rule.ProjectName + ".vcxproj";

                        string ProjectPath = Path.Combine(ProjectFileDir.FullPath, ProjectName);
                        XmlElement ProjectReference = ItemGroup.NewElementItemInclude("ProjectReference", ProjectPath);
                        {
                            ProjectReference.NewElement("Project", string.Format("{{{0}}}", _generatedProjects[It].ProjectGuid));
                        }
                    }
                }

                foreach (var Item in project.DependencyModules)
                {
                    ProjectReferenceBody(Item);
                }
            }

            Project.NewElementImport("$(VCTargetsPath)\\Microsoft.Cpp.targets");
        }

        _generatedProjects.Add(project, new ProjectFileReference
        {
            File = GetProjectFileIntermediateDirectory(project).GetFile(project.Rule.ProjectName + ".vcxproj"),
            ProjectXml = Doc,
            FilterXml = GenerateFilter(project),
            UserXml = GenerateUser(Configurations),
            VcpkgModule = project.Rule.SupportsVcpkg ? GenerateVcpkg(project) : null,
            ProjectGuid = projectGuid
        });
    }

    private record JsonPackages
    {
        [JsonPropertyName("$schema")]
        public string Schema { get; set; } = "https://raw.githubusercontent.com/microsoft/vcpkg/master/scripts/vcpkg.schema.json";

        [JsonPropertyName("name")]
        public string Name { get; set; } = null!;

        [JsonPropertyName("version")]
        public string Version { get; set; } = "0.0.1";

        [JsonPropertyName("dependencies")]
        public List<string> Dependencies { get; set; } = new();
    }

    private static string GenerateVcpkg(Project project)
    {
        JsonPackages jpkg = new()
        {
            Name = project.Rule.ProjectName.ToLower(),
            Version = "0.0.1"
        };

        foreach (var vcpkg in project.VcpkgModules)
        {
            jpkg.Dependencies.Add(vcpkg);
        }

        return JsonSerializer.Serialize(jpkg);
    }

    private XmlDocument GenerateFilter(Project project)
    {
        XmlDocument Doc = new();
        Doc.NewDeclaration();

        XmlElement Project = Doc.NewElement("Project");
        {
            Project.SetAttribute("ToolsVersion", "4.0");
            Project.SetAttribute("xmlns", @"http://schemas.microsoft.com/developer/msbuild/2003");

            XmlElement ItemGroup = Project.NewElement("ItemGroup");
            {
                var AbsolutePath = new DirectoryReference(project.Directory.FullPath);
                AbsolutePath.CreateIfNotExists(true);

                HashSet<string> Filters = new();

                foreach (var IncludeItem in AbsolutePath.GetAllFiles())
                {
                    XmlElement? InnerItem = null;
                    string Extension = IncludeItem.Extension ?? "";

                    if (project.Rule.SourceCodeExtensions.Contains(Extension))
                    {
                        InnerItem = ItemGroup.NewElementItemInclude("ClCompile", IncludeItem.FullPath);
                    }
                    else if (project.Rule.HeaderExtensions.Contains(Extension))
                    {
                        InnerItem = ItemGroup.NewElementItemInclude("ClInclude", IncludeItem.FullPath);
                    }
                    else if (project.Rule.NonBuildExtensions.Contains(Extension))
                    {
                        InnerItem = ItemGroup.NewElementItemInclude("None", IncludeItem.FullPath);
                    }

                    // VisualStudio Specialization.
                    else if (IncludeItem.CompareExtension(".natvis"))
                    {
                        InnerItem = ItemGroup.NewElementItemInclude("Natvis", IncludeItem.FullPath);
                    }
                    else if (IncludeItem.CompareExtension(".hlsli"))
                    {
                        InnerItem = ItemGroup.NewElementItemInclude("None", IncludeItem.FullPath);
                    }
                    else if (IncludeItem.CompareExtension(".hlsl"))
                    {
                        InnerItem = ItemGroup.NewElementItemInclude("FxCompile", IncludeItem.FullPath);
                    }

                    if (InnerItem != null)
                    {
                        string AbsolutePathStr = AbsolutePath.FullPath;
                        string IncludeItemPath = Directory.GetParent(IncludeItem.FullPath)?.FullName ?? "";
                        string FilterPath = Path.GetRelativePath(AbsolutePathStr, IncludeItemPath);

                        if (!string.IsNullOrEmpty(FilterPath) && FilterPath != ".")
                        {
                            InnerItem.NewElement("Filter", FilterPath);
                            Filters.Add(FilterPath);

                            int LastIndex;
                            while ((LastIndex = FilterPath.LastIndexOf(Path.DirectorySeparatorChar)) != -1)
                            {
                                FilterPath = FilterPath[..LastIndex];
                                Filters.Add(FilterPath);
                            }
                        }
                    }
                }

                foreach (var Filter in Filters)
                {
                    XmlElement InnerItem = ItemGroup.NewElementItemInclude("Filter", Filter);
                    InnerItem.NewElement("UniqueIdentifier", $"{{{GenerateProjectGuid(Filter)}}}");
                }
            }
        }

        return Doc;
    }

    private XmlDocument GenerateUser(IEnumerable<BuildConfiguration> Configurations)
    {
        XmlDocument Doc = new();
        Doc.NewDeclaration();

        XmlElement Project = Doc.NewElement("Project");
        {
            Project.SetAttribute("ToolsVersion", "Current");
            Project.SetAttribute("xmlns", @"http://schemas.microsoft.com/developer/msbuild/2003");

            foreach (var Config in Configurations)
            {
                string Condition = $"'$(Configuration)|$(Platform)'=='{Config.Name}|x64'";

                XmlElement PropertyGroup = Project.NewElementPropertyGroup(Condition);
                {
                    PropertyGroup.NewElement("LocalDebuggerWorkingDirectory", "$(SolutionDir)");
                    PropertyGroup.NewElement("DebuggerFlavor", "WindowsLocalDebugger");

                    string Paths = ";%PATH%";
                    if (_solution.EngineDirectory != null)
                    {
                        Paths = Path.Combine(_solution.EngineDirectory.Build.FullPath, "Win64", "$(Configuration)") + Paths;
                    }
                    PropertyGroup.NewElement("LocalDebuggerEnvironment", $"Path={Paths}");
                }
            }
        }

        return Doc;
    }

    private DirectoryReference GetProjectFileIntermediateDirectory(Project project)
    {
        return _solution.Directory.Intermediate.Move("ProjectFiles").Move(project.Rule.ProjectName);
    }

    private record ProjectFileReference
    {
        public FileReference File { get; set; } = null!;
        public XmlDocument ProjectXml { get; set; } = null!;
        public XmlDocument FilterXml { get; set; } = null!;
        public XmlDocument UserXml { get; set; } = null!;
        public string? VcpkgModule { get; set; } = null!;

        public Guid ProjectGuid { get; set; }
    }

    private readonly Dictionary<Project, ProjectFileReference> _generatedProjects = new();
}
