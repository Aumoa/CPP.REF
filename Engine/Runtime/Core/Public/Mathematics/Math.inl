// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

namespace SC::Runtime::Core::Mathematics
{
	template<TIsRealType T>
	template<TIsRealType O>
	inline constexpr TRadians<O> TDegrees<T>::ToRadians() const
	{
		return Value * InvPI;
	}

	template<TIsRealType T>
	template<TIsRealType O>
	inline constexpr TDegrees<O> TRadians<T>::ToDegrees() const
	{
		return static_cast<O>(Value) * static_cast<O>(Inv180);
	}
}