// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Compiler/CppClassSyntaxTree.h"
#include "Compiler/CompileErrors.h"
#include "Compiler/SourceCodeLocator.h"
#include "Exceptions/TerminateException.h"
#include "Reflection/AccessSpecifier.h"

CppClassSyntaxTree::CppClassSyntaxTree()
{
}

CppClassSyntaxTree::~CppClassSyntaxTree() noexcept
{
}

void CppClassSyntaxTree::Parse(String TypeName, SourceCodeLocator& Locator)
{
	String Next = Locator.GetWord(true);
	if (Next == TEXT(":"))
	{
		ParseInheritances(Locator);
	}
	else if (Next != TEXT("{"))
	{
		throw TerminateException(String::Format(TEXT("{}: {}"), Locator.ToString(), CompileErrors::IllegalScope(true)));
	}

	ParseClassBody(TypeName, Locator);
}

int32 CppClassSyntaxTree::GetGeneratedBodyLineNumber() const
{
	return LineOfGeneratedBody;
}

void CppClassSyntaxTree::ParseInheritances(SourceCodeLocator& Locator)
{
	String Next;

	auto AddInheritance = [&]()
	{
		Next = Locator.GetWord(true);
		if (IsKeyword(Next))
		{
			throw TerminateException(String::Format(TEXT("{}: {}"), Locator.ToString(), CompileErrors::IllegalKeyword(Next)));
		}

		if (SuperClass.IsEmpty() == false)
		{
			throw TerminateException(String::Format(TEXT("{}: {}"), Locator.ToString(), CompileErrors::MultipleInheritance()));
		}

		SuperClass = Next;
	};

	while (true)
	{
		Next = Locator.GetWord(true);
		if (Next == TEXT("public"))
		{
			AddInheritance();
		}
		else if (Next == TEXT("protected") || Next == TEXT("private"))
		{
			throw TerminateException(String::Format(TEXT("{}: {}"), Locator.ToString(), CompileErrors::IllegalAccess()));
		}
		else
		{
			if (Next == TEXT("{"))
			{
				break;
			}
			else
			{
				AddInheritance();
			}
		}
	}
}

void CppClassSyntaxTree::ParseClassBody(String TypeName, SourceCodeLocator& Locator)
{
	LineOfGeneratedBody = Locator.GetPosition().LinePos;
	String Next = Locator.GetWord(true);
	if (Next != TEXT("GENERATED_BODY") || Locator.GetWord(true) != TEXT("(") || Locator.GetWord(true) != TEXT(")"))
	{
		throw TerminateException(String::Format(TEXT("{}: {}"), Locator.ToString(), CompileErrors::GeneratedBody()));
	}

	EAccessSpecifier CurrentAccess = EAccessSpecifier::Private;

	while (true)
	{
		Next = Locator.GetWord(true);
		if (Next == TEXT("public"))
		{
			CurrentAccess = EAccessSpecifier::Public;
			if (Locator.GetWord(true) != TEXT(":"))
			{
				throw TerminateException(String::Format(TEXT("{}: {}"), Locator.ToString(), CompileErrors::Colon()));
			}
		}
		else if (Next == TEXT("protected"))
		{
			CurrentAccess = EAccessSpecifier::Protected;
			if (Locator.GetWord(true) != TEXT(":"))
			{
				throw TerminateException(String::Format(TEXT("{}: {}"), Locator.ToString(), CompileErrors::Colon()));
			}
		}
		else if (Next == TEXT("private"))
		{
			CurrentAccess = EAccessSpecifier::Private;
			if (Locator.GetWord(true) != TEXT(":"))
			{
				throw TerminateException(String::Format(TEXT("{}: {}"), Locator.ToString(), CompileErrors::Colon()));
			}
		}
		else if (Next == TEXT("internal"))
		{
			CurrentAccess = EAccessSpecifier::Internal;
			if (Locator.GetWord(true) != TEXT(":"))
			{
				throw TerminateException(String::Format(TEXT("{}: {}"), Locator.ToString(), CompileErrors::Colon()));
			}
		}

		// escape.
		else if (Next == TEXT("}"))
		{
			break;
		}
	}
}

bool CppClassSyntaxTree::IsKeyword(String InWord)
{
	static std::set<String> Keywords =
	{
		TEXT("public"), TEXT("protected"), TEXT("private"), TEXT("internal")
	};

	return Keywords.contains(InWord);
}