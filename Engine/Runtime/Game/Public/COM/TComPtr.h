// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "COMException.h"

template<class T>
class TComPtr
{
	T* instance;

public:
	inline TComPtr();
	inline TComPtr(std::nullptr_t);
	inline TComPtr(T* ptr);
	inline TComPtr(const TComPtr& ptr);
	inline TComPtr(TComPtr&& ptr);
	inline ~TComPtr();

	inline T* Detach();
	inline void Attach(T* ptr);
	inline void Reset(T* ptr = nullptr);

	[[nodiscard]] inline T* Get() const;
	[[nodiscard]] inline T** GetAddressOf();
	[[nodiscard]] inline T** ReleaseAndGetAddressOf();

	template<class O>
	[[nodiscard]] inline bool Is(O** ptr = nullptr) const;
	template<class O>
	[[nodiscard]] inline TComPtr<O> As() const;
	template<class O>
	[[nodiscard]] inline TComPtr<O> TryAs() const;

	vs_property_get_auto(bool, IsValid, instance != nullptr);

	[[nodiscard]] inline T* operator ->() const;
	[[nodiscard]] inline operator bool() const;
	[[nodiscard]] inline T** operator &();

	inline TComPtr& operator =(const TComPtr& rh);
	inline TComPtr& operator =(TComPtr&& rh);

private:
	inline void SafeAddRef();
	inline void SafeRelease();
};

#include "TComPtr.inl"