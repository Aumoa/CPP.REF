// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <functional>
#include "PrimitiveTypes.h"

class Type;
class SObject;

class Property
{
	std::wstring _name;
	Type* _memberType = nullptr;
	void (*_setter)(SObject*, const void*);
	const void* (*_getter)(const SObject*);
	uint8 _bIsPointer : 1;
	uint8 _bIsConst : 1;
	uint8 _bIsStatic : 1;

public:
	struct PropertyGenerator
	{
		std::wstring_view Name;
		Type* MemberType = nullptr;
		void (*Setter)(SObject*, const void*);
		const void* (*Getter)(const SObject*);
		uint8 bIsPointer : 1;
		uint8 bIsConst : 1;
		uint8 bIsStatic : 1;
	};

public:
	Property(const PropertyGenerator* generator)
		: _name(generator->Name)
		, _memberType(generator->MemberType)
		, _setter(generator->Setter)
		, _getter(generator->Getter)
		, _bIsPointer(generator->bIsPointer)
		, _bIsConst(generator->bIsConst)
		, _bIsStatic(generator->bIsStatic)
	{
	}

	inline const std::wstring& GetFriendlyName() const { return _name; }
	inline Type* GetMemberType() const { return _memberType; }
	inline bool IsPointer() const { return _bIsPointer; }
	inline bool IsConst() const { return _bIsConst; }
	inline bool IsStatic() const { return _bIsStatic; }

	template<class T>
	void SetValue(SObject* _this, const T& _value) const
	{
		_setter(_this, &_value);
	}

	template<class T>
	T GetValue(const SObject* _this) const
	{
		return *reinterpret_cast<const T*>(_getter(_this));
	}

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