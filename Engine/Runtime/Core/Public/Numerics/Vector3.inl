// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

inline constexpr Vector3::SelectControl::SelectControl(bool v1, bool v2, bool v3)
	: Value(((char)v1 << 2) | ((char)v1 << 1) | ((char)v1 << 0))
{

}

inline constexpr auto Vector3::SelectControl::Less(const Vector3& lh, const Vector3& rh) -> SelectControl
{
	return SelectControl(lh.X < rh.X, lh.Y < rh.Y, lh.Z < rh.Z);
}

inline constexpr auto Vector3::SelectControl::Greater(const Vector3& lh, const Vector3& rh) -> SelectControl
{
	return SelectControl(lh.X > rh.X, lh.Y > rh.Y, lh.Z > rh.Z);
}

template<TIsVectorType T>
inline void Vector3::Construct(const T& vector)
{
	X = vector.GetComponentOrDefault(0);
	Y = vector.GetComponentOrDefault(1);
	Z = vector.GetComponentOrDefault(2);
}

template<TIsVectorType T>
inline T Vector3::Cast() const
{
	T value;
	value.Construct(*this);
	return value;
}