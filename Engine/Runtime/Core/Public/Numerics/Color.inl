// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

namespace SC::Runtime::Core::Numerics
{
	template<TIsVectorType T>
	inline void Color::Construct(const T& vector)
	{
		R = vector.GetComponentOrDefault(0);
		G = vector.GetComponentOrDefault(1);
		B = vector.GetComponentOrDefault(2);
		A = vector.GetComponentOrDefault(3);
	}

	template<TIsVectorType T>
	inline T Color::Cast() const
	{
		T value;
		value.Construct(*this);
		return value;
	}
}