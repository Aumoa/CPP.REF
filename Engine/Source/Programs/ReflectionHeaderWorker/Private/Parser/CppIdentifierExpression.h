// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CppExpression.h"

class CppIdentifierExpression : public CppExpression
{
public:
	using Super = CppExpression;

public:
	CppIdentifierExpression(String expression, FileReference sourceFile, size_t lineNumber, size_t charNumber);
};