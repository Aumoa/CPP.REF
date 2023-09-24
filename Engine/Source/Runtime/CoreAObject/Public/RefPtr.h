// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"

template<class T>
class RefPtr final
{
	template<class U>
	friend class RefPtr;

private:
	T* Ptr = nullptr;

public:
	constexpr RefPtr() noexcept = default;
	constexpr RefPtr(std::nullptr_t) noexcept : Ptr(nullptr) {}

	template<std::derived_from<T> U>
	constexpr RefPtr(U* Rhs) noexcept
		: Ptr(Rhs)
	{
		SafeAddRef();
	}

	template<std::derived_from<T> U>
	constexpr RefPtr(RefPtr<U>&& Rhs) noexcept
		: Ptr(Rhs.Ptr)
	{
		Rhs.Ptr = nullptr;
	}

	template<std::derived_from<T> U>
	inline RefPtr(const RefPtr<U>& Rhs) noexcept
		: Ptr(Rhs.Ptr)
	{
		SafeAddRef();
	}

	inline ~RefPtr() noexcept
	{
		SafeDecrRef();
	}

	inline void Reset() noexcept
	{
		SafeDecrRef();
	}

	constexpr T* Get() const noexcept
	{
		return Ptr;
	}

	constexpr void Swap(RefPtr<T>& InPtr) noexcept
	{
		std::swap(Ptr, InPtr.Ptr);
	}

	template<std::derived_from<T> U>
	inline void Assign(const RefPtr<U>& InPtr) noexcept
	{
		SafeDecrRef();
		Ptr = InPtr.Ptr;
		SafeAddRef();
	}

	template<std::derived_from<T> U>
	inline void Assign(RefPtr<U>& InPtr) noexcept
	{
		SafeDecrRef();
		Ptr = InPtr.Ptr;
		InPtr.Ptr = nullptr;
	}

	template<std::derived_from<T> U>
	constexpr auto operator <=>(const RefPtr<U>& Rhs) const noexcept { return Ptr <=> Rhs.Ptr; }
	template<std::derived_from<T> U>
	constexpr bool operator ==(const RefPtr<U>& Rhs) const noexcept { return Ptr == Rhs.Ptr; }

	[[nodiscard]] auto operator ->() const noexcept { return Get(); }
	[[nodiscard]] auto& operator *() const noexcept { return *Get(); }

	constexpr operator bool() const noexcept { return Get() != nullptr; }

	template<class U> requires
		requires { { std::declval<T>() + std::declval<RefPtr<U>>() }; }
	constexpr decltype(auto) operator +(const RefPtr<U>& Rhs) const noexcept(noexcept(std::declval<T>() + std::declval<RefPtr<U>>()))
	{
		return *Ptr + Rhs;
	}

	template<class U> requires
		requires { { std::declval<T>() - std::declval<RefPtr<U>>() }; }
	constexpr decltype(auto) operator -(const RefPtr<U>& Rhs) const noexcept(noexcept(std::declval<T>() - std::declval<RefPtr<U>>()))
	{
		return *Ptr - Rhs;
	}

	template<class U> requires
		requires { { std::declval<T>() * std::declval<RefPtr<U>>() }; }
	constexpr decltype(auto) operator *(const RefPtr<U>& Rhs) const noexcept(noexcept(std::declval<T>() * std::declval<RefPtr<U>>()))
	{
		return *Ptr * Rhs;
	}

	template<class U> requires
		requires { { std::declval<T>() / std::declval<RefPtr<U>>() }; }
	constexpr decltype(auto) operator /(const RefPtr<U>& Rhs) const noexcept(noexcept(std::declval<T>() / std::declval<RefPtr<U>>()))
	{
		return *Ptr / Rhs;
	}

	template<class U> requires
		requires { { std::declval<T>() % std::declval<RefPtr<U>>() }; }
	constexpr decltype(auto) operator %(const RefPtr<U>& Rhs) const noexcept(noexcept(std::declval<T>() % std::declval<RefPtr<U>>()))
	{
		return *Ptr % Rhs;
	}

	template<class U> requires
		requires { { std::declval<T>() ^ std::declval<RefPtr<U>>() }; }
	constexpr decltype(auto) operator ^(const RefPtr<U>& Rhs) const noexcept(noexcept(std::declval<T>() ^ std::declval<RefPtr<U>>()))
	{
		return *Ptr ^ Rhs;
	}

	template<class U> requires
		requires { { std::declval<T>() + std::declval<RefPtr<U>>() }; }
	constexpr RefPtr<T>& operator +=(const RefPtr<U>& Rhs) noexcept(noexcept(std::declval<T>() + std::declval<RefPtr<U>>()))
	{
		Assign(*Ptr + Rhs);
		return *this;
	}

	template<class U> requires
		requires { { std::declval<T>() - std::declval<RefPtr<U>>() }; }
	constexpr RefPtr<T>& operator -=(const RefPtr<U>& Rhs) noexcept(noexcept(std::declval<T>() - std::declval<RefPtr<U>>()))
	{
		Assign(*Ptr - Rhs);
		return *this;
	}

	template<class U> requires
		requires { { std::declval<T>() * std::declval<RefPtr<U>>() }; }
	constexpr RefPtr<T>& operator *=(const RefPtr<U>& Rhs) noexcept(noexcept(std::declval<T>() * std::declval<RefPtr<U>>()))
	{
		Assign(*Ptr * Rhs);
		return *this;
	}

	template<class U> requires
		requires { { std::declval<T>() / std::declval<RefPtr<U>>() }; }
	constexpr RefPtr<T>& operator /=(const RefPtr<U>&Rhs) noexcept(noexcept(std::declval<T>() / std::declval<RefPtr<U>>()))
	{
		Assign(*Ptr / Rhs);
		return *this;
	}

	template<class U> requires
		requires { { std::declval<T>() % std::declval<RefPtr<U>>() }; }
	constexpr RefPtr<T>& operator %=(const RefPtr<U>&Rhs) noexcept(noexcept(std::declval<T>() % std::declval<RefPtr<U>>()))
	{
		Assign(*Ptr % Rhs);
		return *this;
	}

	template<class U> requires
		requires { { std::declval<T>() ^ std::declval<RefPtr<U>>() }; }
	constexpr RefPtr<T>& operator ^=(const RefPtr<U>&Rhs) noexcept(noexcept(std::declval<T>() ^ std::declval<RefPtr<U>>()))
	{
		Assign(*Ptr ^ Rhs);
		return *this;
	}

	template<class U> requires requires { { dynamic_cast<U*>(Ptr) }; }
	constexpr RefPtr<U> Cast() noexcept
	{
		return RefPtr<U>(dynamic_cast<U*>(Ptr));
	}

private:
	inline void SafeAddRef() noexcept
	{
		if (Ptr)
		{
			Ptr->Refs->IncrRef();
		}
	}

	inline void SafeDecrRef() noexcept
	{
		if (Ptr)
		{
			if (Ptr->Refs->DecrRef())
			{
				delete Ptr;
			}
			Ptr = nullptr;
		}
	}
};

template<std::derived_from<AObject> UObject>
inline RefPtr<UObject> NewObject(AType* InClassType = nullptr)
{
	if (InClassType == nullptr)
	{
		InClassType = UObject::StaticClass();
	}

	RefPtr<AObject> Instanced = AObject::NewObject(InClassType);
	RefPtr<UObject> Casted = Instanced.Cast<UObject>();
	check(Casted);
	return Casted;
}