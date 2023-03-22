// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class TerminateException : public Exception
{
public:
	TerminateException(String InMessage);
};