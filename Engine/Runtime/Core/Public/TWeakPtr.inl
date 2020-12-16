// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "TRefPtr.h"

namespace SC::Runtime::Core
{
	template<TIsNotPointer T>
	inline TWeakPtr<T>::TWeakPtr()
		: ptr(nullptr)
		, references(nullptr)
	{

	}

	template<TIsNotPointer T>
	inline TWeakPtr<T>::TWeakPtr(std::nullptr_t) : This()
	{

	}

	template<TIsNotPointer T>
	inline TWeakPtr<T>::TWeakPtr(T* ptr) : This()
	{
		this->ptr = ptr;
		if (ptr != nullptr)
		{
			references = ptr->GetWeakReferences();
			references->AddWeakRef();
		}
	}

	template<TIsNotPointer T>
	inline TWeakPtr<T>::TWeakPtr(const T* ptr) : This(const_cast<T*>(ptr))
	{

	}

	template<TIsNotPointer T>
	inline TWeakPtr<T>::TWeakPtr(const TWeakPtr& ptr) : This()
	{
		if (ptr.IsValid)
		{
			this->ptr = ptr.ptr;
			this->references = ptr.references;
			this->references->AddWeakRef();
		}
	}

	template<TIsNotPointer T>
	inline TWeakPtr<T>::TWeakPtr(TWeakPtr&& ptr) : This()
	{
		this->ptr = ptr.ptr;
		this->references = ptr.references;

		ptr.ptr = nullptr;
		ptr.references = nullptr;
	}

	template<TIsNotPointer T>
	inline TWeakPtr<T>::~TWeakPtr()
	{
		SafeRelease();
	}

	template<TIsNotPointer T> template<TIsBaseOf<T> O>
	inline TWeakPtr<T>::TWeakPtr(const TWeakPtr<O>& ptr) : This(ptr.ptr)
	{

	}

	template<TIsNotPointer T> template<TIsBaseOf<T> O>
	inline TWeakPtr<T>::TWeakPtr(TWeakPtr<O>&& ptr) : This()
	{
		this->ptr = ptr.ptr;
		this->references = ptr.references;

		ptr.ptr = nullptr;
		ptr.references = nullptr;
	}

	template<TIsNotPointer T>
	inline void TWeakPtr<T>::Reset(T* ptr)
	{
		SafeRelease();
		Assign(ptr);
	}

	template<TIsNotPointer T>
	inline T* TWeakPtr<T>::Get() const
	{
		if (IsValid)
		{
			return ptr;
		}
		else
		{
			return nullptr;
		}
	}

	template<TIsNotPointer T> template<TIsRefCore O>
	inline bool TWeakPtr<T>::Is(O** ptr) const
	{
		if (!IsValid)
		{
			if (ptr != nullptr)
			{
				*ptr = nullptr;
			}
			return false;
		}

		O* cast = dynamic_cast<O*>(this->ptr);
		if (ptr != nullptr)
		{
			*ptr = cast;
			cast->AddRef();
		}

		return cast != nullptr;
	}

	template<TIsNotPointer T> template<TIsRefCore O>
	inline TWeakPtr<O> TWeakPtr<T>::As() const
	{
		auto ret = TryAs<O>();
		if (!ret.IsValid)
		{
			ThrowInvalidCastException();
		}
		return move(ret);
	}

	template<TIsNotPointer T> template<TIsRefCore O>
	inline TWeakPtr<O> TWeakPtr<T>::TryAs() const
	{
		if (!IsValid)
		{
			return nullptr;
		}

		O* cast = dynamic_cast<O*>(this->ptr);
		if (cast == nullptr)
		{
			return nullptr;
		}

		return ptr;
	}

	template<TIsNotPointer T>
	template<TIsBaseOf<T> O>
	inline TRefPtr<O> TWeakPtr<T>::AsShared() const
	{
		if (IsValid)
		{
			return ptr;
		}
		else
		{
			return nullptr;
		}
	}

	template<TIsNotPointer T>
	inline bool TWeakPtr<T>::IsValid_get() const
	{
		if (ptr == nullptr)
		{
			return false;
		}

		return references->IsValid;
	}

	template<TIsNotPointer T>
	inline T* TWeakPtr<T>::operator ->() const
	{
		return Get();
	}

	template<TIsNotPointer T>
	inline TWeakPtr<T>::operator bool() const
	{
		return IsValid;
	}

	template<TIsNotPointer T>
	inline auto TWeakPtr<T>::operator =(const TWeakPtr& ptr) -> TWeakPtr&
	{
		SafeRelease();
		Assign(ptr);
		return *this;
	}

	template<TIsNotPointer T>
	inline auto TWeakPtr<T>::operator =(TWeakPtr&& ptr) -> TWeakPtr&
	{
		SafeRelease();
		this->ptr = ptr.ptr;
		this->references = ptr.references;
		ptr.ptr = nullptr;
		ptr.references = nullptr;
		return *this;
	}

	template<TIsNotPointer T> template<TIsRefCore O>
	inline bool TWeakPtr<T>::operator ==(const TWeakPtr<O>& ptr) const
	{
		if (!IsValid)
		{
			return !ptr.IsValid;
		}

		else if (!ptr.IsValid)
		{
			return false;
		}

		else
		{
			return this->ptr->operator ==(ptr.ptr);
		}
	}

	template<TIsNotPointer T> template<TIsRefCore O>
	inline bool TWeakPtr<T>::operator !=(const TWeakPtr<O>& ptr) const
	{
		if (!IsValid)
		{
			return ptr.IsValid;
		}

		else if (!ptr.IsValid)
		{
			return true;
		}

		else
		{
			return this->ptr->operator !=(ptr);
		}
	}

	template<TIsNotPointer T> template<class TIndex> requires TIsIndexer<T, TIndex>
	inline auto TWeakPtr<T>::operator [](const TIndex& index) const -> decltype(ptr->operator [](index))
	{
		if (!IsValid)
		{
			ThrowNullReferenceException();
		}

		return ptr->operator [](index);
	}

	template<TIsNotPointer T>
	inline void TWeakPtr<T>::SafeRelease()
	{
		if (IsValid)
		{
			references->ReleaseWeak();
			ptr = nullptr;
			references = nullptr;
		}
	}

	template<TIsNotPointer T>
	inline void TWeakPtr<T>::Assign(T* ptr)
	{
		if (ptr != nullptr)
		{
			this->ptr = ptr;
			this->references = ptr->GetWeakReferences();
			this->references->AddWeakRef();
		}
	}

	template<TIsNotPointer T>
	template<TIsBaseOf<T> O>
	inline TWeakPtr<O> TRefPtr<T>::AsWeak() const
	{
		return ptr;
	}
}