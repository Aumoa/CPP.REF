// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

template<TIsNotPointer T, bool bThreadSafe>
inline TRefPtr<T, bThreadSafe>::TRefPtr()
	: ptr(nullptr)
{

}

template<TIsNotPointer T, bool bThreadSafe>
inline TRefPtr<T, bThreadSafe>::TRefPtr(std::nullptr_t)
	: This()
{

}
	
template<TIsNotPointer T, bool bThreadSafe>
inline TRefPtr<T, bThreadSafe>::TRefPtr(const T* ptr)
	: This()
{
	if (ptr != nullptr)
	{
		this->ptr = const_cast<std::remove_const_t<T>*>(ptr);
		this->ptr->AddRef();
	}
}

template<TIsNotPointer T, bool bThreadSafe>
inline TRefPtr<T, bThreadSafe>::TRefPtr(const TRefPtr& ptr)
	: This(ptr.ptr)
{

}

template<TIsNotPointer T, bool bThreadSafe>
inline TRefPtr<T, bThreadSafe>::TRefPtr(TRefPtr&& ptr)
	: This()
{
	this->ptr = ptr.ptr;
	ptr.ptr = nullptr;
}

template<TIsNotPointer T, bool bThreadSafe>
inline TRefPtr<T, bThreadSafe>::~TRefPtr()
{
	SafeRelease();
}

template<TIsNotPointer T, bool bThreadSafe> template<TIsBaseOf<T> O>
inline TRefPtr<T, bThreadSafe>::TRefPtr(const TRefPtr<O, bThreadSafe>& ptr)
	: This(ptr.ptr)
{

}

template<TIsNotPointer T, bool bThreadSafe> template<TIsBaseOf<T> O>
inline TRefPtr<T, bThreadSafe>::TRefPtr(const TRefPtr<const O, bThreadSafe>& ptr)
	: This(const_cast<O*>(ptr.ptr))
{

}

template<TIsNotPointer T, bool bThreadSafe> template<TIsBaseOf<T> O>
inline TRefPtr<T, bThreadSafe>::TRefPtr(TRefPtr<O, bThreadSafe>&& ptr)
	: This()
{
	this->ptr = ptr.ptr;
	ptr.ptr = nullptr;
}

template<TIsNotPointer T, bool bThreadSafe> template<TIsBaseOf<T> O>
inline TRefPtr<T, bThreadSafe>::TRefPtr(TRefPtr<const O, bThreadSafe>&& ptr)
	: This()
{
	this->ptr = const_cast<O*>(ptr.ptr);
	ptr.ptr = nullptr;
}

template<TIsNotPointer T, bool bThreadSafe>
inline T* TRefPtr<T, bThreadSafe>::Detach()
{
	T* detach = ptr;
	ptr = nullptr;
	return detach;
}

template<TIsNotPointer T, bool bThreadSafe>
inline void TRefPtr<T, bThreadSafe>::Attach(T* ptr)
{
	SafeRelease();
	this->ptr = ptr;
}

template<TIsNotPointer T, bool bThreadSafe>
inline void TRefPtr<T, bThreadSafe>::Reset(T* ptr)
{
	SafeRelease();
	this->ptr = ptr;
	if (ptr != nullptr)
	{
		ptr->AddRef();
	}
}

template<TIsNotPointer T, bool bThreadSafe>
inline T* TRefPtr<T, bThreadSafe>::Get() const
{
	return ptr;
}

template<TIsNotPointer T, bool bThreadSafe>
inline T** TRefPtr<T, bThreadSafe>::GetAddressOf()
{
	return &ptr;
}

template<TIsNotPointer T, bool bThreadSafe>
inline T** TRefPtr<T, bThreadSafe>::ReleaseAndGetAddressOf()
{
	SafeRelease();
	return &ptr;
}

template<TIsNotPointer T, bool bThreadSafe>
inline TRefPtr<String, true> TRefPtr<T, bThreadSafe>::ToString() const
{
	return ptr->ToString();
}

template<TIsNotPointer T, bool bThreadSafe>
inline size_t TRefPtr<T, bThreadSafe>::GetHashCode() const
{
	return ptr->GetHashCode();
}

template<TIsNotPointer T, bool bThreadSafe> template<TIsRefCore O>
inline bool TRefPtr<T, bThreadSafe>::Is(O** ptr) const
{
	O* cast = dynamic_cast<O*>(this->ptr);
	if (cast == nullptr)
	{
		if (ptr != nullptr)
		{
			*ptr = nullptr;
		}
		return false;
	}

	if (ptr != nullptr)
	{
		*ptr = cast;
		cast->AddRef();
	}

	return true;
}

template<TIsNotPointer T, bool bThreadSafe> template<TIsRefCore O>
inline TRefPtr<O, bThreadSafe> TRefPtr<T, bThreadSafe>::As() const
{
	auto ret = TryAs<O>();
	if (!ret.IsValid_get())
	{
		ThrowInvalidCastException();
	}
	return std::move(ret);
}

template<TIsNotPointer T, bool bThreadSafe> template<TIsRefCore O>
inline TRefPtr<O, bThreadSafe> TRefPtr<T, bThreadSafe>::TryAs() const
{
	O* cast = dynamic_cast<O*>(this->ptr);
	if (cast == nullptr)
	{
		return nullptr;
	}

	return cast;
}

template<TIsNotPointer T, bool bThreadSafe>
inline bool TRefPtr<T, bThreadSafe>::IsValid_get() const
{
	return ptr != nullptr;
}

template<TIsNotPointer T, bool bThreadSafe>
template<THasIterator O>
inline auto TRefPtr<T, bThreadSafe>::begin() -> typename O::Iterator
{
	return ptr->begin();
}

template<TIsNotPointer T, bool bThreadSafe>
template<THasIterator O>
inline auto TRefPtr<T, bThreadSafe>::end() -> typename O::Iterator
{
	return ptr->end();
}

template<TIsNotPointer T, bool bThreadSafe>
template<THasConstIterator O>
inline auto TRefPtr<T, bThreadSafe>::begin() const -> typename O::ConstIterator
{
	return cbegin();
}

template<TIsNotPointer T, bool bThreadSafe>
template<THasConstIterator O>
inline auto TRefPtr<T, bThreadSafe>::end() const -> typename O::ConstIterator
{
	return cend();
}

template<TIsNotPointer T, bool bThreadSafe>
template<THasConstIterator O>
inline auto TRefPtr<T, bThreadSafe>::cbegin() const -> typename O::ConstIterator
{
	return ptr->cbegin();
}

template<TIsNotPointer T, bool bThreadSafe>
template<THasConstIterator O>
inline auto TRefPtr<T, bThreadSafe>::cend() const -> typename O::ConstIterator
{
	return ptr->cend();
}

template<TIsNotPointer T, bool bThreadSafe>
inline T* TRefPtr<T, bThreadSafe>::operator->() const
{
	return Get();
}

template<TIsNotPointer T, bool bThreadSafe>
inline TRefPtr<T, bThreadSafe>::operator bool() const
{
	return IsValid;
}

template<TIsNotPointer T, bool bThreadSafe>
inline T** TRefPtr<T, bThreadSafe>::operator&()
{
	return ReleaseAndGetAddressOf();
}

template<TIsNotPointer T, bool bThreadSafe>
inline auto TRefPtr<T, bThreadSafe>::operator=(const TRefPtr& ptr) -> This&
{
	Reset(ptr.ptr);
	return *this;
}

template<TIsNotPointer T, bool bThreadSafe>
inline auto TRefPtr<T, bThreadSafe>::operator=(TRefPtr&& ptr) -> This&
{
	Attach(ptr.Detach());
	return *this;
}

template<TIsNotPointer T, bool bThreadSafe>
inline void TRefPtr<T, bThreadSafe>::SafeRelease()
{
	if (ptr != nullptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}

template<TIsNotPointer T, bool bThreadSafe> template<TIsRefCore O>
inline bool TRefPtr<T, bThreadSafe>::operator ==(const TRefPtr<O, bThreadSafe>& ptr) const
{
	if (this->ptr == nullptr)
	{
		return !ptr.IsValid_get();
	}

	else if (!ptr.IsValid_get())
	{
		return false;
	}

	else
	{
		return this->ptr->operator ==(ptr);
	}
}

template<TIsNotPointer T, bool bThreadSafe> template<TIsRefCore O>
inline bool TRefPtr<T, bThreadSafe>::operator !=(const TRefPtr<O, bThreadSafe>& ptr) const
{
	if (this->ptr == nullptr)
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

template<TIsNotPointer T, bool bThreadSafe> template<TIsRefCore O> requires TComparable_Less<T, TRefPtr<O, bThreadSafe>>
inline auto TRefPtr<T, bThreadSafe>::operator < (const TRefPtr<O, bThreadSafe>& ptr) const -> decltype(((T*)(0))->operator < (ptr))
{
	return this->ptr->operator < (ptr);
}

template<TIsNotPointer T, bool bThreadSafe> template<TIsRefCore O> requires TComparable_LessEquals<T, TRefPtr<O, bThreadSafe>>
inline auto TRefPtr<T, bThreadSafe>::operator <=(const TRefPtr<O, bThreadSafe>& ptr) const -> decltype(((T*)(0))->operator <=(ptr))
{
	return this->ptr->operator <=(ptr);
}

template<TIsNotPointer T, bool bThreadSafe> template<TIsRefCore O> requires TComparable_Greater<T, TRefPtr<O, bThreadSafe>>
inline auto TRefPtr<T, bThreadSafe>::operator > (const TRefPtr<O, bThreadSafe>& ptr) const -> decltype(((T*)(0))->operator > (ptr))
{
	return this->ptr->operator > (ptr);
}

template<TIsNotPointer T, bool bThreadSafe> template<TIsRefCore O> requires TComparable_GreaterEquals<T, TRefPtr<O, bThreadSafe>>
inline auto TRefPtr<T, bThreadSafe>::operator >=(const TRefPtr<O, bThreadSafe>& ptr) const -> decltype(((T*)(0))->operator >=(ptr))
{
	return this->ptr->operator >=(ptr);
}

template<TIsNotPointer T, bool bThreadSafe> template<class TIndex> requires TIsIndexer<T, TIndex>
inline auto TRefPtr<T, bThreadSafe>::operator [](const TIndex& index) const -> decltype(ptr->operator [](index))
{
	if (ptr == nullptr)
	{
		ThrowNullReferenceException();
	}

	return ptr->operator [](index);
}

template<TIsNotPointer O, TIsNotPointer T, bool bThreadSafe> requires TIsBaseOf<O, Object> && TIsBaseOf<T, Object>
inline TRefPtr<O, bThreadSafe> Object::Cast(TRefPtr<T, bThreadSafe> ptr)
{
	return ptr.As<O>();
}