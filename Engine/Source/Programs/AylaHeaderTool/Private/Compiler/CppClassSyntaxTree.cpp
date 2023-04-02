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

	if (Next != TEXT("{"))
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
	throw NotImplementedException();
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