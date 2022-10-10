// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPAddress.h"

struct EndPoint
{
	IPAddress Address;
	uint16 Port;
};