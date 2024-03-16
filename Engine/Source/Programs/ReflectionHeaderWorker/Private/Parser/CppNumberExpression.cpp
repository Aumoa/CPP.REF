// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Parser/CppNumberExpression.h"

CppNumberExpression::CppNumberExpression(String expression, FileReference sourceFile, size_t lineNumber, size_t charNumber, bool isReal)
	: Super(expression, sourceFile, lineNumber, charNumber)
	, isReal(isReal)
{
}

bool CppNumberExpression::IsRealNumber() const
{
	return isReal;
}