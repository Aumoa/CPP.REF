// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CodeAnalysis/SyntaxNode.h"

class AylaCxxSyntaxTree;

class AylaCxxSyntaxNode : public SyntaxNode
{
	friend AylaCxxSyntaxTree;

public:
	enum class ESyntaxType
	{
		None,
		Comment,
		Preprocessor,
		Identifier,
		Operator,
		Keyword
	};

private:
	std::vector<CodeDiagnostic> Diagnostics;
	String Code;
	ESyntaxType SyntaxType;

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

		char_t GetChar() const
		{
			return Code[Index];
		}

		String ReadLine()
		{
			size_t StartIndex = Index;
			String Value;

			while (IsEOF() == false)
			{
				const char_t C = Code[Index];
				Increment();
				if (C == '\n')
				{
					Value = Code.Substring(StartIndex, Index - StartIndex).TrimEnd();
					break;
				}
			}

			if (Value.IsEmpty())
			{
				Value = Code.Substring(StartIndex, Index - StartIndex);
			}

			return Value;
		}

		static constexpr char_t OperatorChars[] =
		{
			'(', ')', '<', '>',
			'*', '+', '-', '/', '%', '^', '~',
			'!', '&', '=',
			':',
			'{', '}', '[', ']',
		};

		static inline String Keywords[] =
		{
			TEXT("new"),
			TEXT("delete"),
			TEXT("int"),
			TEXT("short"),
			TEXT("double"),
			TEXT("char"),
			TEXT("long"),
			TEXT("wchar_t"),
			TEXT("class"),
			TEXT("struct"),
			TEXT("public"),
			TEXT("private"),
			TEXT("protected"),
			TEXT("virtual"),
			TEXT("noexcept"),
			TEXT("void"),
			TEXT("false"),
			TEXT("true"),
			TEXT("bool"),
			TEXT("auto"),
			TEXT("const"),
			TEXT("override"),
			TEXT("final"),
			TEXT("constexpr")
		};

		String ReadIdentifier()
		{
			size_t StartIndex = Index;
			String Value;

			while (IsEOF() == false)
			{
				const char_t C = Code[Index];
				if (Char::IsWhiteSpace(C) || Linq::Contains(OperatorChars, C))
				{
					Value = Code.Substring(StartIndex, Index - StartIndex).TrimEnd();
					break;
				}
				Increment();
			}

			if (Value.IsEmpty())
			{
				Value = Code.Substring(StartIndex, Index - StartIndex);
			}

			return Value;
		}

		String ConsumeString(size_t Len)
		{
			String Str = Code.Substring(Index, Len);
			for (size_t i = 0; i < Str.length(); ++i)
			{
				Increment();
			}
			return Str;
		}
	};

private:
	AylaCxxSyntaxNode(ESyntaxType InSyntaxType);

public:
	ESyntaxType GetSyntaxType() const;
	virtual std::vector<CodeDiagnostic> GetDiagnostics() const override;

public:
	static std::unique_ptr<SyntaxNode> ParseText(CodeParsingContext& Context);

private:
	static std::unique_ptr<SyntaxNode> ParseText_Comment(CodeParsingContext& Context);
	static std::unique_ptr<SyntaxNode> ParseText_Preprocessor(CodeParsingContext& Context);
	static std::unique_ptr<SyntaxNode> ParseText_Identifier(CodeParsingContext& Context);
	static std::unique_ptr<SyntaxNode> ParseText_Operator(CodeParsingContext& Context);

private:
	static void IgnoreWhitespaces(CodeParsingContext& Context);
};