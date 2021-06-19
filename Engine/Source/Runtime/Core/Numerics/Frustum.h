// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Plane.h"
#include <initializer_list>

/// <summary>
/// Represents camera frustum area.
/// </summary>
struct Frustum
{
	/// <summary>
	/// The six planes.
	/// </summary>
	Plane Planes[6];

	/// <summary>
	/// Initialize new <see cref="Frustum"/> instance.
	/// </summary>
	constexpr Frustum()
	{
	}

	/// <summary>
	/// Initialize new <see cref="Frustum"/> instance.
	/// </summary>
	/// <param name="planes"> The initial planes. </param>
	constexpr Frustum(std::initializer_list<Plane> planes)
	{
		for (size_t i = 0; i < planes.size(); ++i)
		{
			Planes[i] = planes.begin()[i];
		}
	}
};