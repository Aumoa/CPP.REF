// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Rect.h"

struct NGenericWindowDefinition
{
    uint8 bPopup : 1 = false;
    uint8 bPrimaryWindow : 1 = false;
    Vector2N DesiredScreenPosition = Vector2N(-1, -1);
    Vector2N DesiredScreenSize = Vector2N(-1, -1);
};