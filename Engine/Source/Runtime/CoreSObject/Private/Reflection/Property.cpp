// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Reflection/Property.h"
#include "Reflection/Type.h"

void Property::SetObject(SObject* InThis, SObject* AssignValue)
{
	SetValue(InThis, reinterpret_cast<int64>(GetMemberType()->FromObject(AssignValue)));
}

SObject* Property::GetObject(SObject* InThis)
{
	uint8* Ptr = reinterpret_cast<uint8*>(GetValue<uint64>(InThis));
	SObject* ObjPtr = GetMemberType()->ToObject(Ptr);
	int64 PtrOff = reinterpret_cast<uint8*>(ObjPtr) - Ptr;
	void* ShiftPtr = Ptr ? Ptr + PtrOff : Ptr;
	return reinterpret_cast<SObject*>(ShiftPtr);
}

void* Property::Internal_GetValue(SObject* InThis)
{
	const uint8* MemberPtr = reinterpret_cast<const uint8*>(Getter(InThis));
	int64 ValOff = MemberPtr - reinterpret_cast<uint8*>(InThis);
	return reinterpret_cast<uint8*>(InThis) + ValOff;
}

void Property::Internal_SetObject(void* InThis, SObject* AssignValue)
{
	Setter_Struct(InThis, GetMemberType()->FromObject(AssignValue));
}

SObject* Property::Internal_GetObject(const void* InThis)
{
	void** Ptr = reinterpret_cast<void**>(const_cast<void*>(Getter_Struct(InThis)));
	return GetMemberType()->ToObject(*Ptr);
}