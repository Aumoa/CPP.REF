// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:ComponentDirtyMask;

import SC.Runtime.Core;

/// <summary>
/// The component dirty mask.
/// </summary>
export enum class EComponentDirtyMask : uint32
{
	/// <summary>
	/// None value.
	/// </summary>
	None = 0,

	/// <summary>
	/// Need to recreate scene proxy.
	/// </summary>
	RecreateProxy = 1,
	
	/// <summary>
	/// Need to update scene proxy.
	/// </summary>
	UpdateProxy = RecreateProxy << 1,

	/// <summary>
	/// Transform updated.
	/// </summary>
	TransformUpdated = UpdateProxy << 1,

	/// <summary>
	/// All bits.
	/// </summary>
	All = 0xFFFFFFFF
};

export
{
	inline constexpr EComponentDirtyMask operator |(EComponentDirtyMask lh, EComponentDirtyMask rh)
	{
		return (EComponentDirtyMask)((uint32)lh | (uint32)rh);
	}

	inline constexpr EComponentDirtyMask operator &(EComponentDirtyMask lh, EComponentDirtyMask rh)
	{
		return (EComponentDirtyMask)((uint32)lh & (uint32)rh);
	}

	inline constexpr EComponentDirtyMask operator ~(EComponentDirtyMask un)
	{
		return (EComponentDirtyMask)~((uint32)un);
	}

	inline EComponentDirtyMask& operator |=(EComponentDirtyMask& lh, EComponentDirtyMask rh)
	{
		return lh = lh | rh;
	}

	inline EComponentDirtyMask& operator &=(EComponentDirtyMask& lh, EComponentDirtyMask rh)
	{
		return lh = lh & rh;
	}
}