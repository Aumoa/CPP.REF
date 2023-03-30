// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Reflection/AType.h"
#include "Compiler/CppClassSyntaxTree.h"

class AAssembly;

class AClass : public AType
{
	String TypeName;
	CppClassSyntaxTree SyntaxTree;

public:
	AClass(const AAssembly& InAssembly, SourceCodeLocator& Locator);
	virtual ~AClass() noexcept override;

	virtual String GetTypeName() const override;
};