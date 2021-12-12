// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <functional>
#include <optional>
#include "PrimitiveTypes.h"

class Type;
class SObject;

class CORE_API Property
{
	std::wstring FriendlyName;
	Type** DeferredMemberType = nullptr;
	void (*Setter)(SObject*, const void*);
	const void* (*Getter)(const SObject*);
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
	void SetValue(SObject* InThis, const T& OutValue) const
	{
		Setter(InThis, &OutValue);
	}

	template<class T>
	const T& GetValue(SObject* InThis)
	{
		return *reinterpret_cast<const T*>(Internal_GetValue(InThis));
	}

	void SetObject(SObject* InThis, SObject* AssignValue) const;
	SObject* GetObject(SObject* InThis);

private:
	void* Internal_GetValue(SObject* InThis);
};