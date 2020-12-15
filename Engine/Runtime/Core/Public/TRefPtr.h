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

	template<TIsNotPointer T>
	class TRefPtr
	{
		friend class Object;
		template<TIsNotPointer O>
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
		template<class TChar> requires TIsChar<TChar> && TIsAssignable<String, T>
		inline TRefPtr(const TChar* text);

		template<class O> requires TIsBaseOf<O, T> || TIsRefCore<T>
		inline TRefPtr(const TRefPtr<O>& ptr);
		template<class O> requires TIsBaseOf<O, T> || TIsRefCore<T>
		inline TRefPtr(TRefPtr<O>&& ptr);

		inline T* Detach();
		inline void Attach(T* ptr);
		inline void Reset(T* ptr);

		[[nodiscard]] inline T* Get() const;
		[[nodiscard]] inline T** GetAddressOf();
		[[nodiscard]] inline T** ReleaseAndGetAddressOf();

		template<TIsRefCore O>
		[[nodiscard]] inline bool Is(O** ptr = nullptr) const;
		template<TIsRefCore O>
		[[nodiscard]] inline auto As() const;
		template<TIsRefCore O>
		[[nodiscard]] inline TRefPtr<O> TryAs() const;

		vs_property_get(bool, IsValid);
		[[nodiscard]] inline bool IsValid_get() const;

		[[nodiscard]] inline T* operator->() const;
		[[nodiscard]] inline operator bool() const;
		[[nodiscard]] inline T** operator&();

		inline TRefPtr& operator=(const TRefPtr& ptr);
		inline TRefPtr& operator=(TRefPtr&& ptr);

		template<TIsRefCore O>
		[[nodiscard]] inline bool operator ==(const TRefPtr<O>& ptr) const;
		template<TIsRefCore O>
		[[nodiscard]] inline bool operator !=(const TRefPtr<O>& ptr) const;
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

#include "TRefPtr.inl"