// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <functional>
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
	void SetValue(SObject* _this, const T& _value) const
	{
		Setter(_this, &_value);
	}

	template<class T>
	const T& GetValue(SObject* _this) const
	{
		return *reinterpret_cast<const T*>(Getter(_this));
	}

	void SetObject(SObject* InThis, SObject* AssignValue) const;
	SObject* GetObject(SObject* InThis) const;

	template<class T>
	consteval static bool __Internal_IsStaticMember();
	template<class T>
	consteval static bool __Internal_IsStaticMember(T*) { return true; }
	template<class TOwner, class T>
	consteval static bool __Internal_IsStaticMember(T(TOwner::*)) { return false; }
	template<class T>
	consteval static bool __Internal_IsStaticMember(const T*) { return true; }
	template<class TOwner, class T>
	consteval static bool __Internal_IsStaticMember(const T(TOwner::*)) { return false; }
};