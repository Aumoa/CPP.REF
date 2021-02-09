// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

inline constexpr EMaterialDirtyMask operator |(EMaterialDirtyMask lh, EMaterialDirtyMask rh)
{
	return (EMaterialDirtyMask)((uint32)lh | (uint32)rh);
}

inline constexpr EMaterialDirtyMask operator &(EMaterialDirtyMask lh, EMaterialDirtyMask rh)
{
	return (EMaterialDirtyMask)((uint32)lh & (uint32)rh);
}

inline constexpr EMaterialDirtyMask operator ~(EMaterialDirtyMask un)
{
	return (EMaterialDirtyMask)~((uint32)un);
}

inline EMaterialDirtyMask& operator |=(EMaterialDirtyMask& lh, EMaterialDirtyMask rh)
{
	return lh = lh | rh;
}

inline EMaterialDirtyMask& operator &=(EMaterialDirtyMask& lh, EMaterialDirtyMask rh)
{
	return lh = lh & rh;
}