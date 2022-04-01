// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Attributes/ClassAttribute.h"
#include "Reflection/Type.h"
#include "Reflection/TypeInfoMetadataGenerator.Impl.h"

GENERATE_BODY(SClassAttribute);

SClassAttribute::SClassAttribute()
	: Super()
{
}

void SClassAttribute::SetBindType(SType* bindType)
{
	_bindType = bindType;
}

SType* SClassAttribute::GetBindType()
{
	return _bindType;
}