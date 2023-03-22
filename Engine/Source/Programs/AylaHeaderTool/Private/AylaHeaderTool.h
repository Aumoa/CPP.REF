// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class AylaHeaderTool
{
private:
	String SourceLocation;

public:
	AylaHeaderTool();

	int32 Run();

private:
	void PrintUsage(TextWriter& Writer);
};