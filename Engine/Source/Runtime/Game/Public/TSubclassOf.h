// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

template<class TBase>
struct TSubclassOf
{
	template<class O>
	friend struct TSubclassOf;

private:
	static TFunction<TRefPtr<Object>()> MyConstructor;

	size_t TypeHashCode;
	TFunction<TRefPtr<Object>()> Constructor;

public:
	inline TSubclassOf();
	inline TSubclassOf(const TSubclassOf& inSubclass);
	template<TIsBaseOf<TBase> O>
	inline TSubclassOf(const TSubclassOf<O>& inSubclass);

	inline size_t GetHashCode() const;

	vs_property_get(bool, IsValid);

	inline TRefPtr<TBase> Instantiate() const;
	template<TIsBaseOf<TBase> O>
	inline TRefPtr<O> Instantiate() const;

	inline TSubclassOf& operator =(const TSubclassOf& inSubclass);
	template<TIsBaseOf<TBase> O>
	inline TSubclassOf& operator =(const TSubclassOf<O>& inSubclass);

	template<TIsBaseOf<TBase> O>
	inline bool operator ==(const TSubclassOf<O>& rh) const;
	template<TIsBaseOf<TBase> O>
	inline bool operator !=(const TSubclassOf<O>& rh) const;

	inline static TSubclassOf StaticClass();
};

#include "TSubclassOf.inl"