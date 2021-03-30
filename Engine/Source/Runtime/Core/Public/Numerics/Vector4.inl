// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

template<TIsVectorType T>
inline void Vector4::Construct(const T& vector)
{
	X = vector.GetComponentOrDefault(0);
	Y = vector.GetComponentOrDefault(1);
	Z = vector.GetComponentOrDefault(2);
	W = vector.GetComponentOrDefault(3);
}

template<TIsVectorType T>
inline T Vector4::Cast() const
{
	T value;
	value.Construct(*this);
	return value;
}