// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Attributes/ClassAttribute.h"

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