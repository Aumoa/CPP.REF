// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Solution.h"
#include "LogSolutionInterface.h"
#include "XmlCommon.h"
#include "IProjectGenerator.h"
#include "Misc/TickCalc.h"

SSolution::SSolution(const SFileReference& SolutionXml) : Super()
{
	TickCalc<> GlobalTimer;
	TickCalc<> LocalTimer;

	SE_LOG(LogSolutionInterface, Verbose, L"Reading '{}'...", SolutionXml.GetFilename().wstring(), LocalTimer.DoCalc());
	ReadSolutionXml(SolutionXml);
	SE_LOG(LogSolutionInterface, Verbose, L"Finish to reading SolutionXml. ({} seconds)", LocalTimer.DoCalc().count());

	SE_LOG(LogSolutionInterface, Verbose, L"Search all projects.", LocalTimer.DoCalc());
	SearchProjects(ThirdpartyRoot);
	SearchProjects(EngineRoot);
	SearchProjects(GameRoot);
	SE_LOG(LogSolutionInterface, Verbose, L"Finish to search projects. {} projects found. ({} seconds)", ProjectMetadatas.size(), LocalTimer.DoCalc().count());

	SE_LOG(LogSolutionInterface, Verbose, L"Generate project runtime data.", LocalTimer.DoCalc());
	GenerateProjectsRuntimeData();
	SE_LOG(LogSolutionInterface, Verbose, L"Done. ({} seconds)", LocalTimer.DoCalc().count());
}

std::wstring SSolution::GetSolutionName()
{
	return SolutionName;
}

std::wstring SSolution::GetFirstProjectName()
{
	return FirstProject;
}

const std::map<std::wstring, ProjectBuildRuntime>& SSolution::GetProjectRuntimes()
{
	return ProjectRuntimes;
}

ISolution* SSolution::GenerateProjects(IProjectGenerator* Generator)
{
	for (auto& [Key, Value] : ProjectRuntimes)
	{
		GenerateProject(Generator, &Value);
	}

	return Generator->GenerateSolution();
}

void SSolution::ReadSolutionXml(const SFileReference& SolutionXml)
{
	using namespace tinyxml2;

	XmlBaseDirectory = std::filesystem::absolute(SolutionXml.GetPath().parent_path());

	XMLDocument Doc;
	XmlOp(Doc.LoadFile(SolutionXml.GetPath().string().c_str()));
	AssignNullCheck(SolutionInfo, Doc.FirstChildElement("SolutionInfo"));
	{
		auto InlineName = [this, SolutionInfo](std::wstring& OutName, XMLElement* Elem, const std::wstring& Default)
		{
			if (const XMLAttribute* Name = Elem->FindAttribute("Name"); Name)
			{
				OutName = ANSI_TO_WCHAR(Name->Value());
			}
			else
			{
				OutName = Default;
			}
		};

		InlineName(SolutionName, SolutionInfo, SolutionXml.GetName().wstring());

		auto InlinePath = [this, SolutionInfo](std::filesystem::path& OutPath, const std::string& ElementName)
		{
			XMLElement* Elem = SolutionInfo->FirstChildElement(ElementName.c_str());
			if (const XMLAttribute* Path; Elem && (Path = Elem->FindAttribute("Path")))
			{
				OutPath = XmlBaseDirectory / Path->Value();
			}
			else
			{
				OutPath = XmlBaseDirectory;
			}
		};

		InlinePath(GameRoot, "GameRoot");
		InlinePath(EngineRoot, "EngineRoot");
		InlinePath(ThirdpartyRoot, "ThirdpartyRoot");

		if (XMLElement* FirstProjectElem = SolutionInfo->FirstChildElement("FirstProject"))
		{
			InlineName(FirstProject, FirstProjectElem, L"");
		}
	}

	SE_LOG(LogSolutionInterface, Verbose, L"SolutionName: {}", SolutionName);
	SE_LOG(LogSolutionInterface, Verbose, L"SolutionDirectory: {}", XmlBaseDirectory.wstring());
	SE_LOG(LogSolutionInterface, Verbose, L"GameRoot: {}", GameRoot.wstring());
	SE_LOG(LogSolutionInterface, Verbose, L"EngineRoot: {}", EngineRoot.wstring());
	SE_LOG(LogSolutionInterface, Verbose, L"ThirdpartyRoot: {}", ThirdpartyRoot.wstring());
	SE_LOG(LogSolutionInterface, Verbose, L"FirstProject: {}", FirstProject);
}

void SSolution::SearchProjects(const std::filesystem::path& Directory)
{
	if (!std::filesystem::exists(Directory))
	{
		SE_LOG(LogSolutionInterface, Warning, L"Unable to found desired directory: {}. Abort.", Directory.wstring());
		return;
	}

	auto SourcePath = Directory / L"Source";
	if (!std::filesystem::exists(SourcePath))
	{
		SE_LOG(LogSolutionInterface, Warning, L"Unable to found source directory: {}. Abort.", SourcePath.wstring());
		return;
	}

	for (auto Item : std::filesystem::recursive_directory_iterator(SourcePath))
	{
		if (Item.exists())
		{
			const std::filesystem::path& XmlPath = Item.path();
			if (StringUtils::ToLower(XmlPath.extension().wstring()) == L".xml")
			{
				if (ProjectBuildMetadata Build; TryParseProject(XmlPath, Build))
				{
					SE_LOG(LogSolutionInterface, Verbose, L"Succeeded to load project metadata. XmlPath: {}", XmlPath.wstring());
					Build.BaseDirectory = Directory.stem().wstring();
					ProjectMetadatas.emplace_back(std::move(Build));
				}
				else
				{
					SE_LOG(LogSolutionInterface, Warning, L"Failed to load project metadata. XmlPath: {}", XmlPath.wstring());
				}
			}
		}
	}
}

#define XmlAttributeRet(Elem, Name) if (Name = Elem->Attribute(#Name); Name == nullptr) { return false; }

bool SSolution::TryParseProject(const std::filesystem::path& XmlPath, ProjectBuildMetadata& OutBuild)
{
	using namespace tinyxml2;
	using tinyxml2::XMLDocument;

	using EAccessKey = ProjectBuildMetadata::EAccessKey;
	using EType = ProjectBuildMetadata::EType;

	static auto ParseAccessKey = [](const char* StringToken)
	{
		return StringToken && std::string_view(StringToken) == "Public" ? EAccessKey::Public : EAccessKey::Private;
	};

	static auto ParseType = [](const char* StringToken)
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
			else if (Token == "Game")
			{
				return EType::Game;
			}
			else if (Token == "ConsoleApplication")
			{
				return EType::ConsoleApplication;
			}
			else if (Token == "Module")
			{
				return EType::Module;
			}
		}

		return EType::None;
	};

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

void SSolution::GenerateProjectsRuntimeData()
{
	for (auto& Meta : ProjectMetadatas)
	{
		auto [It, Result] = ProjectRuntimes.emplace(Meta.Name, ProjectBuildRuntime());
		if (!Result)
		{
			SE_LOG(LogSolutionInterface, Error, LR"(Duplicated project name("{}") detected!)", Meta.Name);
			throw std::exception("Unhandled exception.");
		}

		auto& [Key, Value] = *It;
		Value.Metadata = &Meta;
	}

	for (auto& KeyValuePair : ProjectRuntimes)
	{
		auto& [Key, Value] = KeyValuePair;
		BuildRuntime(&Value);
	}
}

void SSolution::BuildRuntime(ProjectBuildRuntime* Runtime)
{
	using EAccessKey = ProjectBuildMetadata::EAccessKey;

	if (!Runtime->bBuild)
	{
		// Build prerequisites.
		for (auto& Reference : Runtime->Metadata->ReferencedProjects)
		{
			ProjectBuildRuntime* ReferencedRuntime = &ProjectRuntimes[Reference.Name];
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

		Runtime->ProjectGuid = Guid::NewGuid();
		Runtime->bBuild = true;
	}
}

void SSolution::GenerateProject(IProjectGenerator* Generator, ProjectBuildRuntime* Runtime)
{
	if (Runtime->GeneratedProject)
	{
		// Already generated.
		return;
	}

	SE_LOG(LogSolutionInterface, Verbose, L"Check dependencies for '{}' project.", Runtime->Metadata->Name);

	for (auto& Reference : Runtime->PrivateReferences)
	{
		if (Reference->GeneratedProject == nullptr)
		{
			GenerateProject(Generator, Reference);
		}
	}

	for (auto& Reference : Runtime->PublicReferences)
	{
		if (Reference->GeneratedProject == nullptr)
		{
			GenerateProject(Generator, Reference);
		}
	}

	SE_LOG(LogSolutionInterface, Verbose, L"Generating '{}' project.", Runtime->Metadata->Name);
	Runtime->GeneratedProject = Generator->GenerateProject(*Runtime);
}