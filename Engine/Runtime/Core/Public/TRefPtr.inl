// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

//#define BASE_TEMPLATE template<TIsRefCore T>
//#define OTHER_TEMPLATE template<class O> requires TIsBaseOf<O, T>
//#define THIS TRefPtr<T>
//#define CAST_TEMPLATE template<class O> requires TIsBaseOf<O, Object>

namespace SC::Runtime::Core
{
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
	inline TRefPtr<T, bThreadSafe>::TRefPtr(T* ptr)
		: This()
	{
		if (ptr != nullptr)
		{
			this->ptr = ptr;
			ptr->AddRef();
		}
	}

	template<TIsNotPointer T, bool bThreadSafe>
	inline TRefPtr<T, bThreadSafe>::TRefPtr(const T* ptr)
		: This(const_cast<T*>(ptr))
	{

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
	inline TRefPtr<T, bThreadSafe>::TRefPtr(TRefPtr<O, bThreadSafe>&& ptr)
		: This()
	{
		this->ptr = ptr.ptr;
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
		if (!ret.IsValid)
		{
			ThrowInvalidCastException();
		}
		return move(ret);
	}

	template<TIsNotPointer T, bool bThreadSafe> template<TIsRefCore O>
	inline TRefPtr<O, bThreadSafe> TRefPtr<T, bThreadSafe>::TryAs() const
	{
		O* cast = dynamic_cast<O*>(this->ptr);
		if (cast == nullptr)
		{
			return nullptr;
		}

		return ptr;
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
			return !ptr.IsValid;
		}

		else if (!ptr.IsValid)
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

	template<TIsNotPointer T, bool bThreadSafe> template<class TIndex> requires TIsIndexer<T, TIndex>
	inline auto TRefPtr<T, bThreadSafe>::operator [](const TIndex& index) const -> decltype(ptr->operator [](index))
	{
		if (ptr == nullptr)
		{
			ThrowNullReferenceException();
		}

		return ptr->operator [](index);
	}
}