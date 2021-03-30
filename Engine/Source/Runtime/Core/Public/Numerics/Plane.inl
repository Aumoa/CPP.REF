// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

template<TIsVectorType T>
inline void Plane::Construct(const T& vector)
{
	Normal.Construct(vector);
	Distance = vector.GetComponentOrDefault(3);
}

template<TIsVectorType T>
inline T Plane::Cast() const
{
	T value;
	value.Construct(*this);
	return value;
}