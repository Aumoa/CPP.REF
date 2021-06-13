// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:ComponentMobility;

/// <summary>
/// Describes how often this component is allowed to move.
/// </summary>
export enum class EComponentMobility
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