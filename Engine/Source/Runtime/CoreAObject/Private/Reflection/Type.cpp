// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Reflection/Type.h"
#include "Reflection/ObjectMacros.h"

AYLA_DEFINE_CLASS_INFO(Engine, CoreAObject, Type);
AYLA_DEFINE_STATIC_CLASS_FUNCTION(Engine, CoreAObject, Type);

AType::AType() : AObject(std::in_place)
{
}

AType::~AType() noexcept
{
}