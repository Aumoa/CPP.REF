// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

namespace SC::Runtime::Core::Mathematics
{
	template<TIsRealType T>
	inline constexpr TRadians<T> TDegrees<T>::ToRadians() const
	{
		return Value * InvPI;
	}

	template<TIsRealType T>
	inline constexpr TDegrees<T> TRadians<T>::ToDegrees() const
	{
		return Value * Inv180;
	}
}