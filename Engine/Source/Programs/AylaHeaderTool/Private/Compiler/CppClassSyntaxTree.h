// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Compiler/CppSyntax.h"

class SourceCodeLocator;

class CppClassSyntaxTree
{
	std::vector<CppSyntax> Syntaxes;
	int32 LineOfGeneratedBody = 0;

	String SuperClass;

public:
	CppClassSyntaxTree();
	~CppClassSyntaxTree() noexcept;

	void Parse(String TypeName, SourceCodeLocator& Locator);

	int32 GetGeneratedBodyLineNumber() const;

private:
	void ParseInheritances(SourceCodeLocator& Locator);
	void ParseClassBody(String TypeName, SourceCodeLocator& Locator);

private:
	static bool IsKeyword(String InWord);
};