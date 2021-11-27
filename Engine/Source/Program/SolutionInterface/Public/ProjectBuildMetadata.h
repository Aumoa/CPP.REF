// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

struct ProjectBuildMetadata
{
	enum class EType
	{
		None,
		Module,
		ConsoleApplication,
		Application,
		Console,
		Game,
	};

	enum class EAccessKey
	{
		Public,
		Private
	};

	std::wstring AbsoluteDirectory;
	std::wstring BaseDirectory;

	std::wstring Name;
	std::wstring Path;
	EType Type;

	std::wstring Command;
	std::wstring CommandArgs;

	struct IncludePath
	{
		std::filesystem::path RelativePath;
		EAccessKey Access = EAccessKey::Private;
	};
	std::vector<IncludePath> IncludePaths;

	struct ReferencedProject
	{
		std::wstring Name;
		EAccessKey Access = EAccessKey::Private;
	};
	std::vector<ReferencedProject> ReferencedProjects;

	struct DisableWarning
	{
		int32 Number = -1;
		EAccessKey Access = EAccessKey::Private;
	};
	std::vector<DisableWarning> DisableWarnings;

	struct ExternalLink
	{
		std::wstring Name;
		EAccessKey Access = EAccessKey::Private;
	};
	std::vector<ExternalLink> ExternalLinks;
};