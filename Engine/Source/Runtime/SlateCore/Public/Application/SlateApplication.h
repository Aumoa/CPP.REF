// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericApplication.h"

class SLATECORE_API NSlateApplication
{
    NGenericApplication* CurrentApp;

public:
    NSlateApplication(NGenericApplication* InApplication);
};