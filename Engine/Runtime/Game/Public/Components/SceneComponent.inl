// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

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