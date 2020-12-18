// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

//#define BASE_TEMPLATE template<TIsRefCore T>
//#define OTHER_TEMPLATE template<class O> requires TIsBaseOf<O, T>
//#define THIS TRefPtr<T>
//#define CAST_TEMPLATE template<class O> requires TIsBaseOf<O, Object>

namespace SC::Runtime::Core
{
	template<TIsNotPointer T>
	inline TRefPtr<T>::TRefPtr()
		: ptr(nullptr)
	{

	}

	template<TIsNotPointer T>
	inline TRefPtr<T>::TRefPtr(std::nullptr_t)
		: This()
	{

	}
	
	template<TIsNotPointer T>
	inline TRefPtr<T>::TRefPtr(T* ptr)
		: This()
	{
		if (ptr != nullptr)
		{
			this->ptr = ptr;
			ptr->AddRef();
		}
	}

	template<TIsNotPointer T>
	inline TRefPtr<T>::TRefPtr(const T* ptr)
		: This(const_cast<T*>(ptr))
	{

	}

	template<TIsNotPointer T>
	inline TRefPtr<T>::TRefPtr(const TRefPtr& ptr)
		: This(ptr.ptr)
	{

	}

	template<TIsNotPointer T>
	inline TRefPtr<T>::TRefPtr(TRefPtr&& ptr)
		: This()
	{
		this->ptr = ptr.ptr;
		ptr.ptr = nullptr;
	}

	template<TIsNotPointer T>
	inline TRefPtr<T>::~TRefPtr()
	{
		SafeRelease();
	}

	template<TIsNotPointer T> template<TIsBaseOf<T> O>
	inline TRefPtr<T>::TRefPtr(const TRefPtr<O>& ptr)
		: This(ptr.ptr)
	{

	}

	template<TIsNotPointer T> template<TIsBaseOf<T> O>
	inline TRefPtr<T>::TRefPtr(TRefPtr<O>&& ptr)
		: This()
	{
		this->ptr = ptr.ptr;
		ptr.ptr = nullptr;
	}

	template<TIsNotPointer T>
	inline T* TRefPtr<T>::Detach()
	{
		T* detach = ptr;
		ptr = nullptr;
		return detach;
	}

	template<TIsNotPointer T>
	inline void TRefPtr<T>::Attach(T* ptr)
	{
		SafeRelease();
		this->ptr = ptr;
	}

	template<TIsNotPointer T>
	inline void TRefPtr<T>::Reset(T* ptr)
	{
		SafeRelease();
		this->ptr = ptr;
		if (ptr != nullptr)
		{
			ptr->AddRef();
		}
	}

	template<TIsNotPointer T>
	inline T* TRefPtr<T>::Get() const
	{
		return ptr;
	}

	template<TIsNotPointer T>
	inline T** TRefPtr<T>::GetAddressOf()
	{
		return &ptr;
	}

	template<TIsNotPointer T>
	inline T** TRefPtr<T>::ReleaseAndGetAddressOf()
	{
		SafeRelease();
		return &ptr;
	}

	template<TIsNotPointer T> template<TIsRefCore O>
	inline bool TRefPtr<T>::Is(O** ptr) const
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

	template<TIsNotPointer T> template<TIsRefCore O>
	inline TRefPtr<O> TRefPtr<T>::As() const
	{
		auto ret = TryAs<O>();
		if (!ret.IsValid)
		{
			ThrowInvalidCastException();
		}
		return move(ret);
	}

	template<TIsNotPointer T> template<TIsRefCore O>
	inline TRefPtr<O> TRefPtr<T>::TryAs() const
	{
		O* cast = dynamic_cast<O*>(this->ptr);
		if (cast == nullptr)
		{
			return nullptr;
		}

		return ptr;
	}

	template<TIsNotPointer T>
	inline bool TRefPtr<T>::IsValid_get() const
	{
		return ptr != nullptr;
	}

	template<TIsNotPointer T>
	template<THasIterator O>
	inline auto TRefPtr<T>::begin() -> typename O::Iterator
	{
		return ptr->begin();
	}

	template<TIsNotPointer T>
	template<THasIterator O>
	inline auto TRefPtr<T>::end() -> typename O::Iterator
	{
		return ptr->end();
	}

	template<TIsNotPointer T>
	template<THasConstIterator O>
	inline auto TRefPtr<T>::begin() const -> typename O::ConstIterator
	{
		return cbegin();
	}

	template<TIsNotPointer T>
	template<THasConstIterator O>
	inline auto TRefPtr<T>::end() const -> typename O::ConstIterator
	{
		return cend();
	}

	template<TIsNotPointer T>
	template<THasConstIterator O>
	inline auto TRefPtr<T>::cbegin() const -> typename O::ConstIterator
	{
		return ptr->cbegin();
	}

	template<TIsNotPointer T>
	template<THasConstIterator O>
	inline auto TRefPtr<T>::cend() const -> typename O::ConstIterator
	{
		return ptr->cend();
	}

	template<TIsNotPointer T>
	inline T* TRefPtr<T>::operator->() const
	{
		return Get();
	}

	template<TIsNotPointer T>
	inline TRefPtr<T>::operator bool() const
	{
		return IsValid;
	}

	template<TIsNotPointer T>
	inline T** TRefPtr<T>::operator&()
	{
		return ReleaseAndGetAddressOf();
	}

	template<TIsNotPointer T>
	inline auto TRefPtr<T>::operator=(const TRefPtr& ptr) -> This&
	{
		Reset(ptr.ptr);
		return *this;
	}

	template<TIsNotPointer T>
	inline auto TRefPtr<T>::operator=(TRefPtr&& ptr) -> This&
	{
		Attach(ptr.Detach());
		return *this;
	}

	template<TIsNotPointer T>
	inline void TRefPtr<T>::SafeRelease()
	{
		if (ptr != nullptr)
		{
			ptr->Release();
			ptr = nullptr;
		}
	}

	template<TIsNotPointer T> template<TIsRefCore O>
	inline bool TRefPtr<T>::operator ==(const TRefPtr<O>& ptr) const
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

	template<TIsNotPointer T> template<TIsRefCore O>
	inline bool TRefPtr<T>::operator !=(const TRefPtr<O>& ptr) const
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

	template<TIsNotPointer T> template<class TIndex> requires TIsIndexer<T, TIndex>
	inline auto TRefPtr<T>::operator [](const TIndex& index) const -> decltype(ptr->operator [](index))
	{
		if (ptr == nullptr)
		{
			ThrowNullReferenceException();
		}

		return ptr->operator [](index);
	}
}