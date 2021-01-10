// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "TSubclassOf.h"

template<class TBase>
TFunction<TRefPtr<Object>()> TSubclassOf<TBase>::MyConstructor;

template<class TBase>
inline TSubclassOf<TBase>::TSubclassOf()
{

}

template<class TBase>
inline TSubclassOf<TBase>::TSubclassOf(const TSubclassOf& inSubclass)
	: TypeHashCode(inSubclass.TypeHashCode)
	, Constructor(inSubclass.Constructor)
{

}

template<class TBase>
template<TIsBaseOf<TBase> O>
inline TSubclassOf<TBase>::TSubclassOf(const TSubclassOf<O>& inSubclass)
	: TypeHashCode(inSubclass.TypeHashCode)
	, Constructor(inSubclass.Constructor)
{

}

template<class TBase>
inline size_t TSubclassOf<TBase>::GetHashCode() const
{
	return TypeHashCode;
}

template<class TBase>
inline bool TSubclassOf<TBase>::IsValid_get() const
{
	return TypeHashCode != 0 && Constructor.IsValid;
}

template<class TBase>
inline TRefPtr<TBase> TSubclassOf<TBase>::Instantiate() const
{
	if (!IsValid)
	{
		throw NullReferenceException("TSubclassOf<TBase> is not setted.");
	}

	return Object::Cast<TBase>(Constructor());
}

template<class TBase>
template<TIsBaseOf<TBase> O>
inline TRefPtr<O> TSubclassOf<TBase>::Instantiate() const
{
	return Object::Cast<O>(Instantiate());
}

template<class TBase>
inline auto TSubclassOf<TBase>::operator =(const TSubclassOf& rh) -> TSubclassOf&
{
	TypeHashCode = rh.TypeHashCode;
	Constructor = rh.Constructor;
	return *this;
}

template<class TBase>
template<TIsBaseOf<TBase> O>
inline auto TSubclassOf<TBase>::operator =(const TSubclassOf<O>& rh) -> TSubclassOf&
{
	TypeHashCode = rh.TypeHashCode;
	Constructor = rh.Constructor;
	return *this;
}

template<class TBase>
template<TIsBaseOf<TBase> O>
inline bool TSubclassOf<TBase>::operator ==(const TSubclassOf<O>& rh) const
{
	return TypeHashCode == rh.TypeHashCode;
}

template<class TBase>
template<TIsBaseOf<TBase> O>
inline bool TSubclassOf<TBase>::operator !=(const TSubclassOf<O>& rh) const
{
	return TypeHashCode != rh.TypeHashCode;
}

template<class TBase>
inline auto TSubclassOf<TBase>::StaticClass() -> TSubclassOf
{
	if (!MyConstructor.IsValid)
	{
		MyConstructor = []() -> TRefPtr<Object>
		{
			return Object::NewObject<TBase>();
		};
	}

	TSubclassOf sub;
	sub.TypeHashCode = TUniqueType<TBase>::HashCode;
	sub.Constructor = MyConstructor;
	return sub;
}