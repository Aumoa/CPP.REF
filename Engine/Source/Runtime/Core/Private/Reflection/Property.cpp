// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Reflection/Property.h"
#include "Reflection/Type.h"

void Property::SetObject(SObject* InThis, SObject* AssignValue) const
{
	SetValue(InThis, reinterpret_cast<int64>(_memberType->FromObject(AssignValue)));
}

SObject* Property::GetObject(SObject* InThis) const
{
	return _memberType->ToObject(reinterpret_cast<void*>(GetValue<intptr_t>(InThis)));
}