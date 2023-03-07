// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Application/SlateApplication.h"

NSlateApplication::NSlateApplication(NGenericApplication* InApplication)
    : CurrentApp(InApplication)
{
    check(CurrentApp);
}