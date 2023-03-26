// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Compiler/SourceCodeLocation.h"

class SourceCodeLocator
{
	String FilenameHint;
	String SourceCode;

	size_t Pos;
	std::vector<size_t> Lines;

public:
	SourceCodeLocator(String InFilenameHint, String InSourceCode);

	String ToString() const;

	void SetEOF();
	void IncrementPos(int32 Pos);
	void IncrementLine();
	void SkipWhitespace();

	String GetCurrentView() const;
	SourceCodeLocation GetPosition() const;
	bool IsEOF() const;

	String GetWord(bool bStringView = false, bool bPeek = false);
};