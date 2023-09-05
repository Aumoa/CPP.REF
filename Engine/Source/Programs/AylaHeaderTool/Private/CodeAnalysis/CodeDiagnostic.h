// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class CodeDiagnostic
{
	String Path;
	int32 Line;
	int32 Column;
	int32 Code;
	String Message;

public:
	CodeDiagnostic(String InPath, int32 InLine, int32 InColumn, int32 InCode, String InMessage)
		: Path(InPath)
		, Line(InLine)
		, Column(InColumn)
		, Code(InCode)
		, Message(InMessage)
	{
	}

	inline String ToString() const
	{
		return String::Format(TEXT("{}({},{}): error A{:0>3}: {}"), Path, Line, Column, Code, Message);
	}
};