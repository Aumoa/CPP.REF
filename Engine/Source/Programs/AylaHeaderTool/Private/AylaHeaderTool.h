// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class AylaHeaderTool
{
private:
	String SourceLocation;
	String Intermediate;

public:
	AylaHeaderTool();

	int32 Run();

	inline String GetSourceLocation() const { return SourceLocation; }
	inline String GetIntermediateLocation() const { return Intermediate; }

private:
	void PrintUsage(TextWriter& Writer);
};