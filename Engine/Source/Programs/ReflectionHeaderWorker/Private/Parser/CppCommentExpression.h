// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Parser/CppExpression.h"

class CppCommentExpression : public CppExpression
{
public:
	using Super = CppExpression;

public:
	CppCommentExpression(String expression, FileReference sourceFile, size_t lineNumber, size_t charNumber);
};