// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "TRefPtr.h"

template<TIsNotPointer T, bool bThreadSafe>
inline TWeakPtr<T, bThreadSafe>::TWeakPtr()
	: ptr(nullptr)
	, references(nullptr)
{

}

template<TIsNotPointer T, bool bThreadSafe>
inline TWeakPtr<T, bThreadSafe>::TWeakPtr(std::nullptr_t) : This()
{

}

template<TIsNotPointer T, bool bThreadSafe>
inline TWeakPtr<T, bThreadSafe>::TWeakPtr(T* ptr) : This()
{
	this->ptr = ptr;
	if (ptr != nullptr)
	{
		references = ptr->GetWeakReferences();
		references->AddWeakRef();
	}
}

template<TIsNotPointer T, bool bThreadSafe>
inline TWeakPtr<T, bThreadSafe>::TWeakPtr(const T* ptr) : This(const_cast<T*>(ptr))
{

}

template<TIsNotPointer T, bool bThreadSafe>
inline TWeakPtr<T, bThreadSafe>::TWeakPtr(const TWeakPtr& ptr) : This()
{
	if (ptr.IsValid_get())
	{
		this->ptr = ptr.ptr;
		this->references = ptr.references;
		this->references->AddWeakRef();
	}
}

template<TIsNotPointer T, bool bThreadSafe>
inline TWeakPtr<T, bThreadSafe>::TWeakPtr(TWeakPtr&& ptr) : This()
{
	this->ptr = ptr.ptr;
	this->references = ptr.references;

	ptr.ptr = nullptr;
	ptr.references = nullptr;
}

template<TIsNotPointer T, bool bThreadSafe>
inline TWeakPtr<T, bThreadSafe>::~TWeakPtr()
{
	SafeRelease();
}

template<TIsNotPointer T, bool bThreadSafe> template<TIsBaseOf<T> O>
inline TWeakPtr<T, bThreadSafe>::TWeakPtr(const TWeakPtr<O, bThreadSafe>& ptr) : This(ptr.ptr)
{

}

template<TIsNotPointer T, bool bThreadSafe> template<TIsBaseOf<T> O>
inline TWeakPtr<T, bThreadSafe>::TWeakPtr(TWeakPtr<O, bThreadSafe>&& ptr) : This()
{
	this->ptr = ptr.ptr;
	this->references = ptr.references;

	ptr.ptr = nullptr;
	ptr.references = nullptr;
}

template<TIsNotPointer T, bool bThreadSafe>
inline void TWeakPtr<T, bThreadSafe>::Reset(T* ptr)
{
	SafeRelease();
	Assign(ptr);
}

template<TIsNotPointer T, bool bThreadSafe>
inline T* TWeakPtr<T, bThreadSafe>::Get() const
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

template<TIsNotPointer T, bool bThreadSafe> template<TIsRefCore O>
inline bool TWeakPtr<T, bThreadSafe>::Is(O** ptr) const
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

template<TIsNotPointer T, bool bThreadSafe> template<TIsRefCore O>
inline TWeakPtr<O, bThreadSafe> TWeakPtr<T, bThreadSafe>::As() const
{
	auto ret = TryAs<O>();
	if (!ret.IsValid_get())
	{
		ThrowInvalidCastException();
	}
	return move(ret);
}

template<TIsNotPointer T, bool bThreadSafe> template<TIsRefCore O>
inline TWeakPtr<O, bThreadSafe> TWeakPtr<T, bThreadSafe>::TryAs() const
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

template<TIsNotPointer T, bool bThreadSafe>
template<TIsBaseOf<T> O>
inline TRefPtr<O, bThreadSafe> TWeakPtr<T, bThreadSafe>::AsShared() const
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

template<TIsNotPointer T, bool bThreadSafe>
inline bool TWeakPtr<T, bThreadSafe>::IsValid_get() const
{
	if (ptr == nullptr)
	{
		return false;
	}

	return references->IsValid;
}

template<TIsNotPointer T, bool bThreadSafe>
inline T* TWeakPtr<T, bThreadSafe>::operator ->() const
{
	return Get();
}

template<TIsNotPointer T, bool bThreadSafe>
inline TWeakPtr<T, bThreadSafe>::operator bool() const
{
	return IsValid;
}

template<TIsNotPointer T, bool bThreadSafe>
inline auto TWeakPtr<T, bThreadSafe>::operator =(const TWeakPtr& ptr) -> TWeakPtr&
{
	SafeRelease();
	Assign(ptr);
	return *this;
}

template<TIsNotPointer T, bool bThreadSafe>
inline auto TWeakPtr<T, bThreadSafe>::operator =(TWeakPtr&& ptr) -> TWeakPtr&
{
	SafeRelease();
	this->ptr = ptr.ptr;
	this->references = ptr.references;
	ptr.ptr = nullptr;
	ptr.references = nullptr;
	return *this;
}

template<TIsNotPointer T, bool bThreadSafe> template<TIsRefCore O>
inline bool TWeakPtr<T, bThreadSafe>::operator ==(const TWeakPtr<O, bThreadSafe>& ptr) const
{
	if (!IsValid)
	{
		return !ptr.IsValid_get();
	}

	else if (!ptr.IsValid_get())
	{
		return false;
	}

	else
	{
		return this->ptr->operator ==(ptr.ptr);
	}
}

template<TIsNotPointer T, bool bThreadSafe> template<TIsRefCore O>
inline bool TWeakPtr<T, bThreadSafe>::operator !=(const TWeakPtr<O, bThreadSafe>& ptr) const
{
	if (!IsValid)
	{
		return ptr.IsValid_get();
	}

	else if (!ptr.IsValid_get())
	{
		return true;
	}

	else
	{
		return this->ptr->operator !=(ptr);
	}
}

template<TIsNotPointer T, bool bThreadSafe> template<class TIndex> requires TIsIndexer<T, TIndex>
inline auto TWeakPtr<T, bThreadSafe>::operator [](const TIndex& index) const -> decltype(ptr->operator [](index))
{
	if (!IsValid)
	{
		ThrowNullReferenceException();
	}

	return ptr->operator [](index);
}

template<TIsNotPointer T, bool bThreadSafe>
inline void TWeakPtr<T, bThreadSafe>::SafeRelease()
{
	if (IsValid)
	{
		references->ReleaseWeak();
		ptr = nullptr;
		references = nullptr;
	}
}

template<TIsNotPointer T, bool bThreadSafe>
inline void TWeakPtr<T, bThreadSafe>::Assign(T* ptr)
{
	if (ptr != nullptr)
	{
		this->ptr = ptr;
		this->references = ptr->GetWeakReferences();
		this->references->AddWeakRef();
	}
}

template<TIsNotPointer T, bool bThreadSafe>
template<TIsBaseOf<T> O>
inline TWeakPtr<O, bThreadSafe> TRefPtr<T, bThreadSafe>::AsWeak() const
{
	return ptr;
}