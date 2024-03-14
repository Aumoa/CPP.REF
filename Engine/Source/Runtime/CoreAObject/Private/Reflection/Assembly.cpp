// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Reflection/Assembly.h"
#include "Reflection/ObjectMacros.h"

AYLA_DEFINE_CLASS_INFO(Engine, CoreAObject, Assembly);
AYLA_DEFINE_STATIC_CLASS_FUNCTION(Engine, CoreAObject, Assembly);

AAssembly::AAssembly() : AObject(std::in_place)
{
}

AAssembly::~AAssembly() noexcept
{
}