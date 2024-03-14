// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Reflection/ParameterInfo.h"
#include "Reflection/ObjectMacros.h"

AYLA_DEFINE_CLASS_INFO(Engine, CoreAObject, ParameterInfo);
AYLA_DEFINE_STATIC_CLASS_FUNCTION(Engine, CoreAObject, ParameterInfo);

AParameterInfo::AParameterInfo() : AObject(std::in_place)
{
}

AParameterInfo::~AParameterInfo() noexcept
{
}