// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

#include "WeakReferences.h"
#include "TRefPtr.h"

#pragma warning(disable: 4251)

// TRefPtr.cpp
void CORE_API ThrowInvalidCastException();
void CORE_API ThrowNullReferenceException();

template<TIsNotPointer T, bool bThreadSafe = true>
class TWeakPtr
{
public:
	using This = TWeakPtr;

private:
	T* ptr;
	WeakReferences* references;

public:
	inline TWeakPtr();
	inline TWeakPtr(std::nullptr_t);
	inline TWeakPtr(T* ptr);
	inline TWeakPtr(const T* ptr);
	inline TWeakPtr(const TWeakPtr& ptr);
	inline TWeakPtr(TWeakPtr&& ptr);
	inline ~TWeakPtr();

	template<TIsBaseOf<T> O>
	inline TWeakPtr(const TWeakPtr<O, bThreadSafe>& ptr);
	template<TIsBaseOf<T> O>
	inline TWeakPtr(TWeakPtr<O, bThreadSafe>&& ptr);

	inline void Reset(T* ptr = nullptr);

	[[nodiscard]] inline T* Get() const;

	template<TIsRefCore O>
	[[nodiscard]] inline bool Is(O** ptr = nullptr) const;
	template<TIsRefCore O>
	[[nodiscard]] inline TWeakPtr<O, bThreadSafe> As() const;
	template<TIsRefCore O>
	[[nodiscard]] inline TWeakPtr<O, bThreadSafe> TryAs() const;

	template<TIsBaseOf<T> O = T>
	inline TRefPtr<O, bThreadSafe> AsShared() const;

	vs_property_get(bool, IsValid);
	[[nodiscard]] inline bool IsValid_get() const;

	[[nodiscard]] inline T* operator->() const;
	[[nodiscard]] inline operator bool() const;

	inline TWeakPtr& operator=(const TWeakPtr& ptr);
	inline TWeakPtr& operator=(TWeakPtr&& ptr);

	template<TIsRefCore O>
	[[nodiscard]] inline bool operator ==(const TWeakPtr<O, bThreadSafe>& ptr) const;
	template<TIsRefCore O>
	[[nodiscard]] inline bool operator !=(const TWeakPtr<O, bThreadSafe>& ptr) const;
	template<class TIndex> requires TIsIndexer<T, TIndex>
	[[nodiscard]] inline auto operator [](const TIndex& index) const -> decltype(ptr->operator [](index));

private:
	inline void SafeRelease();
	inline void Assign(T* ptr);
};

#include "TWeakPtr.inl"