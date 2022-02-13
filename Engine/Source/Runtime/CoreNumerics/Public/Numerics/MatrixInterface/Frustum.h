// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Numerics/VectorInterface/Plane.h"

struct Frustum
{
	Plane Planes[6];

	constexpr Frustum(const Plane& P0, const Plane& P1, const Plane& P2, const Plane& P3, const Plane& P4, const Plane& P5)
		: Planes{ P0, P1, P2, P3, P4, P5 }
	{
	}

	template<TIsMatrix<float, 6, 4> IMatrix>
	constexpr Frustum(const IMatrix& M)
		: Planes{ M[0], M[1], M[2], M[3], M[4], M[5] }
	{
	}

	constexpr Frustum(const Frustum& F)
		: Planes{ F[0], F[1], F[2], F[3], F[4], F[5] }
	{
	}

	constexpr Frustum& operator =(const Frustum& F)
	{
		Planes[0] = F[0];
		Planes[1] = F[1];
		Planes[2] = F[2];
		Planes[3] = F[3];
		Planes[4] = F[4];
		Planes[5] = F[5];
		return *this;
	}

public:
	using Type = float;
	using VectorType = Plane;

	constexpr Frustum(const VectorType& S)
		: Planes{ S, S, S, S, S, S }
	{
	}

	static constexpr size_t Row()
	{
		return 6;
	}

	static constexpr size_t Column()
	{
		return 4;
	}

	constexpr Frustum operator -() const
	{
		return Frustum(-Planes[0], -Planes[1], -Planes[2], -Planes[3], -Planes[4], -Planes[5]);
	}

	constexpr const VectorType& operator [](size_t N) const
	{
		return Planes[N];
	}

	constexpr VectorType& operator [](size_t N)
	{
		return Planes[N];
	}

	template<TIsMatrix<float, 4, 4> IMatrix>
	constexpr Frustum& operator =(const IMatrix& M)
	{
		Planes[0] = M[0];
		Planes[1] = M[1];
		Planes[2] = M[2];
		Planes[3] = M[3];
		Planes[4] = M[4];
		Planes[5] = M[5];
		return *this;
	}

	static constexpr Frustum Identity()
	{
		return Frustum
		{
			{ 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 0.0f }
		};
	}
};