// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <memory>

namespace Ayla
{
	template<class T>
	struct DelegateObjectValidator
	{
		static constexpr bool bSpecialized = false;
		static constexpr bool bCanLock = false;
	};

	template<class T>
	struct DelegateObjectValidator<std::shared_ptr<T>>
	{
		static constexpr bool bSpecialized = true;
		static constexpr bool bCanLock = true;

		using Weak_t = std::weak_ptr<T>;
		using Lock_t = std::shared_ptr<T>;

		static Weak_t GetWeak(const std::shared_ptr<T>& Obj) noexcept(noexcept(Weak_t(Obj)))
		{
			return Obj;
		}

		static Lock_t GetLock(const Weak_t& Obj) noexcept(noexcept(Obj.lock()))
		{
			return Obj.lock();
		}

		static bool IsValid(const Weak_t& Weak) noexcept(noexcept(Weak.expired()))
		{
			return !Weak.expired();
		}

		static bool IsValid(const Lock_t& Lock) noexcept(noexcept((bool)Lock))
		{
			return (bool)Lock;
		}
	};
}