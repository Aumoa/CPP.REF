// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CppExpression.h"

class CppNumberExpression : public CppExpression
{
public:
	using Super = CppExpression;

private:
	const bool isReal;

public:
	CppNumberExpression(String expression, FileReference sourceFile, size_t lineNumber, size_t charNumber, bool isReal);

	bool IsRealNumber() const;
};