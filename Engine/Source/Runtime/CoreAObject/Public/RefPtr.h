// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "SubclassOf.h"

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
	constexpr auto operator <=>(const T* Rhs) const noexcept { return Ptr <=> Rhs; }

	template<std::derived_from<T> U>
	constexpr bool operator ==(const RefPtr<U>& Rhs) const noexcept { return Ptr == Rhs.Ptr; }
	constexpr bool operator ==(const T* Rhs) const noexcept { return Ptr == Rhs; }

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
		if (auto uPtr = RefPtr<U>(dynamic_cast<U*>(Ptr)))
		{
			uPtr.Ptr->Refs->IncrRef();
			return uPtr;
		}
		return nullptr;
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

template<class T>
class WeakPtr
{
	T* Ptr = nullptr;
	Referencer* Refs = nullptr;

public:
	inline WeakPtr() noexcept
	{
	}

	inline WeakPtr(std::nullptr_t) noexcept
	{
	}

	inline WeakPtr(T* InPtr) noexcept
	{
		Reset(InPtr);
	}

	inline WeakPtr(const WeakPtr& InPtr) noexcept
	{
		Reset(InPtr.Get());
	}

	inline WeakPtr(WeakPtr&& InPtr) noexcept
	{
		Swap(InPtr);
	}

	inline ~WeakPtr() noexcept
	{
		Reset();
	}

	inline bool IsValid() const noexcept
	{
		if (Refs == nullptr)
		{
			return false;
		}

		return Refs->IsLived();
	}

	inline RefPtr<T> Lock() const noexcept
	{
		if (Refs == nullptr)
		{
			return nullptr;
		}

		if (Refs->TryIncrRef() == false)
		{
			return nullptr;
		}

		return RefPtr<T>(Ptr);
	}

	inline WeakPtr& Reset(T* Rhs = nullptr) noexcept
	{
		// Same pointer.
		if (Ptr == Rhs)
		{
			return *this;
		}

		// Unreference pointer.
		if (Refs)
		{
			Refs->DecrWeak();
			Refs = nullptr;
			Ptr = nullptr;
		}

		if (Rhs != nullptr)
		{
			Refs = Rhs->Refs;
			Refs->IncrWeak();
			Ptr = Rhs;
		}

		return *this;
	}

	inline WeakPtr& Swap(WeakPtr& Rhs) noexcept
	{
		std::swap(Ptr, Rhs.Ptr);
		std::swap(Refs, Rhs.Refs);
		return *this;
	}

	inline operator bool() const noexcept
	{
		return IsValid();
	}

	inline WeakPtr& operator =(const WeakPtr& Rhs) noexcept
	{
		return Reset(Rhs.Get());
	}

	inline WeakPtr& operator =(WeakPtr&& Rhs) noexcept
	{
		Reset();
		return Swap(Rhs);
	}
};

template<class T>
WeakPtr(T*) -> WeakPtr<T>;

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

template<std::derived_from<AObject> UObject>
inline RefPtr<UObject> NewObject(TSubclassOf<UObject> InClassType)
{
	return NewObject(InClassType.GetType());
}