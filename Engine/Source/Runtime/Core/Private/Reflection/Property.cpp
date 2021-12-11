// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Reflection/Property.h"
#include "Reflection/Type.h"

void Property::SetObject(SObject* InThis, SObject* AssignValue) const
{
	SetValue(InThis, reinterpret_cast<int64>(GetMemberType()->FromObject(AssignValue)));
}

SObject* Property::GetObject(SObject* InThis)
{
	uint8* Ptr = reinterpret_cast<uint8*>(GetValue<intptr_t>(InThis));

	if (!PtrOff.has_value() && Ptr)
	{
		SObject* ObjPtr = GetMemberType()->ToObject(Ptr);
		PtrOff = reinterpret_cast<uint8*>(ObjPtr) - Ptr;
	}

	void* ShiftPtr = Ptr ? Ptr + *PtrOff : Ptr;
	return reinterpret_cast<SObject*>(ShiftPtr);
}

void* Property::Internal_GetValue(SObject* InThis)
{
	if (!ValOff.has_value())
	{
		const uint8* MemberPtr = reinterpret_cast<const uint8*>(Getter(InThis));
		ValOff = MemberPtr - reinterpret_cast<uint8*>(InThis);
	}

	return reinterpret_cast<uint8*>(InThis) + *ValOff;
}