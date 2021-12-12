// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"

template<class T>
class SharedPtr
{
	template<class U>
	friend class SharedPtr;

private:
	T* Ptr = nullptr;

public:
	inline SharedPtr()
	{
	}

	inline SharedPtr(T* InPtr) : Ptr(InPtr)
	{
		AddRef();
	}

	template<class U> requires (!std::derived_from<U, T>)
	inline explicit SharedPtr(U* InObject) : Ptr(dynamic_cast<U*>(Ptr))
	{
		checkf(InObject == nullptr || Ptr, L"U is not convertible to T.");
		AddRef();
	}

	inline SharedPtr(SharedPtr&& InMove) : Ptr(InMove.Ptr)
	{
		InMove.Ptr = nullptr;
	}

	template<std::derived_from<T> U>
	inline SharedPtr(SharedPtr<U>&& InMove) : Ptr(InMove.Ptr)
	{
		InMove.Ptr = nullptr;
	}

	template<class U>
	inline explicit SharedPtr(SharedPtr<U>&& InMove) : Ptr(dynamic_cast<T*>(InMove.Ptr))
	{
		checkf(InMove.Ptr == nullptr || Ptr, L"U is not convertible to T.");
		InMove.Ptr = nullptr;
	}

	inline SharedPtr(const SharedPtr& InCopy) : Ptr(InCopy.Ptr)
	{
		AddRef();
	}

	template<std::derived_from<T> U>
	inline SharedPtr(const SharedPtr<U>& InCopy) : Ptr(InCopy.Ptr)
	{
		AddRef();
	}

	template<class U>
	inline explicit SharedPtr(const SharedPtr<U>& InCopy) : Ptr(InCopy.Ptr)
	{
		checkf(InCopy.Ptr == nullptr || Ptr, L"U is not convertible to T.");
		AddRef();
	}

	inline ~SharedPtr() noexcept
	{
		ReleaseRef();
	}

	inline bool IsValid() const
	{
		return Ptr != nullptr;
	}

	inline T* Get() const
	{
		return IsValid() ? Ptr : nullptr;
	}

	inline SharedPtr& Assign(T* Ptr)
	{
		if (Ptr != this->Ptr)
		{
			ReleaseRef();
			this->Ptr = Ptr;
			AddRef();
		}
		return *this;
	}

	template<class U> requires (!std::derived_from<U, T>)
	inline SharedPtr& Assign(U* Ptr)
	{
		T* NewPtr = dynamic_cast<T*>(Ptr);
		checkf(Ptr == nullptr || NewPtr, L"U is not convertible to T.");

		if (Ptr != NewPtr)
		{
			ReleaseRef();
			this->Ptr = NewPtr;
			AddRef();
		}
		return *this;
	}

	inline SharedPtr& Assign(const SharedPtr& Ptr)
	{
		return Assign(Ptr.Get());
	}

	template<class U>
	inline SharedPtr& Assign(const SharedPtr<U>& Ptr)
	{
		return Assign(Ptr.Get());
	}

	inline SharedPtr& Assign(SharedPtr&& Ptr)
	{
		if (Ptr.Ptr != this->Ptr)
		{
			ReleaseRef();
			this->Ptr = Ptr.Ptr;
			Ptr.Ptr = nullptr;
		}
		return *this;
	}

	template<std::derived_from<T> U>
	inline SharedPtr& Assign(SharedPtr<U>&& Ptr)
	{
		if (Ptr.Ptr != this->Ptr)
		{
			ReleaseRef();
			this->Ptr = Ptr.Ptr;
			Ptr.Ptr = nullptr;
		}
		return *this;
	}

	template<class U>
	inline SharedPtr& Assign(SharedPtr<U>&& Ptr)
	{
		T* NewPtr = dynamic_cast<T*>(Ptr.Ptr);
		checkf(Ptr.Ptr == nullptr || NewPtr, L"U is not convertible to T.");

		if (NewPtr != this->Ptr)
		{
			ReleaseRef();
			this->Ptr = NewPtr;
			Ptr.Ptr = nullptr;
		}
		return *this;
	}

	inline SharedPtr& operator =(const SharedPtr& Ptr)
	{
		return Assign(Ptr);
	}

	template<std::derived_from<T> U>
	inline SharedPtr& operator =(const SharedPtr<U>& Ptr)
	{
		return Assign(Ptr);
	}

	inline SharedPtr& operator =(SharedPtr&& Ptr)
	{
		return Assign(std::move(Ptr));
	}

	template<std::derived_from<T> U>
	inline SharedPtr& operator =(SharedPtr<U>&& Ptr)
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

	inline auto operator <=>(const SharedPtr&) const = default;

private:
	void AddRef()
	{
		if (Ptr)
		{
			const bool bNoReferences = Ptr->ReferencePtr->References++ == 0;
			if (bNoReferences)
			{
				Ptr->AddToRoot();
			}
		}
	}

	void ReleaseRef()
	{
		if (Ptr)
		{
			bool bNoReferences = --Ptr->ReferencePtr->References == 0;
			if (bNoReferences)
			{
				Ptr->RemoveFromRoot();

				if (!Ptr->ReferencePtr->bMarkAtGC)
				{
					delete Ptr;
					Ptr = nullptr;
				}
			}
		}
	}
};