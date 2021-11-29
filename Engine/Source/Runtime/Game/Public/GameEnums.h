// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

#pragma push_macro("DEFINE_ENUM_FLAG_OPERATORS")
#undef DEFINE_ENUM_FLAG_OPERATORS
#define DEFINE_ENUM_FLAG_OPERATORS(Enum) \
inline constexpr Enum operator |(Enum a, Enum b) { return Enum(((int32)a) | ((int32)b)); }\
inline Enum& operator |=(Enum& a, Enum b) { return (Enum&)(((int32 &)a) |= ((int32)b)); }\
inline constexpr Enum operator &(Enum a, Enum b) { return Enum(((int32)a) & ((int32)b)); }\
inline Enum& operator &=(Enum& a, Enum b) { return (Enum&)(((int32 &)a) &= ((int32)b)); }\
inline constexpr Enum operator ~(Enum a) { return Enum(~((int32)a)); }\
inline constexpr Enum operator ^(Enum a, Enum b) { return Enum(((int32)a) ^ ((int32)b)); }\
inline Enum& operator ^=(Enum& a, Enum b) { return (Enum&)(((int32 &)a) ^= ((int32)b)); }

/// <summary>
/// Describes how often this component is allowed to move.
/// </summary>
enum class EComponentMobility
{
	/// <summary>
	/// Static objects cannot be moved or changed in game.
	/// </summary>
	Static = 0,

	/// <summary>
	/// Movable objects can be moved and changed in game.
	/// </summary>
	Movable = 2
};

/// <summary>
/// Represents component transform space.
/// </summary>
enum class EComponentTransformSpace
{
	/// <summary>
	/// World space unit.
	/// </summary>
	World,

	/// <summary>
	/// Component space unit.
	/// </summary>
	Component
};

#undef DEFINE_ENUM_FLAG_OPERATORS
#pragma pop_macro("DEFINE_ENUM_FLAG_OPERATORS")