// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Parser/CppErrorExpression.h"

CppErrorExpression::CppErrorExpression(String expression, FileReference sourceFile, size_t lineNumber, size_t charNumber)
	: Super(expression, sourceFile, lineNumber, charNumber)
{
}

String CppErrorExpression::Format() const
{
	return String::Format(TEXT("{0}: {1}"), Super::Format(), TEXT("Unexpected character: ") + GetExpression());
}