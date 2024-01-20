// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class ENGINE_API Global
{
	Global() = delete;

private:
	static DirectoryReference EngineDirectory;
	static DirectoryReference GameDirectory;

public:
	static void SetEngineDirectory(const DirectoryReference& InDirectory);
	static DirectoryReference GetEngineDirectory() noexcept { return EngineDirectory; }

	static void SetGameDirectory(const DirectoryReference& InDirectory);
	static DirectoryReference GetGameDirectory() noexcept { return GameDirectory; }
};