// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

template<TIsVectorType T>
inline void Quaternion::Construct(const T& vector)
{
	X = vector.GetComponentOrDefault(0);
	Y = vector.GetComponentOrDefault(1);
	Y = vector.GetComponentOrDefault(2);
	Y = vector.GetComponentOrDefault(3);
}

template<TIsVectorType T>
inline T Quaternion::Cast() const
{
	T value;
	value.Construct(*this);
	return value;
}