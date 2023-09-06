// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

struct CxxCodeParsingContext
{
	String CodePath;
	String Code;
	int32 Line;
	int32 Column;
	size_t Index;

	void Init(String InCodePath, String InCode)
	{
		CodePath = InCodePath;
		if constexpr (Environment::NewLine != TEXT("\n"))
		{
			Code = InCode.Replace(Environment::NewLine, TEXT("\n"));
		}
		else
		{
			Code = InCode;
		}
		Line = 1;
		Column = 0;
		Index = 0;
	}

	constexpr bool IsEOF() const noexcept
	{
		return Index >= Code.length();
	}

	constexpr bool ScopedEquals(String InCode) const noexcept
	{
		String Substr = Code.SubstringView(Index, InCode.length());
		return Substr == InCode;
	}

	constexpr const char_t& GetChar() const noexcept
	{
		return Code[Index];
	}

	static constexpr char_t OperatorChars[] =
	{
		'(', ')', '<', '>',
		'*', '+', '-', '/', '%', '^', '~',
		'!', '&', '=',
		':',
		'{', '}', '[', ']',
		';',
	};

	static constexpr auto Keywords = std::array
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

	const char_t& Increment() noexcept
	{
		check(!IsEOF());
		const char_t& Ch = Code[Index++];
		if (Ch == '\n')
		{
			++Line;
			Column = 0;
		}
		else
		{
			++Column;
		}
		return Ch;
	}

	void IgnoreWhitespaces() noexcept
	{
		if (Char::IsWhiteSpace(Code[Index]) == false)
		{
			return;
		}

		for (; !IsEOF();)
		{
			Increment();
			if (Char::IsWhiteSpace(GetChar()) == false)
			{
				break;
			}
		}
	}

	String ReadLine()
	{
		size_t StartIndex = Index;

		for (; !IsEOF();)
		{
			const char_t& Ch = Increment();
			if (Ch == '\n')
			{
				break;
			}
		}

		size_t Length = Index - StartIndex;
		return Code.Substring(StartIndex, Length - 1);
	}

	String ReadIdentifier()
	{
		size_t StartIndex = Index;

		for (; !IsEOF();)
		{
			Increment();
			if (IsIdentifierChar(GetChar()) == false)
			{
				break;
			}
		}
		
		size_t Length = Index - StartIndex;
		return Code.Substring(StartIndex, Length);
	}

	String ExportString(size_t Len)
	{
		size_t StartIndex = Index;
		for (size_t i = 0; i < Len; ++i)
		{
			Increment();
		}
		size_t Length = Index - StartIndex;
		return Code.Substring(StartIndex, Length);
	}

	String ReadExpression()
	{
		size_t StartIndex = Index;

		for (; !IsEOF();)
		{
			Increment();
			if (IsExpressionChar(GetChar()) == false)
			{
				break;
			}
		}

		size_t Length = Index - StartIndex;
		return Code.Substring(StartIndex, Length);
	}

	String ReadStringLiteral()
	{
		size_t StartIndex = Index;
		bool bRawString = false;

		for (; !IsEOF();)
		{
			const char_t& Ch = Increment();
			if (Ch == '\"')
			{
				if (bRawString)
				{
					InternalReadRawString();
				}
				else
				{
					InternalReadString();
				}
				break;
			}
			else if (Ch == 'R')
			{
				bRawString = true;
			}
		}
		
		size_t Length = Index - StartIndex;
		return Code.Substring(StartIndex, Length);
	}

private:
	void InternalReadString()
	{
		for (; !IsEOF();)
		{
			const char_t& Ch = Increment();
			if (Ch == '\"' && Code[Index - 2] != '\\')
			{
				break;
			}
		}
	}

	void InternalReadRawString()
	{
		std::vector<char_t> RawDeclV;
		String RawDecl;
		for (; !IsEOF();)
		{
			const char_t& Ch = Increment();
			if (Ch == '(')
			{
				RawDecl = String::FromLiteral(std::wstring_view(RawDeclV.data(), RawDeclV.size()));
				break;
			}
			else
			{
				RawDeclV.emplace_back(Ch);
			}
		}

		for (; !IsEOF();)
		{
			const char_t& Ch = Increment();
			if (Ch == ')')
			{
				if (RawDecl.IsEmpty() || ScopedEquals(RawDecl))
				{
					Index += RawDecl.length();
					break;
				}
			}
		}
	}

private:
	static constexpr bool IsIdentifierChar(char_t Ch) noexcept
	{
		return Char::IsDigit(Ch)
			|| Char::IsAlpha(Ch)
			|| Ch == '_';
	}

	static constexpr bool IsExpressionChar(char_t Ch) noexcept
	{
		return Char::IsDigit(Ch)
			|| Char::IsAlpha(Ch)
			|| Ch == '_'
			|| Ch == '.';
	}
};