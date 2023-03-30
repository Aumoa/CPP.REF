// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class CompileErrors
{
	CompileErrors() = delete;

public:
	static String InvalidClassKey(String InClassKey, String InRequired);
	static String IllegalScope(bool bOpen);
	static String GeneratedBody();
	static String Colon();
};