// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

namespace SC::Runtime::Core::Numerics
{
	template<TIsVectorType T>
	inline void Rectangle::Construct(const T& vector)
	{
		Left = vector.GetComponentOrDefault(0);
		Top = vector.GetComponentOrDefault(1);
		Right = vector.GetComponentOrDefault(2);
		Bottom = vector.GetComponentOrDefault(3);
	}

	template<TIsVectorType T>
	inline T Rectangle::Cast() const
	{
		T value;
		value.Construct(*this);
		return value;
	}
}