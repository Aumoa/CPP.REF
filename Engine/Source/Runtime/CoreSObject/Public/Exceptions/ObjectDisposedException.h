// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

class CORESOBJECT_API SObjectDisposedException : public SException
{
	GENERATED_BODY(SObjectDisposedException)

public:
	SObjectDisposedException();
	SObjectDisposedException(std::wstring_view ObjectName);
};