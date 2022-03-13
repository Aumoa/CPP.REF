// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/Paths.h"
#include <stdexcept>

std::filesystem::path Paths::AsPhysicalPath(std::filesystem::path InLogicalPath)
{
	using namespace std::filesystem;

	path LogicalRoot = InLogicalPath.root_directory();
	path PhysicalRoot;

	if (LogicalRoot == L"Engine")
	{
		PhysicalRoot = GetPhysicalRoot(ERoot::Engine);
		InLogicalPath = relative(InLogicalPath, L"Engine/");
	}
	else if (LogicalRoot == L"Game")
	{
		PhysicalRoot = GetPhysicalRoot(ERoot::Game);
		InLogicalPath = relative(InLogicalPath, L"Game/");
	}
	else
	{
		PhysicalRoot = GetPhysicalRoot(ERoot::Game);
	}

	PhysicalRoot /= "Content" / InLogicalPath;
	return PhysicalRoot;
}

std::filesystem::path Paths::GetPhysicalRoot(ERoot Root)
{
	switch (Root)
	{
	case ERoot::Engine:
		return ENGINE_ROOT;
	case ERoot::Game:
		return GAME_ROOT;
	default:
		throw std::invalid_argument("An invalid argument specified in 'Root' argument.");
	}
}