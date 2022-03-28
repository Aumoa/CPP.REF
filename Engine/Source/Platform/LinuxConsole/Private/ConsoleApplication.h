// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SConsoleApplication : virtual public SObject
{
	GENERATED_BODY(SConsoleApplication)

public:
	static int32 GuardedMain(std::span<const std::wstring> Argv);
};