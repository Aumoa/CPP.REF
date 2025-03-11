// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Global.h"

DirectoryReference Global::EngineDirectory;
DirectoryReference Global::GameDirectory;

void Global::SetEngineDirectory(const DirectoryReference& InDirectory)
{
	EngineDirectory = InDirectory.GetAbsolute();
}

void Global::SetGameDirectory(const DirectoryReference& InDirectory)
{
	GameDirectory = InDirectory.GetAbsolute();
}