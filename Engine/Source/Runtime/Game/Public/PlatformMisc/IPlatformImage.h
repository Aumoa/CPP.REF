// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/IRHIImageSourceView.h"

struct GAME_API IPlatformImage : virtual public Object, virtual public IRHIImageSourceView
{
};