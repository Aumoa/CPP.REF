// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

#pragma warning(disable: 4251)

namespace SC::Runtime::Core
{
	class Object;
	class String;

	void CORE_API ThrowInvalidCastException();
	void CORE_API ThrowNullReferenceException();

	template<TIsNotPointer T, bool bThreadSafe>
	class TWeakPtr;

	template<TIsNotPointer T, bool bThreadSafe = true>
	class TRefPtr
	{
		friend class Object;
		template<TIsNotPointer O, bool bThreadSafe1>
		friend class TRefPtr;

	public:
		using Type = T;
		using This = TRefPtr;

	private:
		T* ptr;

	public:
		inline TRefPtr();
		inline TRefPtr(std::nullptr_t);
		inline TRefPtr(T* ptr);
		inline TRefPtr(const T* ptr);
		inline TRefPtr(const TRefPtr& ptr);
		inline TRefPtr(TRefPtr&& ptr);
		inline ~TRefPtr();

		// String.inl
		template<TIsStringConstructible TStringConstructibleArg> requires TIsAssignable<String*, T*>
		inline TRefPtr(TStringConstructibleArg text);

		template<TIsBaseOf<T> O>
		inline TRefPtr(const TRefPtr<O, bThreadSafe>& ptr);
		template<TIsBaseOf<T> O>
		inline TRefPtr(TRefPtr<O, bThreadSafe>&& ptr);

		inline T* Detach();
		inline void Attach(T* ptr);
		inline void Reset(T* ptr = nullptr);

		[[nodiscard]] inline T* Get() const;
		[[nodiscard]] inline T** GetAddressOf();
		[[nodiscard]] inline T** ReleaseAndGetAddressOf();
		[[nodiscard]] inline TRefPtr<String, true> ToString() const;
		[[nodiscard]] inline size_t GetHashCode() const;

		template<TIsRefCore O>
		[[nodiscard]] inline bool Is(O** ptr = nullptr) const;
		template<TIsRefCore O>
		[[nodiscard]] inline TRefPtr<O, bThreadSafe> As() const;
		template<TIsRefCore O>
		[[nodiscard]] inline TRefPtr<O, bThreadSafe> TryAs() const;

		template<TIsBaseOf<T> O = T>
		inline TWeakPtr<O, bThreadSafe> AsWeak() const;  // TWeakPtr.inl

		vs_property_get(bool, IsValid);
		[[nodiscard]] inline bool IsValid_get() const;

		template<THasIterator O = T>
		inline auto begin() -> typename O::Iterator;
		template<THasIterator O = T>
		inline auto end() -> typename O::Iterator;
		template<THasConstIterator O = T>
		inline auto begin() const -> typename O::ConstIterator;
		template<THasConstIterator O = T>
		inline auto end() const -> typename O::ConstIterator;
		template<THasConstIterator O = T>
		inline auto cbegin() const -> typename O::ConstIterator;
		template<THasConstIterator O = T>
		inline auto cend() const -> typename O::ConstIterator;

		[[nodiscard]] inline T* operator->() const;
		[[nodiscard]] inline operator bool() const;
		[[nodiscard]] inline T** operator&();

		inline TRefPtr& operator=(const TRefPtr& ptr);
		inline TRefPtr& operator=(TRefPtr&& ptr);

		template<TIsRefCore O>
		[[nodiscard]] inline bool operator ==(const TRefPtr<O, bThreadSafe>& ptr) const;
		template<TIsRefCore O>
		[[nodiscard]] inline bool operator !=(const TRefPtr<O, bThreadSafe>& ptr) const;
		template<TIsRefCore O> requires TComparable_Less<T, TRefPtr<O, bThreadSafe>>
		[[nodiscard]] inline auto operator < (const TRefPtr<O, bThreadSafe>& ptr) const -> decltype(((T*)(0))->operator <(ptr));
		template<TIsRefCore O> requires TComparable_LessEquals<T, TRefPtr<O, bThreadSafe>>
		[[nodiscard]] inline auto operator <=(const TRefPtr<O, bThreadSafe>& ptr) const -> decltype(((T*)(0))->operator <=(ptr));
		template<TIsRefCore O> requires TComparable_Greater<T, TRefPtr<O, bThreadSafe>>
		[[nodiscard]] inline auto operator > (const TRefPtr<O, bThreadSafe>& ptr) const -> decltype(((T*)(0))->operator >(ptr));
		template<TIsRefCore O> requires TComparable_GreaterEquals<T, TRefPtr<O, bThreadSafe>>
		[[nodiscard]] inline auto operator >=(const TRefPtr<O, bThreadSafe>& ptr) const -> decltype(((T*)(0))->operator >=(ptr));
		template<class TIndex> requires TIsIndexer<T, TIndex>
		[[nodiscard]] inline auto operator [](const TIndex& index) const -> decltype(ptr->operator [](index));

		// String specialize
		template<TIsChar TChar> requires TIsSame<T, String>
		[[nodiscard]] inline bool operator ==(const TChar* text) const;
		template<TIsChar TChar> requires TIsSame<T, String>
		[[nodiscard]] inline bool operator !=(const TChar* text) const;

	private:
		inline void SafeRelease();
	};
}

#include "TWeakPtr.h"
#include "TRefPtr.inl"