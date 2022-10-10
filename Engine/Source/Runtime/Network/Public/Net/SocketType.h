// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SocketType.gen.h"

SENUM()
enum class ESocketType
{
    Unknown = -1,
    Stream = 1,
    Dgram = 2,
    Raw = 3,
    Rdm = 4,
    Seqpacket = 5
};