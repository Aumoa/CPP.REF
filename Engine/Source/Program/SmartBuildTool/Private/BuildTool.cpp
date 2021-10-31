// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "BuildTool.h"
#include "Misc/TickCalc.h"
#include "Misc/CommandLine.h"
#include "IO/DirectoryReference.h"

DEFINE_LOG_CATEGORY(LogBuildTool);

SBuildTool::SBuildTool() : Super()
{
}

int32 SBuildTool::GuardedMain(std::vector<std::wstring_view> InArgs)
{
	TickCalc<> Timer;

	SE_LOG(LogBuildTool, Verbose, L"BuildTool Arguments: {}", StringUtils::Join(L", ", InArgs));
	SE_LOG(LogBuildTool, Verbose, L"");

	if (InArgs.size() > 1)
	{
		SCommandLine CommandLine(InArgs);
		if (size_t Idx = CommandLine.GetArgument(L"--Solution"); Idx != -1)
		{
			std::optional Value = CommandLine.GetArgument(Idx + 1);
			SolutionName = Value.value_or(L"Engine");
		}
		else
		{
			SolutionName = L"Engine";
		}
	}

	Timer.DoCalc();
	SE_LOG(LogBuildTool, Verbose, L"Beginning to search thirdparty projects...");
	int32 Count = SearchProjects(L"ThirdParty");
	SE_LOG(LogBuildTool, Verbose, L"Done. {} project(s) found.", Count);

	SE_LOG(LogBuildTool, Verbose, L"Beginning to search engine projects...");
	Count = SearchProjects(L"Engine");
	SE_LOG(LogBuildTool, Verbose, L"Done. {} project(s) found.", Count);

	SE_LOG(LogBuildTool, Verbose, L"Beginning to search game projects...");
	Count = SearchProjects(L"Game");
	SE_LOG(LogBuildTool, Verbose, L"Done. {} project(s) found.", Count);

	SE_LOG(LogBuildTool, Verbose, L"Beginning to search application projects...");
	Count = SearchProjects(L"Application");
	SE_LOG(LogBuildTool, Verbose, L"Done. {} project(s) found.", Count);
	SE_LOG(LogBuildTool, Verbose, L"Search completed. {} seconds elapsed.", Timer.DoCalc().count());

	Timer.DoCalc();
	SE_LOG(LogBuildTool, Verbose, L"");
	SE_LOG(LogBuildTool, Verbose, L"Generate and project runtime data.");
	if (int32 Ret = GenerateProjectsRuntimeData())
	{
		return Ret;
	}
	SE_LOG(LogBuildTool, Verbose, L"Generate completed. {} seconds elapsed.", Timer.DoCalc().count());

	Timer.DoCalc();
	SE_LOG(LogBuildTool, Verbose, L"");
	SE_LOG(LogBuildTool, Verbose, L"Generate project files.");
	if (int32 Ret = GenerateProjectFiles())
	{
		return Ret;
	}
	SE_LOG(LogBuildTool, Verbose, L"Generate completed. {} seconds elapsed.", Timer.DoCalc().count());

	return 0;
}

int32 SBuildTool::SearchProjects(const std::filesystem::path& DirectoryPath)
{
	if (!std::filesystem::exists(DirectoryPath))
	{
		SE_LOG(LogBuildTool, Warning, L"Cannot found target directory: {}. Abort.", DirectoryPath.wstring());
		return 0;
	}

	int32 Count = 0;
	for (auto Item : std::filesystem::recursive_directory_iterator(DirectoryPath))
	{
		if (Item.exists())
		{
			const std::filesystem::path& XmlPath = Item.path();
			if (XmlPath.extension() == L".xml")
			{
				ProjectBuildMetadata Build;
				if (TryParseProject(XmlPath, Build))
				{
					SE_LOG(LogBuildTool, Verbose, L"Parsing succeeded! XmlPath: {}", XmlPath.wstring());
					Build.BaseDirectory = DirectoryPath.stem();
					ProjectsMetadata.emplace_back(Build);
					++Count;
				}
				else
				{
					SE_LOG(LogBuildTool, Warning, L"Parsing error! XmlPath: {}", XmlPath.wstring());
				}
			}
		}
	}
	return Count;
}

#define XmlAttributeRet(Elem, Name) if (Name = Elem->Attribute(#Name); Name == nullptr) { return false; }

bool SBuildTool::TryParseProject(const std::filesystem::path& XmlPath, ProjectBuildMetadata& OutBuild)
{
	using namespace tinyxml2;
	using tinyxml2::XMLDocument;

	XMLDocument Doc;
	XMLError E = Doc.LoadFile(XmlPath.string().c_str());
	if (E != XMLError::XML_SUCCESS)
	{
		return false;
	}

	XMLElement* ProjectInfo = Doc.FirstChildElement("ProjectInfo");
	if (ProjectInfo)
	{
		const char* Name = nullptr;
		const char* Path = nullptr;

		XmlAttributeRet(ProjectInfo, Name);
		XmlAttributeRet(ProjectInfo, Path);

		OutBuild.Name = ANSI_TO_WCHAR(Name);
		OutBuild.Path = ANSI_TO_WCHAR(Path);
		OutBuild.Type = ParseType(ProjectInfo->Attribute("Type"));

		if (XMLElement* IncludePaths = ProjectInfo->FirstChildElement("IncludePaths"))
		{
			for (XMLElement* Item = IncludePaths->FirstChildElement(); Item != nullptr; Item = Item->NextSiblingElement())
			{
				ProjectBuildMetadata::IncludePath IncludePath;

				if (const char* RelativePath = Item->Attribute("RelativePath"); RelativePath)
				{
					IncludePath.RelativePath = ANSI_TO_WCHAR(RelativePath);
					IncludePath.Access = ParseAccessKey(Item->Attribute("Access"));
					OutBuild.IncludePaths.emplace_back(IncludePath);
				}
			}
		}

		if (XMLElement* ReferencedProjects = ProjectInfo->FirstChildElement("ReferencedProjects"))
		{
			for (XMLElement* Item = ReferencedProjects->FirstChildElement(); Item != nullptr; Item = Item->NextSiblingElement())
			{
				ProjectBuildMetadata::ReferencedProject ReferencedProject;

				if (const char* Name = Item->Attribute("Name"); Name)
				{
					ReferencedProject.Name = ANSI_TO_WCHAR(Name);
					ReferencedProject.Access = ParseAccessKey(Item->Attribute("Access"));
					OutBuild.ReferencedProjects.emplace_back(ReferencedProject);
				}
			}
		}

		if (XMLElement* DisableWarnings = ProjectInfo->FirstChildElement("DisableWarnings"))
		{
			for (XMLElement* Item = DisableWarnings->FirstChildElement(); Item != nullptr; Item = Item->NextSiblingElement())
			{
				ProjectBuildMetadata::DisableWarning DisableWarning;

				if (int32 Number = Item->IntAttribute("Number", -1); Number != -1)
				{
					DisableWarning.Number = Number;
					DisableWarning.Access = ParseAccessKey(Item->Attribute("Access"));
					OutBuild.DisableWarnings.emplace_back(DisableWarning);
				}
			}
		}

		if (XMLElement* ExternalLinks = ProjectInfo->FirstChildElement("ExternalLinks"))
		{
			for (XMLElement* Item = ExternalLinks->FirstChildElement(); Item != nullptr; Item = Item->NextSiblingElement())
			{
				ProjectBuildMetadata::ExternalLink ExternalLink;

				if (const char* Name = Item->Attribute("Name"); Name)
				{
					ExternalLink.Name = ANSI_TO_WCHAR(Name);
					ExternalLink.Access = ParseAccessKey(Item->Attribute("Access"));
					OutBuild.ExternalLinks.emplace_back(ExternalLink);
				}
			}
		}

		return true;
	}

	return false;
}

#undef XmlAttributeRet

int32 SBuildTool::GenerateProjectsRuntimeData()
{
	for (auto& Meta : ProjectsMetadata)
	{
		auto [It, Result] = ProjectsRuntime.emplace(Meta.Name, ProjectBuildRuntime());
		if (!Result)
		{
			SE_LOG(LogBuildTool, Error, L"Duplicated project name(\"{}\") detected!", Meta.Name);
			return -1;
		}

		auto& [Key, Value] = *It;
		Value.Metadata = &Meta;
	}

	for (auto& KeyValuePair : ProjectsRuntime)
	{
		auto& [Key, Value] = KeyValuePair;
		BuildRuntime(&Value);
	}

	return 0;
}

void SBuildTool::BuildRuntime(ProjectBuildRuntime* Runtime)
{
	if (!Runtime->bBuild)
	{
		// Build prerequisites.
		for (auto& Reference : Runtime->Metadata->ReferencedProjects)
		{
			ProjectBuildRuntime* ReferencedRuntime = &ProjectsRuntime[Reference.Name];
			if (!ReferencedRuntime->bBuild)
			{
				BuildRuntime(ReferencedRuntime);
			}

			std::set<ProjectBuildRuntime*>* AccessReferences = nullptr;
			std::set<std::wstring>* AccessIncludePaths = nullptr;
			std::set<int32>* AccessDisableWarnings = nullptr;
			std::set<std::wstring>* AccessExternalLinks = nullptr;
			if (Reference.Access == EAccessKey::Public)
			{
				AccessReferences = &Runtime->PublicReferences;
				AccessIncludePaths = &Runtime->PublicIncludePaths;
				AccessDisableWarnings = &Runtime->PublicDisableWarnings;
				AccessExternalLinks = &Runtime->PublicExternalLinks;
			}
			else
			{
				AccessReferences = &Runtime->PrivateReferences;
				AccessIncludePaths = &Runtime->PrivateIncludePaths;
				AccessDisableWarnings = &Runtime->PrivateDisableWarnings;
				AccessExternalLinks = &Runtime->PrivateExternalLinks;
			}

			AccessReferences->insert(ReferencedRuntime->PublicReferences.begin(), ReferencedRuntime->PublicReferences.end());
			AccessReferences->emplace(ReferencedRuntime);
			AccessIncludePaths->insert(ReferencedRuntime->PublicIncludePaths.begin(), ReferencedRuntime->PublicIncludePaths.end());
			AccessDisableWarnings->insert(ReferencedRuntime->PublicDisableWarnings.begin(), ReferencedRuntime->PublicDisableWarnings.end());
			AccessExternalLinks->insert(ReferencedRuntime->PublicExternalLinks.begin(), ReferencedRuntime->PublicExternalLinks.end());
		}

		// ProjectPath
		std::vector<std::wstring> SplitPath = StringUtils::Split(Runtime->Metadata->Path, L".", true, true);
		Runtime->ProjectPath = std::format(L"$(SolutionDir){}\\Source\\{}", Runtime->Metadata->BaseDirectory, StringUtils::Join(L"\\", SplitPath));

		// AdditionalIncludeDirectories
		for (auto& IncludePath : Runtime->Metadata->IncludePaths)
		{
			std::wstring SolutionRelativePath = Runtime->ProjectPath + L"\\" + IncludePath.RelativePath.wstring();

			if (IncludePath.Access == EAccessKey::Public)
			{
				Runtime->PublicIncludePaths.emplace(SolutionRelativePath);
			}
			else
			{
				Runtime->PrivateIncludePaths.emplace(SolutionRelativePath);
			}
		}

		std::vector<std::wstring> AdditionalIncludeDirectories;
		if (Runtime->PublicIncludePaths.size())
		{
			AdditionalIncludeDirectories.emplace_back(StringUtils::Join(L";", Runtime->PublicIncludePaths));
		}
		if (Runtime->PrivateIncludePaths.size())
		{
			AdditionalIncludeDirectories.emplace_back(StringUtils::Join(L";", Runtime->PrivateIncludePaths));
		}
		AdditionalIncludeDirectories.emplace_back(L"%(AdditionalIncludeDirectories)");
		Runtime->AdditionalIncludeDirectories = StringUtils::Join(L";", AdditionalIncludeDirectories);

		// PreprocessorDefinitions
		std::vector<std::wstring> PreprocessorDefinitions;
		for (auto& Reference : Runtime->PublicReferences)
		{
			std::wstring NameUpper = Reference->Metadata->Name;
			std::transform(NameUpper.begin(), NameUpper.end(), NameUpper.begin(), (int(*)(int))std::toupper);
			PreprocessorDefinitions.emplace_back(NameUpper + L"_API=__declspec(dllimport)");
		}
		for (auto& Reference : Runtime->PrivateReferences)
		{
			std::wstring NameUpper = Reference->Metadata->Name;
			std::transform(NameUpper.begin(), NameUpper.end(), NameUpper.begin(), (int(*)(int))std::toupper);
			PreprocessorDefinitions.emplace_back(NameUpper + L"_API=__declspec(dllimport)");
		}
		{
			std::wstring NameUpper = Runtime->Metadata->Name;
			std::transform(NameUpper.begin(), NameUpper.end(), NameUpper.begin(), (int(*)(int))std::toupper);
			PreprocessorDefinitions.emplace_back(NameUpper + L"_API=__declspec(dllexport)");
		}
		PreprocessorDefinitions.emplace_back(L"$(PreprocessorDefinitions)");
		Runtime->PreprocessorDefinitions = StringUtils::Join(L";", PreprocessorDefinitions);

		// DisableSpecificWarnings
		for (auto& DisableWarning : Runtime->Metadata->DisableWarnings)
		{
			if (DisableWarning.Access == EAccessKey::Public)
			{
				Runtime->PublicDisableWarnings.emplace(DisableWarning.Number);
			}
			else
			{
				Runtime->PrivateDisableWarnings.emplace(DisableWarning.Number);
			}
		}

		std::vector<std::wstring> DisableSpecificWarnings;
		DisableSpecificWarnings.reserve(Runtime->PublicDisableWarnings.size() + Runtime->PrivateDisableWarnings.size());
		for (auto& DisableWarning : Runtime->PublicDisableWarnings)
		{
			DisableSpecificWarnings.emplace_back(std::to_wstring(DisableWarning));
		}
		for (auto& DisableWarning : Runtime->PrivateDisableWarnings)
		{
			DisableSpecificWarnings.emplace_back(std::to_wstring(DisableWarning));
		}
		Runtime->DisableSpecificWarnings = StringUtils::Join(L";", DisableSpecificWarnings);

		// AdditionalDependencies
		for (auto& ExternalLink : Runtime->Metadata->ExternalLinks)
		{
			if (ExternalLink.Access == EAccessKey::Public)
			{
				Runtime->PublicExternalLinks.emplace(ExternalLink.Name);
			}
			else
			{
				Runtime->PrivateExternalLinks.emplace(ExternalLink.Name);
			}
		}

		std::vector<std::wstring> AdditionalDependencies;
		if (Runtime->PublicExternalLinks.size())
		{
			AdditionalDependencies.emplace_back(StringUtils::Join(L";", Runtime->PublicExternalLinks));
		}
		if (Runtime->PrivateExternalLinks.size())
		{
			AdditionalDependencies.emplace_back(StringUtils::Join(L";", Runtime->PrivateExternalLinks));
		}
		AdditionalDependencies.emplace_back(L"$(AdditionalDependencies)");
		Runtime->AdditionalDependencies = StringUtils::Join(L";", AdditionalDependencies);

		Runtime->bBuild = true;
	}
}

int32 SBuildTool::GenerateProjectFiles()
{
	for (auto& [Key, Value] : ProjectsRuntime)
	{
		GenerateProjectFile(&Value);
	}

	return 0;
}

#define BoolStr(x) ((x) ? "true" : "false")

int32 SBuildTool::GenerateProjectFile(ProjectBuildRuntime* Runtime)
{
	check(Runtime->bBuild);

	using namespace tinyxml2;
	using tinyxml2::XMLDocument;

	if (!Runtime->bXmlFileGenerated)
	{
		for (auto Reference : Runtime->PrivateReferences)
		{
			if (!Reference->bXmlFileGenerated)
			{
				GenerateProjectFile(Reference);
			}
		}

		for (auto Reference : Runtime->PublicReferences)
		{
			if (!Reference->bXmlFileGenerated)
			{
				GenerateProjectFile(Reference);
			}
		}

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
			uint8 bUseDebugLibrary : 1;
			uint8 bWholeProgramOptimization : 1;
			uint8 bLinkIncremental : 1;
			uint8 bFunctionLevelLinking : 1;
			uint8 bIntrinsicFunctions : 1;
			uint8 bEnableCOMDATFolding : 1;
			uint8 bOptimizeReferences : 1;

			// AutoGenerate
			std::string Condition;
		};

		BuildConfiguration Configurations[] =
		{
			{
				.Name = "Debug",
				.Configuration = "Debug",
				.bUseDebugLibrary = true,
				.bWholeProgramOptimization = false,
				.bLinkIncremental = true,
				.bFunctionLevelLinking = true,
				.bIntrinsicFunctions = false,
				.bEnableCOMDATFolding = false,
				.bOptimizeReferences = false,
			},
			{
				.Name = "Release",
				.Configuration = "Release",
				.bUseDebugLibrary = false,
				.bWholeProgramOptimization = true,
				.bLinkIncremental = false,
				.bFunctionLevelLinking = true,
				.bIntrinsicFunctions = true,
				.bEnableCOMDATFolding = true,
				.bOptimizeReferences = true,
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
				UUID Guid;
				if (UuidCreate(&Guid) != RPC_S_OK)
				{
					SE_LOG(LogBuildTool, Error, L"Could not create project GUID.");
					return -1;
				}

				RPC_CSTR GuidStr;
				if (UuidToStringA(&Guid, &GuidStr) != RPC_S_OK)
				{
					SE_LOG(LogBuildTool, Error, L"Unexpected error: Cannot convert UUID to string.");
					return -1;
				}

				Runtime->XmlFile.Guid = (const char*)GuidStr;

				NewElement(PropertyGroup, "VCProjectVersion", "16.0");
				NewElement(PropertyGroup, "Keyword", "Win32Proj");
				NewElement(PropertyGroup, "ProjectGuid", std::format("{{{}}}", (const char*)GuidStr));
				NewElement(PropertyGroup, "RootNamespace", WCHAR_TO_ANSI(Runtime->Metadata->Name));
				NewElement(PropertyGroup, "WindowsTargetPlatformVersion", "10.0");

				RpcStringFreeA(&GuidStr);
			}

			NewElementImport(Project, "$(VCTargetsPath)\\Microsoft.Cpp.Default.props");

			for (auto& Config : Configurations)
			{
				XMLElement* PropertyGroup = NewElementPropertyGroup(Project, Config.Condition, "Configuration");
				{
					NewElement(PropertyGroup, "ConfigurationType", GetTypeString(Runtime->Metadata->Type));
					NewElement(PropertyGroup, "UseDebugLibraries", BoolStr(Config.bUseDebugLibrary));
					NewElement(PropertyGroup, "PlatformToolset", "v142");
					NewElement(PropertyGroup, "WholeProgramOptimization", BoolStr(Config.bWholeProgramOptimization));
					NewElement(PropertyGroup, "CharacterSet", "Unicode");
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
					NewElement(PropertyGroup, "LinkIncremental", BoolStr(Config.bLinkIncremental));
					NewElement(PropertyGroup, "OutDir", "$(SolutionDir)Build\\$(Configuration)\\");
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
						NewElement(ClCompile, "IntrinsicFunctions", BoolStr(Config.bIntrinsicFunctions));
						NewElement(ClCompile, "SDLCheck", "true");
						NewElement(ClCompile, "ConformanceMode", "true");
						NewElement(ClCompile, "LanguageStandard", "stdcpplatest");
						NewElement(ClCompile, "MultiProcessorCompilation", "true");
						NewElement(ClCompile, "AdditionalIncludeDirectories", WCHAR_TO_ANSI(Runtime->AdditionalIncludeDirectories));
						NewElement(ClCompile, "PreprocessorDefinitions", WCHAR_TO_ANSI(Runtime->PreprocessorDefinitions));
						NewElement(ClCompile, "UseStandardPreprocessor", "true");
						NewElement(ClCompile, "DisableSpecificWarnings", WCHAR_TO_ANSI(Runtime->DisableSpecificWarnings));
					}

					XMLElement* Link = NewElement(ItemDefinitionGroup, "Link");
					{
						NewElement(Link, "SubSystem", GetSubSystemString(Runtime->Metadata->Type));
						NewElement(Link, "EnableCOMDATFolding", BoolStr(Config.bEnableCOMDATFolding));
						NewElement(Link, "OptimizeReferences", BoolStr(Config.bOptimizeReferences));
						NewElement(Link, "GenerateDebugInformation", "true");
						NewElement(Link, "EnableUAC", "false");
						NewElement(Link, "AdditionalDependencies", WCHAR_TO_ANSI(Runtime->AdditionalDependencies));
					}
				}
			}

			ItemGroup = NewElement(Project, "ItemGroup");
			{
				std::wstring RelativePath = Runtime->ProjectPath;
				if (size_t IndexOf = RelativePath.find(L"$(SolutionDir)"); IndexOf != std::wstring::npos)
				{
					auto It = RelativePath.begin() + IndexOf;
					size_t Count = std::size(L"$(SolutionDir)") - 1;
					RelativePath = RelativePath.replace(It, It + Count, L".\\");
				}

				auto AbsolutePath = std::filesystem::absolute(RelativePath);
				SDirectoryReference(AbsolutePath).CreateIfNotExists(true);

				for (auto IncludeItem : std::filesystem::recursive_directory_iterator(AbsolutePath))
				{
					if (IncludeItem.path().extension() == ".cpp")
					{
						NewElementItemInclude(ItemGroup, "ClCompile", IncludeItem.path().string());
					}
					else if (IncludeItem.path().extension() == ".h")
					{
						NewElementItemInclude(ItemGroup, "ClInclude", IncludeItem.path().string());
					}
					else if (IncludeItem.path().extension() == ".inl")
					{
						NewElementItemInclude(ItemGroup, "ClInclude", IncludeItem.path().string());
					}
					else if (IncludeItem.path().extension() == ".natvis")
					{
						NewElementItemInclude(ItemGroup, "Natvis", IncludeItem.path().string());
					}
				}
			}

			NewElementImport(Project, "$(VCTargetsPath)\\Microsoft.Cpp.targets");
			NewElementImportGroup(Project, "ExtensionTargets");
		}

		std::filesystem::path IntermediateProjectPath = "Intermediate/ProjectFiles";
		for (auto Split : StringUtils::Split(Runtime->Metadata->Path.c_str(), L".", true, true))
		{
			IntermediateProjectPath /= Split;
		}

		// Create directory.
		SDirectoryReference(IntermediateProjectPath).CreateIfNotExists(true);

		IntermediateProjectPath /= Runtime->Metadata->Name;
		XMLError Err = Doc.SaveFile((IntermediateProjectPath.string() + ".vcxproj").c_str());
		if (Err != XMLError::XML_SUCCESS)
		{
			SE_LOG(LogBuildTool, Error, L"Failed to save project file.");
			return -1;
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
				std::wstring ProjectRelativePath = Runtime->ProjectPath;
				if (size_t IndexOf = ProjectRelativePath.find(L"$(SolutionDir)"); IndexOf != std::wstring::npos)
				{
					auto It = ProjectRelativePath.begin() + IndexOf;
					size_t Count = std::size(L"$(SolutionDir)") - 1;
					ProjectRelativePath = ProjectRelativePath.replace(It, It + Count, L".\\");
				}

				auto AbsolutePath = std::filesystem::absolute(ProjectRelativePath).wstring();
				SDirectoryReference(AbsolutePath).CreateIfNotExists(true);

				std::vector<std::wstring> Filters;

				for (auto IncludeItem : std::filesystem::recursive_directory_iterator(AbsolutePath))
				{
					XMLElement* InnerItem = nullptr;
					if (IncludeItem.path().extension() == ".cpp")
					{
						InnerItem = NewElementItemInclude(ItemGroup, "ClCompile", IncludeItem.path().string());
					}
					else if (IncludeItem.path().extension() == ".h")
					{
						InnerItem = NewElementItemInclude(ItemGroup, "ClInclude", IncludeItem.path().string());
					}
					else if (IncludeItem.path().extension() == ".inl")
					{
						InnerItem = NewElementItemInclude(ItemGroup, "ClInclude", IncludeItem.path().string());
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
						NewElement(InnerItem, "Filter", WCHAR_TO_ANSI(FilterPath));
						Filters.emplace_back(FilterPath);
					}
				}

				for (auto& Filter : Filters)
				{
					UUID Guid;
					if (UuidCreate(&Guid) != RPC_S_OK)
					{
						SE_LOG(LogBuildTool, Error, L"Could not create project GUID.");
						return -1;
					}

					RPC_CSTR GuidStr;
					if (UuidToStringA(&Guid, &GuidStr) != RPC_S_OK)
					{
						SE_LOG(LogBuildTool, Error, L"Unexpected error: Cannot convert UUID to string.");
						return -1;
					}

					XMLElement* InnerItem = NewElementItemInclude(ItemGroup, "Filter", WCHAR_TO_ANSI(Filter));
					NewElement(InnerItem, "UniqueIdentifier", std::format("{{{}}}", (const char*)GuidStr));

					RpcStringFreeA(&GuidStr);
				}
			}
		}

		Err = Doc.SaveFile((IntermediateProjectPath.string() + ".vcxproj.filters").c_str());
		if (Err != XMLError::XML_SUCCESS)
		{
			SE_LOG(LogBuildTool, Error, L"Failed to save project file.");
			return -1;
		}

		Runtime->XmlFile.XmlPath = IntermediateProjectPath;
		Runtime->bXmlFileGenerated = true;
	}

	return 0;
}

#undef BoolStr

EType SBuildTool::ParseType(const char* StringToken)
{
	if (StringToken)
	{
		std::string_view Token = StringToken;
		if (Token == "Console")
		{
			return EType::Console;
		}
		else if (Token == "Application")
		{
			return EType::Application;
		}
	}

	return EType::Module;
}

EAccessKey SBuildTool::ParseAccessKey(const char* StringToken)
{
	return StringToken && std::string_view(StringToken) == "Public" ? EAccessKey::Public : EAccessKey::Private;
}

std::string SBuildTool::GetTypeString(EType Type)
{
	switch (Type)
	{
	case EType::Console:
	case EType::Application:
		return "Application";
	default:
		return "DynamicLibrary";
	}
}

std::string SBuildTool::GetSubSystemString(EType Type)
{
	switch (Type)
	{
	case EType::Console:
		return "Console";
	default:
		return "Windows";
	}
}