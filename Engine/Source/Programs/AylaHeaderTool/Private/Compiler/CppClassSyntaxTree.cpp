// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Compiler/CppClassSyntaxTree.h"
#include "Compiler/CompileErrors.h"
#include "Compiler/SourceCodeLocator.h"
#include "Exceptions/TerminateException.h"

CppClassSyntaxTree::CppClassSyntaxTree()
{
}

void CppClassSyntaxTree::Parse(SourceCodeLocator& Locator)
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

	ParseClassBody(Locator);
}

void CppClassSyntaxTree::ParseInheritances(SourceCodeLocator& Locator)
{
	throw NotImplementedException();
}

void CppClassSyntaxTree::ParseClassBody(SourceCodeLocator& Locator)
{
	String Next = Locator.GetWord(true);
	if (Next != TEXT("GENERATED_BODY") || Locator.GetWord(true) != TEXT("(") || Locator.GetWord(true) != TEXT(")"))
	{
		throw TerminateException(String::Format(TEXT("{}: {}"), Locator.ToString(), CompileErrors::GeneratedBody()));
	}

	while (true)
	{
		Next = Locator.GetWord(true);
		if (Next == TEXT("public"))
		{
		}
	}
}