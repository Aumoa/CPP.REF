// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Application/SlateApplication.h"

NSlateApplication::NSlateApplication(std::shared_ptr<NGenericApplication> InApplication)
    : CurrentApp(std::move(InApplication))
{
    check(CurrentApp);
}