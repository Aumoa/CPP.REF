// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Numerics/TransformInterface/Quaternion.h"

namespace Ayla
{
	template<class T>
	struct OrientedCube
	{
		Vector3<T> Center;
		Vector3<T> Extent;
		Quaternion<T> Rotation;

		constexpr OrientedCube()
		{
		}

		constexpr OrientedCube(const Vector3<T>& C, const Vector3<T>& E, const Quaternion<T>& Q)
			: Center(C)
			, Extent(E)
			, Rotation(Q)
		{
		}

		static constexpr bool NearlyEquals(const OrientedCube& OOL, const OrientedCube& OOR, const T& Epsilon)
		{
			return Vector<>::NearlyEquals(OOL.Center, OOR.Center, Epsilon)
				&& Vector<>::NearlyEquals(OOL.Extent, OOR.Extent, Epsilon)
				&& Vector<>::NearlyEquals(OOL.Rotation, OOR.Rotation, Epsilon);
		}

		static String ToString(const OrientedCube& OO)
		{
			return String::Format(
				TEXT("Center: {}, Extent: {}, Rotation{}"),
				OO.Center.ToString(),
				OO.Extent.ToString(),
				OO.Rotation.ToString()
			);
		}

		constexpr auto operator <=>(const OrientedCube& O) const = default;
	};

	using OrientedCubeF = OrientedCube<float>;
	using OrientedCubeD = OrientedCube<double>;
}