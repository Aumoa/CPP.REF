// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

struct SourceCodeLocation
{
	String Filename;
	size_t LinePos;
	size_t CharPos;
	size_t SeekPos;

	String ToString() const
	{
		return String::Format(TEXT("{}({},{})"), Filename, LinePos, CharPos);
	}
};