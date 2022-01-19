// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "SharedPtr.h"
#include "GC/Referencer.h"

template<class T>
class WeakPtr
{
	template<class U>
	friend class WeakPtr;

private:
	T* Ptr = nullptr;
	Referencer* ReferencePtr = nullptr;

public:
	inline WeakPtr()
	{
	}

	inline WeakPtr(T* InObject) : Ptr(InObject)
	{
		if (Ptr)
		{
			ReferencePtr = Ptr->ReferencePtr;
			AddWeakRef();
		}
	}

	template<class U> requires (!std::derived_from<U, T>)
	inline explicit WeakPtr(U* InObject) : Ptr(dynamic_cast<U*>(InObject))
	{
		checkf(InObject == nullptr || Ptr, L"U is not convertible to T.");
		if (Ptr)
		{
			ReferencePtr = Ptr->ReferencePtr;
			AddWeakRef();
		}
	}

	inline WeakPtr(WeakPtr&& InMove)
		: Ptr(InMove.Ptr)
		, ReferencePtr(InMove.ReferencePtr)
	{
		InMove.Ptr = nullptr;
		InMove.ReferencePtr = nullptr;
	}

	template<std::derived_from<T> U>
	inline WeakPtr(WeakPtr<U>&& InMove)
		: Ptr(InMove.Ptr)
		, ReferencePtr(InMove.ReferencePtr)
	{
		InMove.Ptr = nullptr;
		InMove.ReferencePtr = nullptr;
	}

	template<class U>
	inline explicit WeakPtr(WeakPtr<U>&& InMove)
		: Ptr(dynamic_cast<T*>(InMove.Ptr))
		, ReferencePtr(InMove.ReferencePtr)
	{
		checkf(InMove.Ptr == nullptr || Ptr, L"U is not convertible to T.");
		InMove.Ptr = nullptr;
		InMove.ReferencePtr = nullptr;
	}

	inline WeakPtr(const WeakPtr& InCopy)
		: Ptr(InCopy.Ptr)
		, ReferencePtr(InCopy.ReferencePtr)
	{
		AddWeakRef();
	}

	template<std::derived_from<T> U>
	inline WeakPtr(const WeakPtr<U>& InCopy)
		: Ptr(InCopy.Ptr)
		, ReferencePtr(InCopy.ReferencePtr)
	{
		AddWeakRef();
	}

	template<class U>
	inline explicit WeakPtr(const WeakPtr<U>& InCopy)
		: Ptr(dynamic_cast<T*>(InCopy.Ptr))
		, ReferencePtr(InCopy.ReferencePtr)
	{
		checkf(InCopy.Ptr == nullptr || Ptr, L"U is not convertible to T.");
		AddWeakRef();
	}

	inline ~WeakPtr() noexcept
	{
		ReleaseWeakRef();
	}

	inline bool IsValid() const
	{
		return ReferencePtr && ReferencePtr->IsValid();
	}

	inline T* Get() const
	{
		return IsValid() ? Ptr : nullptr;
	}

	inline SharedPtr<T> Lock() const
	{
		// Atomic lock operation.
		if (ReferencePtr)
		{
			// Guard for delete.
			bool bNoReferences = ReferencePtr->References++ == 0;

			// Disposed by GC.
			if (bNoReferences && !ReferencePtr->bMarkAtGC)
			{
				--ReferencePtr->References;
				return nullptr;
			}

			SharedPtr<T> AtomicPtr(Ptr);
			--ReferencePtr->References;
			
			return AtomicPtr;
		}

		return nullptr;
	}

	inline WeakPtr& Assign(T* Ptr)
	{
		ReleaseWeakRef();

		this->Ptr = Ptr;
		this->ReferencePtr = Ptr ? Ptr->ReferencePtr : nullptr;

		AddWeakRef();
		return *this;
	}

	template<class U> requires (!std::derived_from<U, T>)
	inline WeakPtr& Assign(U* Ptr)
	{
		ReleaseWeakRef();

		this->Ptr = dynamic_cast<T*>(Ptr);
		this->ReferencePtr = Ptr ? Ptr->ReferencePtr : nullptr;
		checkf(Ptr == nullptr || this->Ptr, L"U is not convertible to T.");

		AddWeakRef();
		return *this;
	}

	inline WeakPtr& Assign(const WeakPtr& Ptr)
	{
		ReleaseWeakRef();

		this->Ptr = Ptr.Ptr;
		this->ReferencePtr = Ptr.ReferencePtr;

		AddWeakRef();
		return *this;
	}

	template<std::derived_from<T> U>
	inline WeakPtr& Assign(const WeakPtr<U>& Ptr)
	{
		ReleaseWeakRef();

		this->Ptr = Ptr.Ptr;
		this->ReferencePtr = Ptr.ReferencePtr;

		AddWeakRef();
		return *this;
	}

	template<class U>
	inline WeakPtr& Assign(const WeakPtr<U>& Ptr)
	{
		ReleaseWeakRef();

		this->Ptr = dynamic_cast<U*>(Ptr.Ptr);
		this->ReferencePtr = Ptr.ReferencePtr;
		checkf(Ptr.Ptr == nullptr || this->Ptr, L"U is not convertible to T.");

		AddWeakRef();
		return *this;
	}

	inline WeakPtr& Assign(WeakPtr&& Ptr)
	{
		ReleaseWeakRef();

		this->Ptr = Ptr.Ptr;
		this->ReferencePtr = Ptr.ReferencePtr;

		Ptr.Ptr = nullptr;
		Ptr.ReferencePtr = nullptr;

		AddWeakRef();
		return *this;
	}

	template<std::derived_from<T> U>
	inline WeakPtr& Assign(WeakPtr<U>&& Ptr)
	{
		ReleaseWeakRef();

		this->Ptr = Ptr.Ptr;
		this->ReferencePtr = Ptr.ReferencePtr;

		Ptr.Ptr = nullptr;
		Ptr.ReferencePtr = nullptr;

		AddWeakRef();
		return *this;
	}

	template<class U>
	inline WeakPtr& Assign(WeakPtr<U>&& Ptr)
	{
		ReleaseWeakRef();

		this->Ptr = dynamic_cast<U*>(Ptr.Ptr);
		this->ReferencePtr = Ptr.ReferencePtr;
		checkf(Ptr.Ptr == nullptr || this->Ptr, L"U is not convertible to T.");

		Ptr.Ptr = nullptr;
		Ptr.ReferencePtr = nullptr;

		AddWeakRef();
		return *this;
	}

	inline WeakPtr& operator =(const WeakPtr& Ptr)
	{
		return Assign(Ptr);
	}

	template<std::derived_from<T> U>
	inline WeakPtr& operator =(const WeakPtr<U>& Ptr)
	{
		return Assign(Ptr);
	}

	inline WeakPtr& operator =(WeakPtr&& Ptr)
	{
		return Assign(std::move(Ptr));
	}

	template<std::derived_from<T> U>
	inline WeakPtr& operator =(WeakPtr<U>&& Ptr)
	{
		return Assign(std::move(Ptr));
	}

	inline operator bool() const
	{
		return IsValid();
	}

	inline auto operator ->() const
	{
		return Get();
	}

	inline auto operator <=>(const WeakPtr&) const = default;

private:
	inline void AddWeakRef()
	{
		if (ReferencePtr)
		{
			++ReferencePtr->WeakReferences;
		}
	}

	inline void ReleaseWeakRef()
	{
		if (ReferencePtr)
		{
			if (--ReferencePtr->WeakReferences == 0)
			{
				delete ReferencePtr;
				ReferencePtr = nullptr;
			}
		}
	}
};