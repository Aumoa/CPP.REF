// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "VSProject.h"
#include "tinyxml2.h"
#include "LogVSProjects.h"
#include "GuidHelper.h"
#include "IO/DirectoryReference.h"
#include "IO/FileReference.h"

GENERATE_BODY(SVSProject);

std::string SVSProject::GetTypeString(ProjectBuildMetadata::EType Type)
{
	switch (Type)
	{
	case ProjectBuildMetadata::EType::ConsoleApplication:
	case ProjectBuildMetadata::EType::Application:
		return "Application";
	default:
		return "DynamicLibrary";
	}
}

template<class T>
std::string_view SVSProject::BoolStr(T Expr)
{
	return ((bool)(Expr) ? "true" : "false");
}

std::string SVSProject::GetSubSystemString(ProjectBuildMetadata::EType Type)
{
	switch (Type)
	{
	case ProjectBuildMetadata::EType::ConsoleApplication:
		return "Console";
	default:
		return "Windows";
	}
}

SVSProject::SVSProject(IProjectGenerator* Generator, const ProjectBuildRuntime& RuntimeData)
{
	using namespace tinyxml2;
	using EType = ProjectBuildMetadata::EType;

	XMLDocument Doc;

	auto NewElement = [&](XMLElement* Parent, std::string_view Name, std::string_view Text = "")
	{
		XMLElement* Elem = Doc.NewElement(Name.data());
		Parent->LinkEndChild(Elem);
		if (!Text.empty())
		{
			Elem->SetText(Text.data());
		}

		return Elem;
	};

	auto NewProjectConfiguration = [&](XMLElement* Parent, std::string_view Include, std::string_view InConfiguration, std::string_view InPlatform)
	{
		XMLElement* ProjectConfiguration = NewElement(Parent, "ProjectConfiguration");
		{
			ProjectConfiguration->SetAttribute("Include", Include.data());
			NewElement(ProjectConfiguration, "Configuration", InConfiguration.data());
			NewElement(ProjectConfiguration, "Platform", InPlatform.data());
		}
		return ProjectConfiguration;
	};

	auto NewElementSingleAttr = [&](XMLElement* Parent, std::string_view Name, std::string_view AttrName, std::string_view AttrValue)
	{
		XMLElement* Elem = NewElement(Parent, Name);
		Elem->SetAttribute(AttrName.data(), AttrValue.data());
		return Elem;
	};

	auto NewElementPropertyGroup = [&](XMLElement* Parent, std::string_view Condition, std::string_view Label = "")
	{
		XMLElement* Elem = NewElement(Parent, "PropertyGroup");
		if (!Condition.empty())
		{
			Elem->SetAttribute("Condition", Condition.data());
		}
		if (!Label.empty())
		{
			Elem->SetAttribute("Label", Label.data());
		}
		return Elem;
	};

	auto NewElementImport = [&](XMLElement* Parent, std::string_view Project, std::string_view Condition = "", std::string_view Label = "")
	{
		XMLElement* Elem = NewElement(Parent, "Import");
		Elem->SetAttribute("Project", Project.data());
		if (!Condition.empty())
		{
			Elem->SetAttribute("Condition", Condition.data());
		}
		if (!Label.empty())
		{
			Elem->SetAttribute("Label", Label.data());
		}
		return Elem;
	};

	auto NewElementImportGroup = [&](XMLElement* Parent, std::string_view Label, std::string_view Condition = "")
	{
		XMLElement* Elem = NewElement(Parent, "ImportGroup");
		Elem->SetAttribute("Label", Label.data());
		if (!Condition.empty())
		{
			Elem->SetAttribute("Condition", Condition.data());
		}
		return Elem;
	};

	auto NewElementItemDefinitionGroup = [&](XMLElement* Parent, std::string_view Condition)
	{
		XMLElement* Elem = NewElement(Parent, "ItemDefinitionGroup");
		if (!Condition.empty())
		{
			Elem->SetAttribute("Condition", Condition.data());
		}
		return Elem;
	};

	auto NewElementItemInclude = [&](XMLElement* Parent, std::string_view Type, std::string_view Path)
	{
		XMLElement* Elem = NewElement(Parent, Type);
		Elem->SetAttribute("Include", Path.data());
		return Elem;
	};

	struct BuildConfiguration
	{
		std::string Name;
		std::string Configuration;
		std::string Optimization[2];
		std::string BasicRuntimeChecks[2];
		std::string Macros;
		uint8 bUseDebugLibrary : 1;
		uint8 bWholeProgramOptimization : 2;
		uint8 bLinkIncremental : 2;
		uint8 bFunctionLevelLinking : 1;
		uint8 bIntrinsicFunctions : 2;
		uint8 bEnableCOMDATFolding : 2;
		uint8 bOptimizeReferences : 2;

		// AutoGenerate
		std::string Condition;
	};

	// Engine/Game
	auto DoubleBit = [](uint8 bDoubleBit, bool bFlag)
	{
		if (bFlag)
		{
			return (bDoubleBit & 0b01) != 0;
		}
		else
		{
			return (bDoubleBit & 0x10) != 0;
		}
	};

	BuildConfiguration Configurations[] =
	{
		{
			.Name = "Debug",
			.Configuration = "Debug",
			.Optimization = { "Disabled", "Disabled" },
			.BasicRuntimeChecks = { "EnableFastChecks", "EnableFastChecks" },
			.Macros = "DO_CHECK=1;",
			.bUseDebugLibrary = true,
			.bWholeProgramOptimization = 0b00,
			.bLinkIncremental = 0b11,
			.bFunctionLevelLinking = true,
			.bIntrinsicFunctions = 0b00,
			.bEnableCOMDATFolding = 0b00,
			.bOptimizeReferences = 0b00,
		},
		{
			.Name = "DebugGame",
			.Configuration = "DebugGame",
			.Optimization = { "Disabled", "MaxSpeed" },
			.BasicRuntimeChecks = { "EnableFastChecks", "Default" },
			.Macros = "DO_CHECK=1;",
			.bUseDebugLibrary = false,
			.bWholeProgramOptimization = 0b01,
			.bLinkIncremental = 0b10,
			.bFunctionLevelLinking = true,
			.bIntrinsicFunctions = 0b01,
			.bEnableCOMDATFolding = 0b01,
			.bOptimizeReferences = 0b01,
		},
		{
			.Name = "Release",
			.Configuration = "Release",
			.Optimization = { "MaxSpeed", "MaxSpeed" },
			.BasicRuntimeChecks = { "Default", "Default" },
			.Macros = "DO_CHECK=0;",
			.bUseDebugLibrary = false,
			.bWholeProgramOptimization = 0b11,
			.bLinkIncremental = 0b00,
			.bFunctionLevelLinking = true,
			.bIntrinsicFunctions = 0b11,
			.bEnableCOMDATFolding = 0b11,
			.bOptimizeReferences = 0b11,
		}
	};

	for (auto& Config : Configurations)
	{
		Config.Condition = std::format("\'$(Configuration)|$(Platform)\'==\'{}|x64\'", Config.Name);
	}

	// Make declaration.
	XMLDeclaration* Decl = Doc.NewDeclaration();
	Doc.LinkEndChild(Decl);

	// Begin <Project>
	XMLElement* Project = Doc.NewElement("Project");
	Doc.LinkEndChild(Project);
	{
		Project->SetAttribute("DefaultTargets", "Build");
		Project->SetAttribute("xmlns", "http://schemas.microsoft.com/developer/msbuild/2003");

		XMLElement* ItemGroup = NewElementSingleAttr(Project, "ItemGroup", "Label", "ProjectConfigurations");
		{
			for (auto& Config : Configurations)
			{
				NewProjectConfiguration(ItemGroup, Config.Name + "|x64", Config.Configuration, "x64");
			}
		}

		XMLElement* PropertyGroup = NewElementPropertyGroup(Project, "", "Globals");
		{
			NewElement(PropertyGroup, "VCProjectVersion", "16.0");
			NewElement(PropertyGroup, "Keyword", "Win32Proj");
			NewElement(PropertyGroup, "ProjectGuid", std::format("{{{}}}", WCHAR_TO_ANSI(RuntimeData.ProjectGuid.ToString())));
			NewElement(PropertyGroup, "RootNamespace", WCHAR_TO_ANSI(RuntimeData.Metadata->Name));
			NewElement(PropertyGroup, "WindowsTargetPlatformVersion", "10.0");
		}

		NewElementImport(Project, "$(VCTargetsPath)\\Microsoft.Cpp.Default.props");

		for (auto& Config : Configurations)
		{
			XMLElement* PropertyGroup = NewElementPropertyGroup(Project, Config.Condition, "Configuration");
			{
				NewElement(PropertyGroup, "ConfigurationType", GetTypeString(RuntimeData.Metadata->Type));
				NewElement(PropertyGroup, "UseDebugLibraries", BoolStr(Config.bUseDebugLibrary));
				NewElement(PropertyGroup, "PlatformToolset", "v142");
				NewElement(PropertyGroup, "WholeProgramOptimization", BoolStr(DoubleBit(Config.bWholeProgramOptimization, RuntimeData.Metadata->bEngine)));
				NewElement(PropertyGroup, "CharacterSet", "Unicode");
				NewElement(PropertyGroup, "EnableUnitySupport", "true");
			}
		}

		NewElementImport(Project, "$(VCTargetsPath)\\Microsoft.Cpp.props");
		NewElementImportGroup(Project, "ExtensionSettings");
		NewElementImportGroup(Project, "Shared");

		for (auto& Config : Configurations)
		{
			XMLElement* ImportGroup = NewElementImportGroup(Project, "PropertySheets", Config.Condition);
			{
				NewElementImport(ImportGroup, "$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props", "exists(\'$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\')", "LocalAppDataPlatform");
			}
		}

		NewElementPropertyGroup(Project, "", "UserMacros");
		for (auto& Config : Configurations)
		{
			XMLElement* PropertyGroup = NewElementPropertyGroup(Project, Config.Condition);
			{
				NewElement(PropertyGroup, "LinkIncremental", BoolStr(DoubleBit(Config.bLinkIncremental, RuntimeData.Metadata->bEngine)));
				NewElement(PropertyGroup, "OutDir", "$(SolutionDir)Build\\");
				NewElement(PropertyGroup, "IntDir", "$(SolutionDir)Intermediate\\$(Configuration)\\$(ProjectName)\\");
			}
		}

		for (auto& Config : Configurations)
		{
			XMLElement* ItemDefinitionGroup = NewElementItemDefinitionGroup(Project, Config.Condition);
			{
				XMLElement* ClCompile = NewElement(ItemDefinitionGroup, "ClCompile");
				{
					NewElement(ClCompile, "WarningLevel", "Level3");
					NewElement(ClCompile, "FunctionLevelLinking", BoolStr(Config.bFunctionLevelLinking));
					NewElement(ClCompile, "IntrinsicFunctions", BoolStr(DoubleBit(Config.bIntrinsicFunctions, RuntimeData.Metadata->bEngine)));
					NewElement(ClCompile, "SDLCheck", "true");
					NewElement(ClCompile, "ConformanceMode", "true");
					NewElement(ClCompile, "LanguageStandard", "stdcpplatest");
					NewElement(ClCompile, "MultiProcessorCompilation", "true");
					NewElement(ClCompile, "AdditionalIncludeDirectories", WCHAR_TO_ANSI(RuntimeData.AdditionalIncludeDirectories));
					NewElement(ClCompile, "PreprocessorDefinitions", Config.Macros + WCHAR_TO_ANSI(RuntimeData.PreprocessorDefinitions));
					NewElement(ClCompile, "UseStandardPreprocessor", "true");
					NewElement(ClCompile, "DisableSpecificWarnings", WCHAR_TO_ANSI(RuntimeData.DisableSpecificWarnings));
					NewElement(ClCompile, "Optimization", Config.Optimization[RuntimeData.Metadata->bEngine]);
					NewElement(ClCompile, "BasicRuntimeChecks", Config.BasicRuntimeChecks[RuntimeData.Metadata->bEngine]);
				}

				XMLElement* Link = NewElement(ItemDefinitionGroup, "Link");
				{
					NewElement(Link, "SubSystem", GetSubSystemString(RuntimeData.Metadata->Type));
					NewElement(Link, "EnableCOMDATFolding", BoolStr(DoubleBit(Config.bEnableCOMDATFolding, RuntimeData.Metadata->bEngine)));
					NewElement(Link, "OptimizeReferences", BoolStr(DoubleBit(Config.bOptimizeReferences, RuntimeData.Metadata->bEngine)));
					NewElement(Link, "GenerateDebugInformation", "true");
					NewElement(Link, "EnableUAC", "false");
					NewElement(Link, "AdditionalDependencies", WCHAR_TO_ANSI(RuntimeData.AdditionalDependencies));
				}
			}
		}

		ItemGroup = NewElement(Project, "ItemGroup");
		{
			std::wstring RelativePath = RuntimeData.ProjectPath;
			if (size_t IndexOf = RelativePath.find(L"$(SolutionDir)"); IndexOf != std::wstring::npos)
			{
				auto It = RelativePath.begin() + IndexOf;
				size_t Count = std::size(L"$(SolutionDir)") - 1;
				RelativePath = RelativePath.replace(It, It + Count, L".\\");
			}

			auto AbsolutePath = std::filesystem::absolute(RelativePath);
			NewObject<SDirectoryReference>(AbsolutePath)->CreateIfNotExists(true);

			for (auto IncludeItem : std::filesystem::recursive_directory_iterator(AbsolutePath))
			{
				if (IncludeItem.path().extension() == ".cpp" || IncludeItem.path().extension() == ".ixx")
				{
					NewElementItemInclude(ItemGroup, "ClCompile", IncludeItem.path().string());
				}
				else if (IncludeItem.path().extension() == ".h" || IncludeItem.path().extension() == ".inl")
				{
					NewElementItemInclude(ItemGroup, "ClInclude", IncludeItem.path().string());
				}
				else if (IncludeItem.path().extension() == ".xml")
				{
					NewElementItemInclude(ItemGroup, "Xml", IncludeItem.path().string());
				}
				else if (IncludeItem.path().extension() == ".natvis")
				{
					NewElementItemInclude(ItemGroup, "Natvis", IncludeItem.path().string());
				}
			}
		}

		ItemGroup = NewElement(Project, "ItemGroup");
		{
			auto ProjectReferenceBody = [&](auto It)
			{
				auto& [Guid, Referenced] = It;
				if (Referenced->Metadata->Type != EType::None)
				{
					std::filesystem::path IntermediateProjectPath = "$(SolutionDir)Intermediate\\ProjectFiles";
					for (auto Split : StringUtils::Split(Referenced->Metadata->Path, L".", true, true))
					{
						IntermediateProjectPath /= Split;
					}

					IntermediateProjectPath /= Referenced->Metadata->Name + L".vcxproj";
					XMLElement* ProjectReference = NewElementItemInclude(ItemGroup, "ProjectReference", WCHAR_TO_ANSI(IntermediateProjectPath.wstring()));
					{
						NewElement(ProjectReference, "Project", std::format("{{{}}}", WCHAR_TO_ANSI(Referenced->ProjectGuid.ToString())));
					}
				}
			};

			std::for_each(RuntimeData.PublicReferences.begin(), RuntimeData.PublicReferences.end(), ProjectReferenceBody);
			std::for_each(RuntimeData.PrivateReferences.begin(), RuntimeData.PrivateReferences.end(), ProjectReferenceBody);
		}

		NewElementImport(Project, "$(VCTargetsPath)\\Microsoft.Cpp.targets");
		NewElementImportGroup(Project, "ExtensionTargets");
	}

	std::filesystem::path IntermediateProjectPath = "Intermediate/ProjectFiles";
	for (auto Split : StringUtils::Split(RuntimeData.Metadata->Path, L".", true, true))
	{
		IntermediateProjectPath /= Split;
	}

	// Create directory.
	NewObject<SDirectoryReference>(IntermediateProjectPath)->CreateIfNotExists(true);

	IntermediateProjectPath /= RuntimeData.Metadata->Name;
	XmlPath = IntermediateProjectPath;
	XmlPath.replace_extension(L".vcxproj");

	XMLError Err = SaveAs(&Doc, XmlPath);
	if (Err != XML_SUCCESS)
	{
		FatalAndDisplay(L"Failed to save project file.");
		return;
	}

	// Make filters XML.
	Doc.Clear();

	Decl = Doc.NewDeclaration();
	Doc.LinkEndChild(Decl);

	Project = Doc.NewElement("Project");
	Doc.LinkEndChild(Project);
	{
		Project->SetAttribute("ToolsVersion", "4.0");
		Project->SetAttribute("xmlns", "http://schemas.microsoft.com/developer/msbuild/2003");

		XMLElement* ItemGroup = NewElement(Project, "ItemGroup");
		{
			std::wstring ProjectRelativePath = RuntimeData.ProjectPath;
			if (size_t IndexOf = ProjectRelativePath.find(L"$(SolutionDir)"); IndexOf != std::wstring::npos)
			{
				auto It = ProjectRelativePath.begin() + IndexOf;
				size_t Count = std::size(L"$(SolutionDir)") - 1;
				ProjectRelativePath = ProjectRelativePath.replace(It, It + Count, L".\\");
			}

			auto AbsolutePath = std::filesystem::absolute(ProjectRelativePath).wstring();
			NewObject<SDirectoryReference>(AbsolutePath)->CreateIfNotExists(true);

			std::set<std::wstring> Filters;

			for (auto IncludeItem : std::filesystem::recursive_directory_iterator(AbsolutePath))
			{
				XMLElement* InnerItem = nullptr;
				if (IncludeItem.path().extension() == ".cpp" || IncludeItem.path().extension() == ".ixx")
				{
					InnerItem = NewElementItemInclude(ItemGroup, "ClCompile", IncludeItem.path().string());
				}
				else if (IncludeItem.path().extension() == ".h" || IncludeItem.path().extension() == ".inl")
				{
					InnerItem = NewElementItemInclude(ItemGroup, "ClInclude", IncludeItem.path().string());
				}
				else if (IncludeItem.path().extension() == ".xml")
				{
					InnerItem = NewElementItemInclude(ItemGroup, "Xml", IncludeItem.path().string());
				}
				else if (IncludeItem.path().extension() == ".natvis")
				{
					InnerItem = NewElementItemInclude(ItemGroup, "Natvis", IncludeItem.path().string());
				}

				if (InnerItem)
				{
					std::wstring IncludeItemPath = IncludeItem.path().wstring();
					size_t IndexOfFilterStart = IncludeItemPath.find(AbsolutePath) + AbsolutePath.length() + 1;

					std::wstring FilterPath = std::filesystem::path(IncludeItemPath.substr(IndexOfFilterStart)).parent_path().wstring();
					if (!FilterPath.empty())
					{
						NewElement(InnerItem, "Filter", WCHAR_TO_ANSI(FilterPath));
						Filters.emplace(FilterPath);

						size_t LastIndex;
						while ((LastIndex = FilterPath.find_last_of(L'\\')) != std::wstring::npos)
						{
							FilterPath = FilterPath.substr(0, LastIndex);
							Filters.emplace(FilterPath);
						}
					}
				}
			}

			for (auto& Filter : Filters)
			{
				XMLElement* InnerItem = NewElementItemInclude(ItemGroup, "Filter", WCHAR_TO_ANSI(Filter));
				NewElement(InnerItem, "UniqueIdentifier", std::format("{{{}}}", WCHAR_TO_ANSI(GuidHelper::GenerateGuid(Filter).ToString())));
			}
		}
	}

	Err = SaveAs(&Doc, IntermediateProjectPath.string() + ".vcxproj.filters");
	if (Err != XMLError::XML_SUCCESS)
	{
		FatalAndDisplay(L"Failed to save project file.");
	}

	// Make debugger settings.
	Doc.Clear();

	Decl = Doc.NewDeclaration();
	Doc.LinkEndChild(Decl);

	Project = Doc.NewElement("Project");
	Doc.LinkEndChild(Project);
	{
		Project->SetAttribute("ToolsVersion", "Current");
		Project->SetAttribute("xmlns", "http://schemas.microsoft.com/developer/msbuild/2003");
			
		for (auto& Config : Configurations)
		{
			XMLElement* PropertyGroup = NewElementPropertyGroup(Project, Config.Condition);
			{
				NewElement(PropertyGroup, "LocalDebuggerWorkingDirectory", "$(SolutionDir)");
				NewElement(PropertyGroup, "DebuggerFlavor", "WindowsLocalDebugger");
				switch (RuntimeData.Metadata->Type)
				{
				case EType::Game:
					NewElement(PropertyGroup, "LocalDebuggerCommand", "$(OutDir)Windows.exe");
					NewElement(PropertyGroup, "LocalDebuggerCommandArguments", std::format("--GameDll \"{}\"", WCHAR_TO_ANSI(RuntimeData.Metadata->Name)));
					break;
				case EType::Console:
					NewElement(PropertyGroup, "LocalDebuggerCommand", "$(OutDir)WindowsConsole.exe");
					NewElement(PropertyGroup, "LocalDebuggerCommandArguments", std::format("--ConsoleDll \"{}\"", WCHAR_TO_ANSI(RuntimeData.Metadata->Name)));
					break;
				}
			}
		}
	}

	Err = SaveAs(&Doc, IntermediateProjectPath.string() + ".vcxproj.user");
	if (Err != XMLError::XML_SUCCESS)
	{
		FatalAndDisplay(L"Failed to save project file.");
	}
}

std::filesystem::path SVSProject::GetPath()
{
	return XmlPath;
}

tinyxml2::XMLError SVSProject::SaveAs(tinyxml2::XMLDocument* Doc, const std::filesystem::path& Path)
{
	using namespace tinyxml2;

	XMLPrinter Print;
	Doc->Accept(&Print);

	std::wstring Previous = StringUtils::Trim(NewObject<SFileReference>(Path)->ReadAllText());
	std::wstring Build = StringUtils::Trim(ANSI_TO_WCHAR(Print.CStr()));
	if (Previous == Build)
	{
		return XML_SUCCESS;
	}

	return Doc->SaveFile(Path.string().c_str());
}