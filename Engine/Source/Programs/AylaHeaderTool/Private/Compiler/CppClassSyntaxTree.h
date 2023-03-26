// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Compiler/CppSyntax.h"

class SourceCodeLocator;

class CppClassSyntaxTree
{
	std::vector<CppSyntax> Syntaxes;

public:
	CppClassSyntaxTree();

	void Parse(SourceCodeLocator& Locator);
	
private:
	void ParseInheritances(SourceCodeLocator& Locator);
	void ParseClassBody(SourceCodeLocator& Locator);
};