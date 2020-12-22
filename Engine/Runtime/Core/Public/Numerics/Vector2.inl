// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

namespace SC::Runtime::Core::Numerics
{
	template<TIsVectorType T>
	inline void Vector2::Construct(const T& vector)
	{
		X = vector.GetComponentOrDefault(0);
		Y = vector.GetComponentOrDefault(1);
	}

	template<TIsVectorType T>
	inline T Vector2::Cast() const
	{
		T value;
		value.Construct(*this);
		return value;
	}
}