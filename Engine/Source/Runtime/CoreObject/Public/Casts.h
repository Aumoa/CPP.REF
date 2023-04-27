// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "Concepts/IDerivedFrom.h"
//#include "CoreAssert.h"

// ------------- Object to Object casts. -------------

template<class TTo, class TFrom>
inline TTo* Cast(TFrom* from) requires
	IDerivedFrom<TTo, Object> &&
	IDerivedFrom<TFrom, Object>
{
	return dynamic_cast<TTo*>(from);
}

template<class TTo, class TFrom>
inline const TTo* Cast(const TFrom* from) requires
	IDerivedFrom<TTo, Object> &&
	IDerivedFrom<TFrom, Object>
{
	return dynamic_cast<const TTo*>(from);
}

// ---------------------------------------------------

// --------------- Void to Object casts. -------------

template<class TTo>
inline TTo* Cast(void* from) requires
	IDerivedFrom<TTo, Object>
{
	return Cast<TTo>(reinterpret_cast<Object*>(from));
}

template<class TTo, class TFrom>
inline TTo Cast(TFrom* from) requires
	std::same_as<TTo, void*> &&
	IDerivedFrom<TFrom, Object>
{
	return reinterpret_cast<void*>(Cast<Object>(from));
}

// ---------------- shared_ptr casts. ----------------

template<class TTo, class TFrom>
inline std::shared_ptr<TTo> Cast(std::shared_ptr<TFrom> from)
{
	return std::dynamic_pointer_cast<TTo>(from);
}

template<class TTo, class TFrom>
inline std::shared_ptr<TTo> CastChecked(std::shared_ptr<TFrom> from)
{
	check(from.get() == nullptr || dynamic_cast<TTo*>(from.get()));
	return std::static_pointer_cast<TTo>(from);
}