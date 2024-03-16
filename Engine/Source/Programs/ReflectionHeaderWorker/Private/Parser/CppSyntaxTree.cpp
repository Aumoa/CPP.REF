// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "CppSyntaxTree.h"
#include "Parser/CppNumberExpression.h"
#include "Parser/CppErrorExpression.h"
#include "Parser/CppCommentExpression.h"
#include "Parser/CppPreprocessorExpression.h"
#include "Parser/CppIdentifierExpression.h"
#include "Parser/CppKeywordExpression.h"

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
		if (CompareCharsSimple("\r"))
		{
			++index;
			continue;
		}

		if (CompareCharsSimple("\n"))
		{
			IncrementIndex(true);
			continue;
		}

		if (Char::IsWhiteSpace(sourceCode[index]))
		{
			IncrementIndex(false);
			continue;
		}

		if (CompareCharsSimple("//"))
		{
			expressions.emplace_back(ReadSingleLineComment());
			continue;
		}

		if (CompareCharsSimple("#"))
		{
			expressions.emplace_back(ReadPreprocessor());
			continue;
		}

		if (Char::IsDigit(sourceCode[index]))
		{
			expressions.emplace_back(ReadNumberExpression());
			continue;
		}

		if (CompareCharsMultiple("char", "short", "int", "long", "float", "double", "bool", "void",
			"class", "struct", "enum",
			"static", "volatile", "thread_local", "const", "mutable", "constexpr",
			"typedef", "using",
			"noexcept", "concept", "requires",
			"public", "protected", "private",
			"if", "while", "else", "do", "switch",
			"try", "catch",
			"case", "break", "continue",
			"virtual", "override", "final",
			"static_cast", "dynamic_cast", "const_cast", "reinterpret_cast",
			"false", "true", "nullptr"))
		{
			expressions.emplace_back(ReadKeyword());
			continue;
		}

		if (Char::IsAlpha(sourceCode[index]))
		{
			expressions.emplace_back(ReadIdentifier());
			continue;
		}

		expressions.emplace_back(MakeErrorExpression());
		break;
	}
}

bool CppSyntaxTree::TryGetError(const CppExpression** outExpression) const
{
	for (auto& expression : expressions)
	{
		if (auto* errorExp = dynamic_cast<const CppErrorExpression*>(expression.get()))
		{
			*outExpression = errorExp;
			return true;
		}
	}

	outExpression = nullptr;
	return false;
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
			if (CompareCharsSimple("f"))
			{
				IncrementIndex(false);
				isFloat = true;
				break;
			}

			return MakeErrorExpression();
		}
		else
		{
			if (CompareCharsSimple("u") ||
				CompareCharsSimple("l") ||
				CompareCharsSimple("ul") ||
				CompareCharsSimple("lu") ||
				CompareCharsSimple("ull") ||
				CompareCharsSimple("llu"))
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

std::unique_ptr<CppExpression> CppSyntaxTree::ReadPreprocessor()
{
	size_t start = index;
	IncrementIndex(false);
	IncrementIndex(false);
	while (sourceCode.IsValidIndex(index))
	{
		if (CompareCharsSimple("\n"))
		{
			if (sourceCode[index - 1] == '\\')
			{
				IncrementIndex(true);
			}
			else
			{
				return std::make_unique<CppPreprocessorExpression>(sourceCode.Substring(start, index - start), sourceFile, lineNumber, charNumber);
			}
		}
		else if (CompareCharsSimple("\r\n"))
		{
			if (sourceCode[index - 1] == '\\')
			{
				IncrementIndex(true);
				IncrementIndex(true);
			}
			else
			{
				return std::make_unique<CppPreprocessorExpression>(sourceCode.Substring(start, index - start), sourceFile, lineNumber, charNumber);
			}
		}

		IncrementIndex(false);
	}

	return std::make_unique<CppPreprocessorExpression>(sourceCode.Substring(start, index - start), sourceFile, lineNumber, charNumber);
}

std::unique_ptr<CppExpression> CppSyntaxTree::ReadIdentifier()
{
	size_t start = index;
	IncrementIndex(false);
	while (sourceCode.IsValidIndex(index))
	{
		auto ch = sourceCode[index];
		if (Char::IsAlpha(ch) || ch == '_')
		{
			IncrementIndex(false);
			continue;
		}

		return std::make_unique<CppIdentifierExpression>(sourceCode.Substring(start, index - start), sourceFile, lineNumber, charNumber);
	}

	return std::make_unique<CppIdentifierExpression>(sourceCode.Substring(start, index - start), sourceFile, lineNumber, charNumber);
}

std::unique_ptr<CppExpression> CppSyntaxTree::ReadKeyword()
{
	size_t start = index;
	IncrementIndex(false);
	while (sourceCode.IsValidIndex(index))
	{
		auto ch = sourceCode[index];
		if (Char::IsAlpha(ch) || ch == '_')
		{
			IncrementIndex(false);
			continue;
		}

		return std::make_unique<CppKeywordExpression>(sourceCode.Substring(start, index - start), sourceFile, lineNumber, charNumber);
	}

	return std::make_unique<CppKeywordExpression>(sourceCode.Substring(start, index - start), sourceFile, lineNumber, charNumber);
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