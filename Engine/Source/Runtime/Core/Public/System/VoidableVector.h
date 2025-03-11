// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <vector>

template<class T>
class VoidableVector : public std::vector<T>
{
public:
	template<class... TArgs> requires std::constructible_from<std::vector<T>, TArgs...>
	constexpr VoidableVector(TArgs&&... Args) noexcept(noexcept(std::vector<T>(std::forward<TArgs>(Args)...)))
		: std::vector<T>(std::forward<TArgs>(Args)...)
	{
	}

	constexpr VoidableVector& operator =(const VoidableVector& R) const noexcept(noexcept(std::declval<std::vector<T>&>() = std::declval<const std::vector<T>&>()))
	{
		std::vector<T>::operator =(R);
		return *this;
	}

	constexpr VoidableVector&& operator =(VoidableVector&& R) const noexcept(noexcept(std::declval<std::vector<T>&>() = std::declval<std::vector<T>&&>()))
	{
		std::vector<T>::operator =(std::move(R));
		return *this;
	}
};

template<>
class VoidableVector<void>
{
	size_t Size = 0;

public:
	constexpr VoidableVector() noexcept = default;
	constexpr VoidableVector(size_t) noexcept {}
	constexpr VoidableVector(const VoidableVector&) noexcept = default;
	constexpr VoidableVector(VoidableVector&&) noexcept = default;

	constexpr VoidableVector& operator =(const VoidableVector&) noexcept = default;
	constexpr VoidableVector& operator =(VoidableVector&&) noexcept = default;

	constexpr void resize(size_t NewSize) noexcept { Size = NewSize; }
	constexpr size_t size() const noexcept { return Size; }
};