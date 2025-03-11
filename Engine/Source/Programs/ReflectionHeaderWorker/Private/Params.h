// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class Params
{
	Params() = delete;

private:
	static DirectoryReference s_InputDirectory;

public:
	static void Init();

	static DirectoryReference GetInputDirectory();
};