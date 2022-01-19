// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <functional>
#include <optional>
#include "PrimitiveTypes.h"

class Type;
class SObject;

class CORESOBJECT_API Property
{
	std::wstring FriendlyName;
	Type** DeferredMemberType = nullptr;
	void (*Setter)(SObject*, const void*);
	const void* (*Getter)(const SObject*);
	void (*Setter_Struct)(void*, const void*);
	const void* (*Getter_Struct)(const void*);
	uint8 bIsPointer : 1;
	uint8 bIsConst : 1;
	uint8 bIsStatic : 1;

	std::optional<intptr_t> ValOff;
	std::optional<intptr_t> PtrOff;

public:
	struct PropertyGenerator
	{
		std::wstring_view Name;
		Type** DeferredMemberType = nullptr;
		void (*Setter)(SObject*, const void*);
		const void* (*Getter)(const SObject*);
		void (*Setter_Struct)(void*, const void*);
		const void* (*Getter_Struct)(const void*);
		uint8 bIsPointer : 1;
		uint8 bIsConst : 1;
		uint8 bIsStatic : 1;
	};

public:
	Property(const PropertyGenerator* Generator)
		: FriendlyName(Generator->Name)
		, DeferredMemberType(Generator->DeferredMemberType)
		, Setter(Generator->Setter)
		, Getter(Generator->Getter)
		, Setter_Struct(Generator->Setter_Struct)
		, Getter_Struct(Generator->Getter_Struct)
		, bIsPointer(Generator->bIsPointer)
		, bIsConst(Generator->bIsConst)
		, bIsStatic(Generator->bIsStatic)
	{
	}

	inline const std::wstring& GetFriendlyName() const { return FriendlyName; }
	inline bool IsPointer() const { return bIsPointer; }
	inline bool IsConst() const { return bIsConst; }
	inline bool IsStatic() const { return bIsStatic; }
	inline Type* GetMemberType() const { return *DeferredMemberType; }

	template<class T>
	void SetValue(SObject* InThis, const T& InValue)
	{
		Setter(InThis, &InValue);
	}

	template<class T>
	const T& GetValue(SObject* InThis)
	{
		return *reinterpret_cast<const T*>(Internal_GetValue(InThis));
	}

	template<class T, class TStruct>
	void SetValue(TStruct& InThis, const T& InValue)
	{
		Setter_Struct(&InThis, &InValue);
	}

	template<class T, class TStruct>
	const T& GetValue(TStruct& InThis)
	{
		return *reinterpret_cast<const T*>(Getter_Struct(&InThis));
	}

	void SetObject(SObject* InThis, SObject* AssignValue);
	SObject* GetObject(SObject* InThis);
	
	template<class TStruct>
	void SetObject(TStruct& InThis, SObject* AssignValue)
	{
		Internal_SetObject(&InThis, AssignValue);
	}

	template<class TStruct>
	SObject* GetObject(const TStruct& InThis)
	{
		return Internal_GetObject(&InThis);
	}

private:
	void* Internal_GetValue(SObject* InThis);
	void Internal_SetObject(void* InThis, SObject* AssignValue);
	SObject* Internal_GetObject(const void* InThis);
};