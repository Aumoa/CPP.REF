// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Exception.h"

class CORE_API SObjectDisposedException : public SException
{
	GENERATED_BODY(SObjectDisposedException)

public:
	SObjectDisposedException();
	SObjectDisposedException(std::wstring_view ObjectName);
};