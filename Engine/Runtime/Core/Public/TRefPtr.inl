// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

//#define BASE_TEMPLATE template<TIsRefCore T>
//#define OTHER_TEMPLATE template<class O> requires TIsBaseOf<O, T>
//#define THIS TRefPtr<T>
//#define CAST_TEMPLATE template<class O> requires TIsBaseOf<O, Object>

namespace SC::Runtime::Core
{
	template<TIsRefCore T>
	inline TRefPtr<T>::TRefPtr(T* ptr)
		: This()
	{
		if (ptr != nullptr)
		{
			this->ptr = ptr;
			ptr->AddRef();
		}
	}

	template<TIsRefCore T>
	inline TRefPtr<T>::TRefPtr()
		: ptr(nullptr)
	{

	}

	template<TIsRefCore T>
	inline TRefPtr<T>::TRefPtr(std::nullptr_t)
		: This()
	{

	}

	template<TIsRefCore T>
	inline TRefPtr<T>::TRefPtr(const TRefPtr& ptr)
		: This(ptr.ptr)
	{

	}

	template<TIsRefCore T>
	inline TRefPtr<T>::TRefPtr(TRefPtr&& ptr)
		: This()
	{
		this->ptr = ptr.ptr;
		ptr.ptr = nullptr;
	}

	template<TIsRefCore T>
	inline TRefPtr<T>::~TRefPtr()
	{
		SafeRelease();
	}

	template<TIsRefCore T> template<class O> requires TIsBaseOf<O, T>
	inline TRefPtr<T>::TRefPtr(const TRefPtr<O>& ptr)
		: This(ptr.ptr)
	{

	}

	template<TIsRefCore T> template<class O> requires TIsBaseOf<O, T>
	inline TRefPtr<T>::TRefPtr(TRefPtr<O>&& ptr)
		: This()
	{
		this->ptr = ptr.ptr;
		ptr.ptr = nullptr;
	}

	template<TIsRefCore T>
	inline T* TRefPtr<T>::Detach()
	{
		T* detach = ptr;
		ptr = nullptr;
		return detach;
	}

	template<TIsRefCore T>
	inline void TRefPtr<T>::Attach(T* ptr)
	{
		SafeRelease();
		this->ptr = ptr;
	}

	template<TIsRefCore T>
	inline void TRefPtr<T>::Reset(T* ptr)
	{
		SafeRelease();
		this->ptr = ptr;
		if (ptr != nullptr)
		{
			ptr->AddRef();
		}
	}

	template<TIsRefCore T>
	inline T* TRefPtr<T>::Get() const
	{
		return ptr;
	}

	template<TIsRefCore T>
	inline T** TRefPtr<T>::GetAddressOf()
	{
		return &ptr;
	}

	template<TIsRefCore T>
	inline T** TRefPtr<T>::ReleaseAndGetAddressOf()
	{
		SafeRelease();
		return &ptr;
	}

	template<TIsRefCore T> template<class O> requires TIsBaseOf<O, Object>
	inline bool TRefPtr<T>::Is(O** ptr) const
	{
		O* cast = dynamic_cast<O*>(this->ptr);
		if (cast == nullptr)
		{
			*ptr = nullptr;
			return false;
		}

		*ptr = cast;
		this->ptr->AddRef();
		return true;
	}

	template<TIsRefCore T> template<class O> requires TIsBaseOf<O, Object>
	inline auto TRefPtr<T>::As() const
	{
		auto ret = TryAs<O>();
		if (!ret.IsValid)
		{
			ThrowInvalidCastException();
		}
		return move(ret);
	}

	template<TIsRefCore T> template<class O> requires TIsBaseOf<O, Object>
	inline TRefPtr<O> TRefPtr<T>::TryAs() const
	{
		O* cast = dynamic_cast<O*>(this->ptr);
		if (cast == nullptr)
		{
			return nullptr;
		}

		return ptr;
	}

	template<TIsRefCore T>
	inline bool TRefPtr<T>::IsValid_get() const
	{
		return ptr != nullptr;
	}

	template<TIsRefCore T>
	inline T* TRefPtr<T>::operator->() const
	{
		return Get();
	}

	template<TIsRefCore T>
	inline TRefPtr<T>::operator bool() const
	{
		return IsValid;
	}

	template<TIsRefCore T>
	inline T** TRefPtr<T>::operator&()
	{
		return ReleaseAndGetAddressOf();
	}

	template<TIsRefCore T>
	inline auto TRefPtr<T>::operator=(const TRefPtr& ptr) -> This&
	{
		Reset(ptr.ptr);
		return *this;
	}

	template<TIsRefCore T>
	inline auto TRefPtr<T>::operator=(TRefPtr&& ptr) -> This&
	{
		Attach(ptr.Detach());
		return *this;
	}

	template<TIsRefCore T>
	inline void TRefPtr<T>::SafeRelease()
	{
		if (ptr != nullptr)
		{
			ptr->Release();
			ptr = nullptr;
		}
	}
}