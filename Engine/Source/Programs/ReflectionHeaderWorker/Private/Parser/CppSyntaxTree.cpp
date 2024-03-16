// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "CppSyntaxTree.h"
#include "Parser/CppNumberExpression.h"
#include "Parser/CppErrorExpression.h"
#include "Parser/CppCommentExpression.h"

CppSyntaxTree::CppSyntaxTree(FileReference sourceFile, String sourceCode)
	: sourceFile(sourceFile)
	, sourceCode(sourceCode)
{
}

CppSyntaxTree::~CppSyntaxTree() noexcept
{
}

void CppSyntaxTree::Parse()
{
	lineNumber = 1;
	charNumber = 1;
	index = 0;

	while (sourceCode.IsValidIndex(index))
	{
		if (Char::IsWhiteSpace(sourceCode[index]))
		{
			IncrementIndex(false);
			continue;
		}

		if (CompareCharsSimple('\r'))
		{
			++index;
			continue;
		}

		if (CompareCharsSimple('\n'))
		{
			IncrementIndex(true);
			continue;
		}

		if (Char::IsDigit(sourceCode[index]))
		{
			expressions.emplace_back(ReadNumberExpression());
			continue;
		}

		if (CompareCharsSimple('/', '/'))
		{
			expressions.emplace_back(ReadSingleLineComment());
			continue;
		}

		expressions.emplace_back(MakeErrorExpression());
		break;
	}
}

bool CppSyntaxTree::HasError() const
{
	using namespace Linq;
	return expressions | Any([](const auto& p) { return dynamic_cast<const CppErrorExpression*>(p.get()); });
}

std::unique_ptr<CppExpression> CppSyntaxTree::MakeErrorExpression()
{
	return std::make_unique<CppErrorExpression>(sourceCode.Substring(index, 1), sourceFile, lineNumber, charNumber);
}

std::unique_ptr<CppExpression> CppSyntaxTree::ReadNumberExpression()
{
	bool isReal = false;
	bool isHex = false;
	bool isBinary = false;
	bool isDelimiter = false;
	bool isOctal = false;
	bool isFloat = false;
	size_t start = index;
	IncrementIndex(false);

	while (sourceCode.IsValidIndex(index))
	{
		char_t ch = sourceCode[index];

		// begin scope
		if (Char::ToLower(ch) == 'x' && sourceCode[index - 1] == '0' && index == start + 1)
		{
			IncrementIndex(false);
			isHex = true;
			continue;
		}

		if (Char::ToLower(ch) == 'b' && sourceCode[index - 1] == '0' && index == start + 1)
		{
			IncrementIndex(false);
			isBinary = true;
			continue;
		}

		if (Char::IsDigit(ch) && sourceCode[index - 1] == '0' && index == start + 1)
		{
			IncrementIndex(false);
			isOctal = true;
			continue;
		}

		// middle scope
		if (ch == '.' && !isHex && !isBinary)
		{
			IncrementIndex(false);
			isReal = true;
			isOctal = false;
			continue;
		}

		if (ch == '\'' && !isDelimiter)
		{
			IncrementIndex(false);
			isDelimiter = true;
			continue;
		}

		if (Char::IsDigit(ch))
		{
			IncrementIndex(false);
			isDelimiter = false;
			continue;
		}

		if (isHex && Char::IsHexChar(ch))
		{
			IncrementIndex(false);
			isDelimiter = false;
			continue;
		}

		// ending scope
		if (Char::IsWhiteSpace(ch))
		{
			break;
		}

		if (isReal)
		{
			if (CompareCharsSimple('f'))
			{
				IncrementIndex(false);
				isFloat = true;
				break;
			}

			return MakeErrorExpression();
		}
		else
		{
			if (CompareCharsSimple('u') ||
				CompareCharsSimple('l') ||
				CompareCharsSimple('u', 'l') ||
				CompareCharsSimple('l', 'u') ||
				CompareCharsSimple('u', 'l', 'l') ||
				CompareCharsSimple('l', 'l', 'u'))
			{
				IncrementIndex(false);
				break;
			}

			return MakeErrorExpression();
		}
	}

	return std::make_unique<CppNumberExpression>(sourceCode.Substring(start, index - start), sourceFile, lineNumber, charNumber, isReal);
}

std::unique_ptr<CppExpression> CppSyntaxTree::ReadSingleLineComment()
{
	size_t start = index;
	IncrementIndex(false);
	IncrementIndex(false);
	while (sourceCode.IsValidIndex(index))
	{
		if (sourceCode[index] == '\n' || sourceCode[index] == '\r')
		{
			return std::make_unique<CppCommentExpression>(sourceCode.Substring(start, index - start), sourceFile, lineNumber, charNumber);
		}

		IncrementIndex(false);
	}

	return std::make_unique<CppCommentExpression>(sourceCode.Substring(start, index - start), sourceFile, lineNumber, charNumber);
}

void CppSyntaxTree::IncrementIndex(bool isNewLine)
{
	++index;
	if (isNewLine)
	{
		++lineNumber;
		charNumber = 1;
	}
	else
	{
		++charNumber;
	}
}