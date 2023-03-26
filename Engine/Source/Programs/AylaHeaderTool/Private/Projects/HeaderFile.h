// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Projects/SourceCode.h"
#include "Reflection/AType.h"

class AAssembly;

class HeaderFile : public SourceCode
{
	std::vector<std::unique_ptr<AType>> Types;

public:
	HeaderFile(String SourceFile, String SourceCode);

	bool Parse(const AAssembly& InAssembly);
};