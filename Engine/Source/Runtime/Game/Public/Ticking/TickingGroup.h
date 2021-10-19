// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

/// <summary>
/// Represents ticking group.
/// </summary>
enum class ETickingGroup
{
	/// <summary>
	/// Tick update before physics updating.
	/// </summary>
	PrePhysics = 0,

	/// <summary>
	/// Tick update during physics updating.
	/// </summary>
	DuringPhysics = 1,

	/// <summary>
	/// Tick update after physics updated.
	/// </summary>
	PostPhysics = 2,

	/// <summary>
	/// Tick update after all other ticking groups are updated.
	/// </summary>
	PostUpdateWork = 3,
	NumGroups
};