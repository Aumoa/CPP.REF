// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

using DotNETUtilities;

using System.Security.Cryptography;
using System.Xml;

namespace BuildTool;

class GeneratedVSProject : IGeneratedProject
{
    struct BuildConfiguration
    {
        public string Name;
        public string Configuration;
        public string Optimization;
        public string BasicRuntimeChecks;
        public string Macros;
        public string DebugInformationFormat;
        public bool bUseDebugLibrary;
        public bool bWholeProgramOptimization;
        public bool bLinkIncremental;
        public bool bFunctionLevelLinking;
        public bool bIntrinsicFunctions;
        public bool bEnableCOMDATFolding;
        public bool bOptimizeReferences;
    }

    struct BuildTarget
    {
        public BuildConfiguration Game;
        public BuildConfiguration ThirdParty;
        public BuildConfiguration Engine;

        public BuildConfiguration this[TargetType Index]
        {
            get
            {
                switch (Index)
                {
                    case TargetType.Game:
                        return Game;
                    case TargetType.ThirdParty:
                        return ThirdParty;
                    case TargetType.Engine:
                        return Engine;
                    default:
                        throw new ArgumentException(Index.ToString());
                }
            }
        }
    }

    static BuildTarget Debug;
    static BuildTarget DebugGame;
    static BuildTarget Release;

    static GeneratedVSProject()
    {
        Debug = new BuildTarget
        {
            Game = new BuildConfiguration
            {
                Name = "Debug",
                Configuration = "Debug",
                Optimization = "Disabled",
                BasicRuntimeChecks = "EnableFastChecks",
                Macros = "DO_CHECK=1;SHIPPING=0;",
                DebugInformationFormat = "EditAndContinue",
                bUseDebugLibrary = true,
                bWholeProgramOptimization = false,
                bLinkIncremental = true,
                bFunctionLevelLinking = true,
                bIntrinsicFunctions = false,
                bEnableCOMDATFolding = false,
                bOptimizeReferences = false
            },
            Engine = new BuildConfiguration
            {
                Name = "Debug",
                Configuration = "Debug",
                Optimization = "Disabled",
                BasicRuntimeChecks = "EnableFastChecks",
                Macros = "DO_CHECK=1;SHIPPING=0;",
                DebugInformationFormat = "EditAndContinue",
                bUseDebugLibrary = true,
                bWholeProgramOptimization = false,
                bLinkIncremental = true,
                bFunctionLevelLinking = true,
                bIntrinsicFunctions = false,
                bEnableCOMDATFolding = false,
                bOptimizeReferences = false
            },
            ThirdParty = new BuildConfiguration
            {
                Name = "Debug",
                Configuration = "Debug",
                Optimization = "MaxSpeed",
                BasicRuntimeChecks = "Default",
                Macros = "DO_CHECK=0;SHIPPING=0;",
                DebugInformationFormat = "ProgramDatabase",
                bUseDebugLibrary = true,
                bWholeProgramOptimization = true,
                bLinkIncremental = false,
                bFunctionLevelLinking = true,
                bIntrinsicFunctions = true,
                bEnableCOMDATFolding = true,
                bOptimizeReferences = true
            }
        };

        DebugGame = new BuildTarget
        {
            Game = new BuildConfiguration
            {
                Name = "DebugGame",
                Configuration = "DebugGame",
                Optimization = "Disabled",
                BasicRuntimeChecks = "EnableFastChecks",
                Macros = "DO_CHECK=1;SHIPPING=0;",
                DebugInformationFormat = "EditAndContinue",
                bUseDebugLibrary = true,
                bWholeProgramOptimization = false,
                bLinkIncremental = true,
                bFunctionLevelLinking = true,
                bIntrinsicFunctions = false,
                bEnableCOMDATFolding = false,
                bOptimizeReferences = false
            },
            Engine = new BuildConfiguration
            {
                Name = "DebugGame",
                Configuration = "DebugGame",
                Optimization = "MaxSpeed",
                BasicRuntimeChecks = "Default",
                Macros = "DO_CHECK=1;SHIPPING=0;",
                DebugInformationFormat = "ProgramDatabase",
                bUseDebugLibrary = true,
                bWholeProgramOptimization = true,
                bLinkIncremental = false,
                bFunctionLevelLinking = true,
                bIntrinsicFunctions = true,
                bEnableCOMDATFolding = true,
                bOptimizeReferences = true
            },
            ThirdParty = new BuildConfiguration
            {
                Name = "DebugGame",
                Configuration = "DebugGame",
                Optimization = "MaxSpeed",
                BasicRuntimeChecks = "Default",
                Macros = "DO_CHECK=0;SHIPPING=0;",
                DebugInformationFormat = "ProgramDatabase",
                bUseDebugLibrary = true,
                bWholeProgramOptimization = true,
                bLinkIncremental = false,
                bFunctionLevelLinking = true,
                bIntrinsicFunctions = true,
                bEnableCOMDATFolding = true,
                bOptimizeReferences = true
            }
        };

        Release = new BuildTarget
        {
            Game = new BuildConfiguration
            {
                Name = "Release",
                Configuration = "Release",
                Optimization = "MaxSpeed",
                BasicRuntimeChecks = "Default",
                Macros = "DO_CHECK=0;SHIPPING=1;",
                DebugInformationFormat = "ProgramDatabase",
                bUseDebugLibrary = false,
                bWholeProgramOptimization = true,
                bLinkIncremental = false,
                bFunctionLevelLinking = true,
                bIntrinsicFunctions = true,
                bEnableCOMDATFolding = true,
                bOptimizeReferences = true
            },
            Engine = new BuildConfiguration
            {
                Name = "Release",
                Configuration = "Release",
                Optimization = "MaxSpeed",
                BasicRuntimeChecks = "Default",
                Macros = "DO_CHECK=0;SHIPPING=1;",
                DebugInformationFormat = "ProgramDatabase",
                bUseDebugLibrary = false,
                bWholeProgramOptimization = true,
                bLinkIncremental = false,
                bFunctionLevelLinking = true,
                bIntrinsicFunctions = true,
                bEnableCOMDATFolding = true,
                bOptimizeReferences = true
            },
            ThirdParty = new BuildConfiguration
            {
                Name = "Release",
                Configuration = "Release",
                Optimization = "MaxSpeed",
                BasicRuntimeChecks = "Default",
                Macros = "DO_CHECK=0;SHIPPING=1;",
                DebugInformationFormat = "ProgramDatabase",
                bUseDebugLibrary = false,
                bWholeProgramOptimization = true,
                bLinkIncremental = false,
                bFunctionLevelLinking = true,
                bIntrinsicFunctions = true,
                bEnableCOMDATFolding = true,
                bOptimizeReferences = true
            }
        };
    }

    public readonly Project CompiledProject;
    private bool bGenerated;

    private List<BuildConfiguration> Configurations = new();
    private ModuleRule Rule;
    public readonly Guid GeneratedGuid;

    public GeneratedVSProject(Project CompiledProject)
    {
        this.CompiledProject = CompiledProject;
        Rule = CompiledProject.CompiledRule;
        GeneratedGuid = Rule.GenerateGuid(Rule.Name);

        switch (Rule.TargetType)
        {
            case TargetType.Game:
                Configurations.Add(Debug.Game);
                Configurations.Add(DebugGame.Game);
                Configurations.Add(Release.Game);
                break;
            case TargetType.Engine:
                Configurations.Add(Debug.Engine);
                Configurations.Add(DebugGame.Engine);
                Configurations.Add(Release.Engine);
                break;
            case TargetType.ThirdParty:
                Configurations.Add(Debug.ThirdParty);
                Configurations.Add(DebugGame.ThirdParty);
                Configurations.Add(Release.ThirdParty);
                break;
        }
    }

    private VisualStudioVersion VSVersion;

    private List<GeneratedVSProject> PublicDependencyModules = new();
    private List<GeneratedVSProject> PrivateDependencyModules = new();
    private List<GeneratedVSProject> DependencyModules = new();

    private string IntermediateProjectPath;

    public void Generate(VisualStudioGenerator SlnGenerator)
    {
        VSVersion = SlnGenerator.VSVersion;

        if (!bGenerated)
        {
            ModuleRule Rule = CompiledProject.CompiledRule;

            // Make dependencies.
            List<GeneratedVSProject> PublicDependencyModules = new();
            List<GeneratedVSProject> PrivateDependencyModules = new();

            void MakeDependencyModules(IList<GeneratedVSProject> List, IList<string> Names)
            {
                foreach (var ModuleName in Names)
                {
                    if (!SlnGenerator.GeneratedProjects.TryGetValue(ModuleName, out var DependencyModule))
                    {
                        Console.WriteLine("Couldn't find dependency module '{0}' while load '{1}' project", ModuleName, Rule.Name);
                        continue;
                    }

                    DependencyModule.Generate(SlnGenerator);
                    List.Add(DependencyModule);
                }
            }

            MakeDependencyModules(PublicDependencyModules, CompiledProject.CompiledRule.PublicDependencyModuleNames);
            MakeDependencyModules(PrivateDependencyModules, CompiledProject.CompiledRule.PrivateDependencyModuleNames);

            foreach (var Module in PublicDependencyModules)
            {
                this.PublicDependencyModules.AddRange(Module.PublicDependencyModules);
            }

            foreach (var Module in PrivateDependencyModules)
            {
                this.PrivateDependencyModules.AddRange(Module.PrivateDependencyModules);
            }

            this.PublicDependencyModules.AddRange(PublicDependencyModules);
            this.PublicDependencyModules = this.PublicDependencyModules.Distinct().ToList();

            this.PrivateDependencyModules.AddRange(PrivateDependencyModules);
            this.PrivateDependencyModules = this.PrivateDependencyModules.Distinct().ToList();

            DependencyModules.AddRange(this.PublicDependencyModules);
            DependencyModules.AddRange(this.PrivateDependencyModules);
            DependencyModules = DependencyModules.Distinct().ToList();

            GenerateRuntime(SlnGenerator);

            IntermediateProjectPath = "Intermediate/ProjectFiles";
            foreach (var Split in Rule.RelativePath.Split(".", StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries))
            {
                IntermediateProjectPath = Path.Combine(IntermediateProjectPath, Split);
            }

            // Create directory.
            new DirectoryReference(IntermediateProjectPath).CreateIfNotExists(true);

            GenerateXml();
            GenerateFilterXml();
            GenerateUserXml();

            bGenerated = true;
        }
    }

    private List<string> AdditionalIncludeDirectories = new();
    private List<string> PreprocessorDefines = new();
    private List<string> DisableSpecificWarnings = new();
    private List<string> AdditionalLibraries = new();
    private string ApplicationMacros = "";

    private void GenerateRuntime(VisualStudioGenerator SlnGenerator)
    {
        foreach (var Module in DependencyModules)
        {
            var Rule = Module.CompiledProject.CompiledRule;

            AdditionalIncludeDirectories.AddRange(Rule.PublicIncludePaths);
            PreprocessorDefines.AddRange(Rule.PublicPreprocessorDefines);
            DisableSpecificWarnings.AddRange(Rule.PublicDisableWarnings.Select((x) => x.ToString()));
            AdditionalLibraries.AddRange(Rule.PublicAdditionalLibraries);

            if (Rule.ModuleType == ModuleType.GameModule || Rule.ModuleType == ModuleType.ConsoleModule)
            {
                PreprocessorDefines.Add(Rule.GetAPI(false));
            }
        }

        {
            var Rule = CompiledProject.CompiledRule;
            Rule.GenerateAdditionalIncludeDirectories(CompiledProject.ProjectDir.FullPath);

            AdditionalIncludeDirectories.AddRange(Rule.PublicIncludePaths);
            AdditionalIncludeDirectories.AddRange(Rule.PrivateIncludePaths);
            AdditionalIncludeDirectories = AdditionalIncludeDirectories.Distinct().ToList();
            PreprocessorDefines.AddRange(Rule.PublicPreprocessorDefines);
            PreprocessorDefines.AddRange(Rule.PrivatePreprocessorDefines);
            PreprocessorDefines = PreprocessorDefines.Distinct().ToList();
            DisableSpecificWarnings.AddRange(Rule.PublicDisableWarnings.Select((x) => x.ToString()));
            DisableSpecificWarnings.AddRange(Rule.PrivateDisableWarnings.Select((x) => x.ToString()));
            DisableSpecificWarnings = DisableSpecificWarnings.Distinct().ToList();
            AdditionalLibraries.AddRange(Rule.PublicAdditionalLibraries);
            AdditionalLibraries.AddRange(Rule.PrivateAdditionalLibraries);
            AdditionalLibraries = AdditionalLibraries.Distinct().ToList();

            if (Rule.ModuleType == ModuleType.GameModule || Rule.ModuleType == ModuleType.ConsoleModule)
            {
                PreprocessorDefines.Add(Rule.GetAPI(true));
            }
        }

        ApplicationMacros += $"SE_APPLICATION=\"{SlnGenerator.Solution.CompiledRule.Name}\";";
        ApplicationMacros += $"SE_APPLICATION_TARGET=\"{SlnGenerator.PrimaryModule.CompiledRule.TargetName}\";";
        ApplicationMacros += Environment.OSVersion.Platform switch
        {
            PlatformID.Unix => $"PLATFORM_LINUX=1;",
            PlatformID.Win32NT => $"PLATFORM_WINDOWS=1;",
            _ => throw new PlatformNotSupportedException()
        };
    }

    private void GenerateXml()
    {
        // Create declaration.
        XmlDocument Doc = new();
        Doc.CreateXmlDeclaration();

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
			    PropertyGroup.NewElement("ProjectGuid", string.Format("{{{0}}}", GeneratedGuid.ToString()));
			    PropertyGroup.NewElement("RootNamespace", Rule.Name);
			    PropertyGroup.NewElement("WindowsTargetPlatformVersion", "10.0");
		    }

		    Project.NewElementImport("$(VCTargetsPath)\\Microsoft.Cpp.Default.props");

		    foreach (var Config in Configurations)
		    {
                string Condition = $"'$(Configuration)|$(Platform)'=='{Config.Name}|x64'";

			    XmlElement InnerPropGroup = Project.NewElementPropertyGroup(Condition, "Configuration");
			    {
				    InnerPropGroup.NewElement("ConfigurationType", Rule.ModuleType.GetTypeString());
				    InnerPropGroup.NewElement("UseDebugLibraries", Config.bUseDebugLibrary.ToString());
				    InnerPropGroup.NewElement("PlatformToolset", VSVersion.GetPlatformToolsets());
				    InnerPropGroup.NewElement("WholeProgramOptimization", Config.bWholeProgramOptimization.ToString());
				    InnerPropGroup.NewElement("CharacterSet", "Unicode");
				    InnerPropGroup.NewElement("EnableUnitySupport", "true");
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
				    InnerPropGroup.NewElement("LinkIncremental", Config.bLinkIncremental.ToString());
				    InnerPropGroup.NewElement("OutDir", "$(SolutionDir)Build\\");
				    InnerPropGroup.NewElement("IntDir", "$(SolutionDir)Intermediate\\$(Configuration)\\$(ProjectName)\\");
				    InnerPropGroup.NewElement("TargetName", Rule.TargetName);
			    }
		    }

		    foreach (var Config in Configurations)
            {
                string Condition = $"'$(Configuration)|$(Platform)'=='{Config.Name}|x64'";

                XmlElement ItemDefinitionGroup = Project.NewElementItemDefinitionGroup(Condition);
			    {
				    XmlElement ClCompile = ItemDefinitionGroup.NewElement("ClCompile");
				    {
					    ClCompile.NewElement("WarningLevel", "Level3");
					    ClCompile.NewElement("FunctionLevelLinking", Config.bFunctionLevelLinking.ToString());
					    ClCompile.NewElement("IntrinsicFunctions", Config.bIntrinsicFunctions.ToString());
					    ClCompile.NewElement("SDLCheck", "true");
					    ClCompile.NewElement("ConformanceMode", "true");
					    ClCompile.NewElement("LanguageStandard", "stdcpplatest");
					    ClCompile.NewElement("MultiProcessorCompilation", "true");
					    ClCompile.NewElement("AdditionalIncludeDirectories", string.Join(";", AdditionalIncludeDirectories));
					    ClCompile.NewElement("PreprocessorDefinitions", Config.Macros + ApplicationMacros + string.Join(";", PreprocessorDefines));
					    ClCompile.NewElement("UseStandardPreprocessor", "true");
					    ClCompile.NewElement("DisableSpecificWarnings", string.Join(";", DisableSpecificWarnings));
					    ClCompile.NewElement("Optimization", Config.Optimization);
					    ClCompile.NewElement("BasicRuntimeChecks", Config.BasicRuntimeChecks);
                        ClCompile.NewElement("DebugInformationFormat", Config.DebugInformationFormat);
                    }

				    XmlElement Link = ItemDefinitionGroup.NewElement("Link");
				    {
					    Link.NewElement("SubSystem", CompiledProject.CompiledRule.ModuleType.GetSubsystemString());
					    Link.NewElement("EnableCOMDATFolding", Config.bEnableCOMDATFolding.ToString());
					    Link.NewElement("OptimizeReferences", Config.bOptimizeReferences.ToString());
					    Link.NewElement("GenerateDebugInformation", "true");
					    Link.NewElement("EnableUAC", "false");
					    Link.NewElement("AdditionalDependencies", string.Join(";", AdditionalLibraries));
				    }

				    if (CompiledProject.CompiledRule.ModuleType == ModuleType.Vcpkg)
				    {
					    XmlElement PostBuildEvent = ItemDefinitionGroup.NewElement("PostBuildEvent");
					    {
						    string[] Commands = new[]
						    {
							    $"(pushd {CompiledProject.ProjectDir.FullPath}\\..\\vcpkg\\)",
							    $"(IF NOT EXIST \"vcpkg.exe\" ( bootstrap-vcpkg.bat ))",
							    $"(vcpkg.exe install {CompiledProject.CompiledRule.TargetName}:x64-windows)",
							    $"(vcpkg.exe integrate install)",
							    $"(popd)"
						    };
						    PostBuildEvent.NewElement("Command", string.Join("\n", Commands));
					    }
				    }
			    }
		    }

		    ItemGroup = Project.NewElement("ItemGroup");
		    {
			    string RelativePath = CompiledProject.ProjectDir.FullPath;
                RelativePath = RelativePath.Replace("$(SolutionDir)", ".\\");

                DirectoryReference AbsolutePath = new(RelativePath);
                AbsolutePath.CreateIfNotExists();

			    foreach (var IncludeItem in AbsolutePath.GetAllFiles())
			    {
				    if (IncludeItem.CompareExtension(".cpp") ||
                        IncludeItem.CompareExtension(".ixx") ||
                        IncludeItem.CompareExtension(".c"))
				    {
					    ItemGroup.NewElementItemInclude("ClCompile", IncludeItem.FullPath);
				    }
				    else if (IncludeItem.CompareExtension(".h")
                        ||   IncludeItem.CompareExtension(".inl"))
				    {
					    ItemGroup.NewElementItemInclude("ClInclude", IncludeItem.FullPath);
				    }
				    else if (IncludeItem.CompareExtension(".natvis"))
				    {
					    ItemGroup.NewElementItemInclude("Natvis", IncludeItem.FullPath);
                    }
                    else if (IncludeItem.CompareExtension(".cs"))
                    {
                        ItemGroup.NewElementItemInclude("None", IncludeItem.FullPath);
                    }
                }
		    }

		    ItemGroup = Project.NewElement("ItemGroup");
		    {
                void ProjectReferenceBody(GeneratedVSProject It)
                {
                    ModuleRule CompiledRule = It.CompiledProject.CompiledRule;
                    if (CompiledRule.ModuleType != ModuleType.None && CompiledRule.ModuleType != ModuleType.Vcpkg)
                    {
					    string IntermediateProjectPath = "$(SolutionDir)Intermediate\\ProjectFiles";
					    foreach (var Split in CompiledRule.RelativePath.Split(".", StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries))
					    {
						    IntermediateProjectPath = Path.Combine(IntermediateProjectPath, Split);
					    }

                        IntermediateProjectPath = Path.Combine(IntermediateProjectPath, CompiledRule.Name + ".vcxproj");
					    XmlElement ProjectReference = ItemGroup.NewElementItemInclude("ProjectReference", IntermediateProjectPath);
					    {
						    ProjectReference.NewElement("Project", string.Format("{{{0}}}", It.GeneratedGuid));
					    }
                    }
                }

                foreach (var Item in PublicDependencyModules)
                {
                    ProjectReferenceBody(Item);
                }

                foreach (var Item in PrivateDependencyModules)
                {
                    ProjectReferenceBody(Item);
                }
		    }

		    Project.NewElementImport("$(VCTargetsPath)\\Microsoft.Cpp.targets");
		    Project.NewElementImportGroup("ExtensionTargets");
	    }

	    string ProjectFilePath = Path.Combine(IntermediateProjectPath, Rule.Name);
        ProjectFilePath += ".vcxproj";

        ProjectFile = new FileReference(ProjectFilePath);
        XmlProjectFile = Doc;
    }

    public virtual FileReference ProjectFile { get; private set; }
    private XmlDocument XmlProjectFile;

    private void GenerateFilterXml()
    {
        XmlDocument Doc = new();
        Doc.CreateXmlDeclaration();

        XmlElement Project = Doc.NewElement("Project");
        {
            Project.SetAttribute("ToolsVersion", "4.0");
            Project.SetAttribute("xmlns", @"http://schemas.microsoft.com/developer/msbuild/2003");
            
		    XmlElement ItemGroup = Project.NewElement("ItemGroup");
		    {
                var AbsolutePath = new DirectoryReference(CompiledProject.ProjectDir.FullPath);
                AbsolutePath.CreateIfNotExists(true);

			    HashSet<string> Filters = new();

			    foreach (var IncludeItem in AbsolutePath.GetAllFiles())
			    {
				    XmlElement InnerItem = null;
				    if (IncludeItem.CompareExtension(".cpp") || IncludeItem.CompareExtension(".ixx") || IncludeItem.CompareExtension(".c"))
				    {
					    InnerItem = ItemGroup.NewElementItemInclude("ClCompile", IncludeItem.FullPath);
				    }
				    else if (IncludeItem.CompareExtension(".h") || IncludeItem.CompareExtension(".inl"))
				    {
					    InnerItem = ItemGroup.NewElementItemInclude("ClInclude", IncludeItem.FullPath);
				    }
				    else if (IncludeItem.CompareExtension(".natvis"))
				    {
					    InnerItem = ItemGroup.NewElementItemInclude("Natvis", IncludeItem.FullPath);
                    }
                    else if (IncludeItem.CompareExtension(".cs"))
                    {
                        InnerItem = ItemGroup.NewElementItemInclude("None", IncludeItem.FullPath);
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
						    while ((LastIndex = FilterPath.IndexOf('\\')) != -1)
						    {
							    FilterPath = FilterPath.Substring(0, LastIndex);
							    Filters.Add(FilterPath);
						    }
					    }
				    }
			    }

			    foreach (var Filter in Filters)
			    {
				    XmlElement InnerItem = ItemGroup.NewElementItemInclude("Filter", Filter);
				    InnerItem.NewElement("UniqueIdentifier", $"{{{this.GenerateGuid(Filter)}}}");
			    }
		    }
        }

        string ProjectFilePath = Path.Combine(IntermediateProjectPath, Rule.Name);
        ProjectFilePath += ".vcxproj.filters";

        ProjectFilterFile = new FileReference(ProjectFilePath);
        XmlProjectFilterFile = Doc;
    }

    public FileReference ProjectFilterFile { get; private set; }
    private XmlDocument XmlProjectFilterFile;

    private void GenerateUserXml()
    {
        XmlDocument Doc = new();
        Doc.CreateXmlDeclaration();

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
				    PropertyGroup.NewElement("LocalDebuggerCommand", "$(OutDir)$(SolutionName).exe");
				    switch (Rule.ModuleType)
				    {
                        case ModuleType.GameModule:
					        PropertyGroup.NewElement("LocalDebuggerCommandArguments", "[](--GameDll \"$(TargetName)\")");
					        break;
                        case ModuleType.ConsoleModule:
					        PropertyGroup.NewElement("LocalDebuggerCommandArguments", "[](--ConsoleDll \"$(TargetName)\")");
					        break;
				    }
			    }
		    }
        }

        string ProjectFilePath = Path.Combine(IntermediateProjectPath, Rule.Name);
        ProjectFilePath += ".vcxproj.user";

        ProjectUserFile = new FileReference(ProjectFilePath);
        XmlProjectUserFile = Doc;
    }

    public FileReference ProjectUserFile { get; private set; }
    private XmlDocument XmlProjectUserFile;

    public virtual void SaveAll()
    {
        XmlProjectFile.SaveIfChanged(ProjectFile);
        XmlProjectFilterFile.SaveIfChanged(ProjectFilterFile);
        XmlProjectUserFile.SaveIfChanged(ProjectUserFile);
    }
}