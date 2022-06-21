// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include <concepts>

// ------------- Object to Object casts. -------------

template<class TTo, class TFrom>
inline TTo* Cast(TFrom* from) requires
	std::derived_from<TTo, Object> &&
	std::derived_from<TFrom, Object>
{
	return dynamic_cast<TTo*>(from);
}

template<class TTo, class TFrom>
inline const TTo* Cast(const TFrom* from) requires
	std::derived_from<TTo, Object> &&
	std::derived_from<TFrom, Object>
{
	return dynamic_cast<const TTo*>(from);
}

// ---------------------------------------------------