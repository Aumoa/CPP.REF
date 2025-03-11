// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Parser/CppExpression.h"

CppExpression::CppExpression(String expression, FileReference sourceFile, size_t lineNumber, size_t charNumber)
	: expression(expression)
	, sourceFile(sourceFile)
	, lineNumber(lineNumber)
	, charNumber(charNumber)
{
}

CppExpression::~CppExpression() noexcept
{
}

String CppExpression::Format() const
{
	return String::Format(TEXT("{0}({1},{2})"), sourceFile.GetAbsolutePath(), lineNumber, charNumber);
}

String CppExpression::GetExpression() const
{
	return expression;
}

FileReference CppExpression::GetSourceFile() const
{
	return sourceFile;
}

size_t CppExpression::GetLineNumber() const
{
	return lineNumber;
}

size_t CppExpression::GetCharNumber() const
{
	return charNumber;
}