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

	template<class T> requires TIsRefCore<T>
	class TRefPtr
	{
		friend class Object;

	public:
		using Type = T;
		using This = TRefPtr;

	private:
		T* ptr;

		inline TRefPtr(T* ptr);

	public:
		inline TRefPtr();
		inline TRefPtr(std::nullptr_t);
		inline TRefPtr(const TRefPtr& ptr);
		inline TRefPtr(TRefPtr&& ptr);
		inline ~TRefPtr();

		// String.inl
		template<class TChar> requires TIsChar<TChar> && TIsAssignable<String, T>
		inline TRefPtr(const TChar* text);

		template<class O> requires TIsBaseOf<O, T>
		inline TRefPtr(const TRefPtr<O>& ptr);
		template<class O> requires TIsBaseOf<O, T>
		inline TRefPtr(TRefPtr<O>&& ptr);

		inline T* Detach();
		inline void Attach(T* ptr);
		inline void Reset(T* ptr);

		[[nodiscard]] inline T* Get() const;
		[[nodiscard]] inline T** GetAddressOf();
		[[nodiscard]] inline T** ReleaseAndGetAddressOf();

		template<class O> requires TIsBaseOf<O, Object>
		[[nodiscard]] inline bool Is(O** ptr = nullptr) const;
		template<class O> requires TIsBaseOf<O, Object>
		[[nodiscard]] inline auto As() const;
		template<class O> requires TIsBaseOf<O, Object>
		[[nodiscard]] inline TRefPtr<O> TryAs() const;

		vs_property_get(bool, IsValid);
		[[nodiscard]] inline bool IsValid_get() const;

		[[nodiscard]] inline T* operator->() const;
		[[nodiscard]] inline operator bool() const;
		[[nodiscard]] inline T** operator&();

		inline TRefPtr& operator=(const TRefPtr& ptr);
		inline TRefPtr& operator=(TRefPtr&& ptr);

	private:
		inline void SafeRelease();
	};
}

#include "TRefPtr.inl"