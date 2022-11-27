// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <memory>
#include <variant>
#include "CoreAssert.h"

template<class T>
class PolPtr
{
	std::variant<T*, std::unique_ptr<T>, std::shared_ptr<T>> _ptr;

public:
	PolPtr() noexcept = default;
	PolPtr(const PolPtr&) = delete;
	PolPtr(PolPtr&&) noexcept = default;

	PolPtr(T* ptr) noexcept
		: _ptr(ptr)
	{
	}

	PolPtr(std::unique_ptr<T> ptr) noexcept
		: _ptr(std::move(ptr))
	{
	}

	PolPtr(std::shared_ptr<T> ptr) noexcept
		: _ptr(std::move(ptr))
	{
	}

	const T* get() const noexcept { return const_cast<PolPtr*>(this)->get(); }
	T* get() noexcept
	{
		switch (_ptr.index())
		{
		case 0:
			return std::get<0>(_ptr);
		case 1:
			return std::get<1>(_ptr).get();
		case 2:
			return std::get<2>(_ptr).get();
		default:
			return nullptr;
		}
	}

	const T* operator ->() const noexcept { auto p = get(); check(p); return p; }
	T* operator ->() noexcept { auto p = get(); check(p); return p; }
	operator bool() const noexcept { return get() != nullptr; }

	PolPtr& operator =(const PolPtr&) = delete;
	PolPtr& operator =(PolPtr&&) noexcept = default;
};