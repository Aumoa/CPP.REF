// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <any>
#include <string_view>
#include <vector>
#include "PrimitiveTypes.h"
#include "CoreAssert.h"

class Type;
class SObject;

class Method
{
	std::function<std::any(SObject*, std::any)> _genericFunction;
	std::wstring _friendlyName;

	uint8 _bMember : 1 = false;
	uint8 _bConst : 1 = false;

private:
	template<class T>
	struct ToFunction
	{
	};

public:
	template<class TRet, class TOwner, class... TArgs>
	Method(TRet(TOwner::* function)(TArgs...), std::wstring_view friendlyName)
		: Method(function, friendlyName, std::make_index_sequence<sizeof...(TArgs)>{})
	{
	}

	template<class TRet, class TOwner, class... TArgs, size_t... _Idx>
	Method(TRet(TOwner::* function)(TArgs...), std::wstring_view friendlyName, std::index_sequence<_Idx...>&&)
		: _friendlyName(friendlyName)
		, _bMember(true)
		, _bConst(false)
	{
		_genericFunction = [function](SObject* caller, std::any pack) -> std::any
		{
			std::tuple pack_tuple = std::any_cast<std::tuple<TArgs...>>(pack);
			if constexpr (std::same_as<TRet, void>)
			{
				(static_cast<TOwner*>(caller)->*function)(std::get<_Idx>(pack_tuple)...);
				return std::any();
			}
			else
			{
				return (static_cast<TOwner*>(caller)->*function)(std::get<_Idx>(pack_tuple)...);
			}
		};
	}

	template<class TRet, class TOwner, class... TArgs>
	Method(TRet(TOwner::* function)(TArgs...) const, std::wstring_view friendlyName)
		: Method(function, friendlyName, std::make_index_sequence<sizeof...(TArgs)>{})
	{
	}

	template<class TRet, class TOwner, class... TArgs, size_t... _Idx>
	Method(TRet(TOwner::* function)(TArgs...) const, std::wstring_view friendlyName, std::index_sequence<_Idx...>&&)
		: _friendlyName(friendlyName)
		, _bMember(true)
		, _bConst(true)
	{
		_genericFunction = [function](SObject* caller, std::any pack) -> std::any
		{
			auto* const_caller = const_cast<const SObject*>(caller);
			std::tuple pack_tuple = std::any_cast<std::tuple<TArgs...>>(pack);
			if constexpr (std::same_as<TRet, void>)
			{
				(static_cast<const TOwner*>(const_caller)->*function)(std::get<_Idx>(pack_tuple)...);
				return std::any();
			}
			else
			{
				return (static_cast<const TOwner*>(const_caller)->*function)(std::get<_Idx>(pack_tuple)...);
			}
		};
	}

	template<class TRet, class... TArgs>
	Method(TRet(*function)(TArgs...), std::wstring_view friendlyName)
		: Method(function, friendlyName, std::make_index_sequence<sizeof...(TArgs)>{})
	{
	}

	template<class TRet, class... TArgs, size_t... _Idx>
	Method(TRet(*function)(TArgs...), std::wstring_view friendlyName, std::index_sequence<_Idx...>&&)
		: _friendlyName(friendlyName)
		, _bMember(false)
		, _bConst(false)
	{
		_genericFunction = [function](SObject* caller, std::any pack) -> std::any
		{
			std::tuple pack_tuple = std::any_cast<std::tuple<TArgs...>>(pack);
			if constexpr (std::same_as<TRet, void>)
			{
				function(std::get<_Idx>(pack_tuple)...);
				return std::any();
			}
			else
			{
				return function(std::get<_Idx>(pack_tuple)...);
			}
		};
	}

	inline bool IsMember() const { return _bMember; }
	inline bool IsConst() const { return _bConst; }
	inline bool IsStatic() const { return !_bMember; }
	inline const std::wstring& GetFriendlyName() const { return _friendlyName; }

	template<class TRet, class TOwner, class... TArgs>
	inline TRet Invoke(TOwner* caller, TArgs&&... args) const
	{
		if constexpr (std::same_as<TRet, void>)
		{
			_genericFunction(caller, std::make_tuple(std::forward<TArgs>(args)...));
		}
		else
		{
			return std::any_cast<TRet>(_genericFunction(caller, std::make_tuple(std::forward<TArgs>(args)...)));
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
			_genericFunction(nullptr, std::make_tuple(std::forward<TArgs>(args)...));
		}
		else
		{
			return _genericFunction(nullptr, std::make_tuple(std::forward<TArgs>(args)...));
		}
	}
};