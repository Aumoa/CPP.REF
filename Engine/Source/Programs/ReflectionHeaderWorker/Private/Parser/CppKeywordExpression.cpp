// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Parser/CppKeywordExpression.h"

CppKeywordExpression::CppKeywordExpression(String expression, FileReference sourceFile, size_t lineNumber, size_t charNumber)
	: Super(expression, sourceFile, lineNumber, charNumber)
{
}