// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Attributes/ClassAttribute.h"

using namespace libty;

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