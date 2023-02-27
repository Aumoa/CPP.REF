// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericApplication.h"

class SLATECORE_API NSlateApplication
{
    std::shared_ptr<NGenericApplication> CurrentApp;

public:
    NSlateApplication(std::shared_ptr<NGenericApplication> InApplication);
};