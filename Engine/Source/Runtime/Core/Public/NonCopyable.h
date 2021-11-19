// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class NonCopyable
{
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator =(const NonCopyable&) = delete;

public:
	NonCopyable()
	{
	}
};