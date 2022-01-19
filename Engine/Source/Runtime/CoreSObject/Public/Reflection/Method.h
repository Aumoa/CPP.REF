// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <any>
#include <string_view>
#include <vector>
#include "PrimitiveTypes.h"
#include "CoreAssert.h"

class SObject;
class Type;

class Method
{
	std::function<std::any(SObject*, std::any)> GenericFunction;
	std::wstring FriendlyName;

	uint8 bMember : 1 = false;
	uint8 bConst : 1 = false;

	Type* ReturnT = nullptr;
	std::vector<Type*> Parameters;

private:
	// Defined in Type.h
	template<bool bRet, class T, class... TArgs>
	void ProcessParameters();

public:
	template<class TRet, class TOwner, class... TArgs>
	Method(TRet(TOwner::* function)(TArgs...), std::wstring_view friendlyName)
		: Method(function, friendlyName, std::make_index_sequence<sizeof...(TArgs)>{})
	{
	}

private:
	template<class TRet, class TOwner, class... TArgs, size_t... _Idx>
	Method(TRet(TOwner::* function)(TArgs...), std::wstring_view friendlyName, std::index_sequence<_Idx...>&&)
		: FriendlyName(friendlyName)
		, bMember(true)
		, bConst(false)
	{
		ProcessParameters<true, TRet, TArgs...>();
		GenericFunction = [function](SObject* caller, std::any pack) -> std::any
		{
			std::tuple pack_tuple = std::any_cast<std::tuple<TArgs...>>(pack);
			if constexpr (std::same_as<TRet, void>)
			{
				(dynamic_cast<TOwner*>(caller)->*function)(std::get<_Idx>(pack_tuple)...);
				return std::any();
			}
			else if constexpr (std::derived_from<std::remove_pointer_t<TRet>, SObject>)
			{
				return static_cast<SObject*>((dynamic_cast<TOwner*>(caller)->*function)(std::get<_Idx>(pack_tuple)...));
			}
			else
			{
				return (dynamic_cast<TOwner*>(caller)->*function)(std::get<_Idx>(pack_tuple)...);
			}
		};
	}

public:
	template<class TRet, class TOwner, class... TArgs>
	Method(TRet(TOwner::* function)(TArgs...) const, std::wstring_view friendlyName)
		: Method(function, friendlyName, std::make_index_sequence<sizeof...(TArgs)>{})
	{
	}

private:
	template<class TRet, class TOwner, class... TArgs, size_t... _Idx>
	Method(TRet(TOwner::* function)(TArgs...) const, std::wstring_view friendlyName, std::index_sequence<_Idx...>&&)
		: FriendlyName(friendlyName)
		, bMember(true)
		, bConst(true)
	{
		ProcessParameters<true, TRet, TArgs...>();
		GenericFunction = [function](SObject* caller, std::any pack) -> std::any
		{
			auto* const_caller = const_cast<const SObject*>(caller);
			std::tuple pack_tuple = std::any_cast<std::tuple<TArgs...>>(pack);
			if constexpr (std::same_as<TRet, void>)
			{
				(dynamic_cast<const TOwner*>(const_caller)->*function)(std::get<_Idx>(pack_tuple)...);
				return std::any();
			}
			else if constexpr (std::derived_from<std::remove_pointer_t<TRet>, SObject>)
			{
				return static_cast<SObject*>((dynamic_cast<const TOwner*>(const_caller)->*function)(std::get<_Idx>(pack_tuple)...));
			}
			else
			{
				return (dynamic_cast<const TOwner*>(const_caller)->*function)(std::get<_Idx>(pack_tuple)...);
			}
		};
	}

public:
	template<class TRet, class... TArgs>
	Method(TRet(*function)(TArgs...), std::wstring_view friendlyName)
		: Method(function, friendlyName, std::make_index_sequence<sizeof...(TArgs)>{})
	{
	}

private:
	template<std::derived_from<SObject> T>
	static SObject* ToArgument(T* ObjectPtr, int)
	{
		return static_cast<SObject*>(ObjectPtr);
	}

	template<class T>
	static T&& ToArgument(T&& NativeValue, short)
	{
		return std::forward<T>(NativeValue);
	}

	template<class T >
	static T FromReturn(SObject* ReturnPtr)
	{
		return dynamic_cast<T>(ReturnPtr);
	}

private:
	template<class TRet, class... TArgs, size_t... _Idx>
	Method(TRet(*function)(TArgs...), std::wstring_view friendlyName, std::index_sequence<_Idx...>&&)
		: FriendlyName(friendlyName)
		, bMember(false)
		, bConst(false)
	{
		ProcessParameters<true, TRet, TArgs...>();
		GenericFunction = [function](SObject* caller, std::any pack) -> std::any
		{
			std::tuple pack_tuple = std::any_cast<std::tuple<TArgs...>>(pack);
			if constexpr (std::same_as<TRet, void>)
			{
				function(std::get<_Idx>(pack_tuple)...);
				return std::any();
			}
			else if constexpr (std::derived_from<std::remove_pointer_t<TRet>, void>)
			{
				return static_cast<SObject*>(function(std::get<_Idx>(pack_tuple)...));
			}
			else
			{
				return function(std::get<_Idx>(pack_tuple)...);
			}
		};
	}

public:
	inline bool IsMember() const { return bMember; }
	inline bool IsConst() const { return bConst; }
	inline bool IsStatic() const { return !bMember; }
	inline const std::wstring& GetFriendlyName() const { return FriendlyName; }

	inline const std::vector<Type*>& GetParameterTypes() const { return Parameters; }
	inline Type* GetParameterType(size_t Index) const { return Parameters[Index]; }
	inline Type* GetReturnType() { return ReturnT; }

	template<class TRet, class TOwner, class... TArgs>
	inline TRet Invoke(TOwner* caller, TArgs&&... args) const
	{
		if constexpr (std::same_as<TRet, void>)
		{
			GenericFunction(caller, std::make_tuple(std::forward<TArgs>(ToArgument<TArgs>(std::forward<TArgs>(args), 0))...));
		}
		else if constexpr (std::derived_from<std::remove_pointer_t<TRet>, SObject>)
		{
			return FromReturn<TRet>(std::any_cast<SObject*>(GenericFunction(caller, std::make_tuple(std::forward<TArgs>(ToArgument<TArgs>(std::forward<TArgs>(args), 0))...))));
		}
		else
		{
			return std::any_cast<TRet>(GenericFunction(caller, std::make_tuple(std::forward<TArgs>(ToArgument<TArgs>(std::forward<TArgs>(args), 0))...)));
		}
	}

	template<class TRet, class TOwner, class... TArgs>
	inline TRet Invoke(const TOwner* caller, TArgs&&... args) const
	{
		check(IsConst());
		return Invoke<TRet, TOwner, TArgs...>(const_cast<TOwner*>(caller), std::forward<TArgs>(args)...);
	}

	template<class TRet, class... TArgs>
	inline TRet Invoke(nullptr_t, TArgs&&... args) const
	{
		if constexpr (std::same_as<TRet, void>)
		{
			GenericFunction(nullptr, std::make_tuple(std::forward<TArgs>(ToArgument<TArgs>(std::forward<TArgs>(args), 0))...));
		}
		else if constexpr (std::derived_from<std::remove_pointer_t<TRet>, SObject>)
		{
			return FromReturn<TRet>(std::any_cast<SObject*>(GenericFunction(nullptr, std::make_tuple(std::forward<TArgs>(ToArgument<TArgs>(std::forward<TArgs>(args), 0))...))));
		}
		else
		{
			return GenericFunction(nullptr, std::make_tuple(std::forward<TArgs>(ToArgument<TArgs>(std::forward<TArgs>(args), 0))...));
		}
	}
};