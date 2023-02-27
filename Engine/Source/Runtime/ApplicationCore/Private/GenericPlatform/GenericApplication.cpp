// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GenericPlatform/GenericApplication.h"

NGenericApplication* NGenericApplication::sApp;

NGenericApplication::NGenericApplication()
{
    check(!sApp);
    sApp = this;
}

NGenericApplication::~NGenericApplication() noexcept
{
    check(sApp);
    sApp = nullptr;
}