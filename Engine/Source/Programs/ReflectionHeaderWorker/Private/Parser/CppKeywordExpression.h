// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Parser/CppExpression.h"

class CppKeywordExpression : public CppExpression
{
public:
	using Super = CppExpression;

public:
	CppKeywordExpression(String expression, FileReference sourceFile, size_t lineNumber, size_t charNumber);
};