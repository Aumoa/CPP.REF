// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Numerics:ObjectOrientedCube;

export import Core;
export import :Vector;
export import :Quaternion;

export struct ObjectOrientedCube
{
	Vector3 Center;
	Vector3 Extent;
	Quaternion Rotation;

	constexpr ObjectOrientedCube()
	{
	}

	constexpr ObjectOrientedCube(const Vector3& C, const Vector3& E, const Quaternion& Q)
		: Center(C)
		, Extent(E)
		, Rotation(Q)
	{
	}

	static constexpr bool NearlyEquals(const ObjectOrientedCube& OOL, const ObjectOrientedCube& OOR, float Epsilon)
	{
		return Vector<>::NearlyEquals(OOL.Center, OOR.Center, Epsilon)
			&& Vector<>::NearlyEquals(OOL.Extent, OOR.Extent, Epsilon)
			&& Vector<>::NearlyEquals(OOL.Rotation, OOR.Rotation, Epsilon);
	}

	static String ToString(const ObjectOrientedCube& OO)
	{
		return String::Format(
			TEXT("Center: {}, Extent: {}, Rotation{}"),
			OO.Center.ToString(),
			OO.Extent.ToString(),
			OO.Rotation.ToString()
		);
	}

	constexpr auto operator <=>(const ObjectOrientedCube& O) const = default;
};