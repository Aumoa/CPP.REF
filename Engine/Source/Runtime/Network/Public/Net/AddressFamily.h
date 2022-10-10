// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "AddressFamily.gen.h"

SENUM()
enum class EAddressFamily
{
    Unspecified = 0,
    InterNetwork = 2,
    Ipx = 6,
    AppleTalk = 16,
    NetBios = 17,
    InterNetworkV6 = 23,
    Irda = 26,
};