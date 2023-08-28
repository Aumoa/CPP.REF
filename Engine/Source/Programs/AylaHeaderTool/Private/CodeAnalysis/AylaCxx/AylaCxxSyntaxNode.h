// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CodeAnalysis/SyntaxNode.h"

class AylaCxxSyntaxTree;

class AylaCxxSyntaxNode : public SyntaxNode
{
	friend AylaCxxSyntaxTree;

private:
	std::vector<CodeDiagnostic> Diagnostics;
	String Code;

public:
	static constexpr int32 ErrorCode_UnknownSyntaxExpression = 1;
	static constexpr String Message_UnknownSyntaxExpression = TEXT("Unknown Syntax Expression");

private:
	struct CodeParsingContext
	{
		String CodePath;
		String Code;
		int32 Line;
		int32 Column;
		size_t Index;

		constexpr bool IsEOF() const noexcept
		{
			return Index >= Code.length();
		}

		constexpr bool Equals(String InCode) const noexcept
		{
			String Substr = Code.SubstringView(Index, InCode.length());
			return Substr == InCode;
		}

		void Increment()
		{
			if (IsEOF())
			{
				return;
			}

			const char_t C = Code[Index++];
			switch (C)
			{
			case '\n':
				++Line;
				Column = 0;
				break;
			case '\r':
				break;
			default:
				++Column;
				break;
			}
		}
	};

private:
	AylaCxxSyntaxNode();

public:
	virtual std::vector<CodeDiagnostic> GetDiagnostics() const override;

public:
	static std::unique_ptr<SyntaxNode> ParseText(CodeParsingContext& Context);

private:
	static std::unique_ptr<SyntaxNode> ParseText_Comment(CodeParsingContext& Context);

private:
	static void IgnoreWhitespaces(CodeParsingContext& Context);
};