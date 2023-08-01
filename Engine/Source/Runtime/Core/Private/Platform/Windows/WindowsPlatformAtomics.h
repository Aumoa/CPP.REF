// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

FORCEINLINE int8 PlatformAtomics::InterlockedIncrement(volatile int8* Value)
{
	return (int8)_InterlockedExchangeAdd8((volatile char*)Value, 1) + 1;
}

FORCEINLINE int16 PlatformAtomics::InterlockedIncrement(volatile int16* Value)
{
	return (int16)_InterlockedIncrement16((volatile short*)Value);
}

FORCEINLINE int32 PlatformAtomics::InterlockedIncrement(volatile int32* Value)
{
	return (int32)_InterlockedIncrement((volatile long*)Value);
}

FORCEINLINE int64 PlatformAtomics::InterlockedIncrement(volatile int64* Value)
{
	return (int64)_InlineInterlockedIncrement64((volatile long long*)Value);
}