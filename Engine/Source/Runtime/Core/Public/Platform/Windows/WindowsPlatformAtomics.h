// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_WINDOWS

#include "Platform/Generic/GenericPlatformAtomics.h"
#include "Platform/PlatformMacros.h"
#include "System/IntegralTypes.h"

struct CORE_API WindowsPlatformAtomics : public GenericPlatformAtomics
{
	static FORCEINLINE int8 InterlockedIncrement(volatile int8* Value) noexcept
	{
		return (int8)::_InterlockedExchangeAdd8((volatile char*)Value, 1) + 1;
	}

	static FORCEINLINE uint8 InterlockedIncrement(volatile uint8* Value) noexcept
	{
		return (uint8)::_InterlockedExchangeAdd8((volatile char*)Value, 1) + 1;
	}

	static FORCEINLINE int16 InterlockedIncrement(volatile int16* Value) noexcept
	{
		return (int16)::_InterlockedIncrement16((volatile short*)Value);
	}

	static FORCEINLINE uint16 InterlockedIncrement(volatile uint16* Value) noexcept
	{
		return (uint16)::_InterlockedIncrement16((volatile short*)Value);
	}

	static FORCEINLINE int32 InterlockedIncrement(volatile int32* Value) noexcept
	{
		return (int32)::_InterlockedIncrement((volatile long*)Value);
	}

	static FORCEINLINE uint32 InterlockedIncrement(volatile uint32* Value) noexcept
	{
		return (uint32)::_InterlockedIncrement((volatile long*)Value);
	}

	static FORCEINLINE int64 InterlockedIncrement(volatile int64* Value) noexcept
	{
		return (int64)::_InterlockedIncrement64((volatile long long*)Value);
	}

	static FORCEINLINE int8 InterlockedDecrement(volatile int8* Value) noexcept
	{
		return (int8)::_InterlockedExchangeAdd8((volatile char*)Value, -1) - 1;
	}

	static FORCEINLINE uint8 InterlockedDecrement(volatile uint8* Value) noexcept
	{
		return (uint8)::_InterlockedExchangeAdd8((volatile char*)Value, -1) - 1;
	}

	static FORCEINLINE int16 InterlockedDecrement(volatile int16* Value) noexcept
	{
		return (int16)::_InterlockedDecrement16((volatile short*)Value);
	}

	static FORCEINLINE uint16 InterlockedDecrement(volatile uint16* Value) noexcept
	{
		return (uint16)::_InterlockedDecrement16((volatile short*)Value);
	}

	static FORCEINLINE int32 InterlockedDecrement(volatile int32* Value) noexcept
	{
		return (int32)::_InterlockedDecrement((volatile long*)Value);
	}

	static FORCEINLINE uint32 InterlockedDecrement(volatile uint32* Value) noexcept
	{
		return (uint32)::_InterlockedDecrement((volatile long*)Value);
	}

	static FORCEINLINE int64 InterlockedDecrement(volatile int64* Value) noexcept
	{
		return (int64)::_InterlockedDecrement64((volatile long long*)Value);
	}

	static FORCEINLINE uint64 InterlockedDecrement(volatile uint64* Value) noexcept
	{
		return (uint64)::_InterlockedDecrement64((volatile long long*)Value);
	}

	static FORCEINLINE int8 InterlockedAdd(volatile int8* Value, int8 Amount) noexcept
	{
		return (int8)::_InterlockedExchangeAdd8((volatile char*)Value, (char)Amount);
	}

	static FORCEINLINE uint8 InterlockedAdd(volatile uint8* Value, uint8 Amount) noexcept
	{
		return (uint8)::_InterlockedExchangeAdd8((volatile char*)Value, (char)Amount);
	}

	static FORCEINLINE int16 InterlockedAdd(volatile int16* Value, int16 Amount) noexcept
	{
		return (int16)::_InterlockedExchangeAdd16((volatile short*)Value, (short)Amount);
	}

	static FORCEINLINE uint16 InterlockedAdd(volatile uint16* Value, uint16 Amount) noexcept
	{
		return (uint16)::_InterlockedExchangeAdd16((volatile short*)Value, (short)Amount);
	}

	static FORCEINLINE int32 InterlockedAdd(volatile int32* Value, int32 Amount) noexcept
	{
		return (int32)::_InterlockedExchangeAdd((volatile long*)Value, (long)Amount);
	}

	static FORCEINLINE uint32 InterlockedAdd(volatile uint32* Value, uint32 Amount) noexcept
	{
		return (uint32)::_InterlockedExchangeAdd((volatile long*)Value, (long)Amount);
	}

	static FORCEINLINE int64 InterlockedAdd(volatile int64* Value, int64 Amount) noexcept
	{
		return (int64)::_InterlockedExchangeAdd64((volatile int64*)Value, (int64)Amount);
	}

	static FORCEINLINE uint64 InterlockedAdd(volatile uint64* Value, uint64 Amount) noexcept
	{
		return (uint64)::_InterlockedExchangeAdd64((volatile int64*)Value, (int64)Amount);
	}

	static FORCEINLINE int8 InterlockedExchange(volatile int8* Value, int8 Exchange) noexcept
	{
		return (int8)::_InterlockedExchange8((volatile char*)Value, (char)Exchange);
	}

	static FORCEINLINE int8 InterlockedExchange(volatile uint8* Value, uint8 Exchange) noexcept
	{
		return (uint8)::_InterlockedExchange8((volatile char*)Value, (char)Exchange);
	}

	static FORCEINLINE int16 InterlockedExchange(volatile int16* Value, int16 Exchange) noexcept
	{
		return (int16)::_InterlockedExchange16((volatile short*)Value, (short)Exchange);
	}

	static FORCEINLINE uint16 InterlockedExchange(volatile uint16* Value, uint16 Exchange) noexcept
	{
		return (uint16)::_InterlockedExchange16((volatile short*)Value, (short)Exchange);
	}

	static FORCEINLINE int32 InterlockedExchange(volatile int32* Value, int32 Exchange) noexcept
	{
		return (int32)::_InterlockedExchange((volatile long*)Value, (long)Exchange);
	}

	static FORCEINLINE uint32 InterlockedExchange(volatile uint32* Value, uint32 Exchange) noexcept
	{
		return (uint32)::_InterlockedExchange((volatile long*)Value, (long)Exchange);
	}

	static FORCEINLINE int64 InterlockedExchange(volatile int64* Value, int64 Exchange) noexcept
	{
		return (int64)::_InterlockedExchange64((volatile long long*)Value, (long long)Exchange);
	}

	static FORCEINLINE uint64 InterlockedExchange(volatile uint64* Value, uint64 Exchange) noexcept
	{
		return (uint64)::_InterlockedExchange64((volatile long long*)Value, (long long)Exchange);
	}

	static FORCEINLINE void* InterlockedExchangePtr(void* volatile* Dest, void* Exchange) noexcept
	{
		return ::_InterlockedExchangePointer(Dest, Exchange);
	}

	static FORCEINLINE int8 InterlockedCompareExchange(volatile int8* Dest, int8 Exchange, int8 Comparand) noexcept
	{
		return (int8)::_InterlockedCompareExchange8((volatile char*)Dest, (char)Exchange, (char)Comparand);
	}

	static FORCEINLINE uint8 InterlockedCompareExchange(volatile uint8* Dest, uint8 Exchange, uint8 Comparand) noexcept
	{
		return (uint8)::_InterlockedCompareExchange8((volatile char*)Dest, (char)Exchange, (char)Comparand);
	}

	static FORCEINLINE int16 InterlockedCompareExchange(volatile int16* Dest, int16 Exchange, int16 Comparand) noexcept
	{
		return (int16)::_InterlockedCompareExchange16((volatile short*)Dest, (short)Exchange, (short)Comparand);
	}

	static FORCEINLINE uint16 InterlockedCompareExchange(volatile uint16* Dest, uint16 Exchange, uint16 Comparand) noexcept
	{
		return (uint16)::_InterlockedCompareExchange16((volatile short*)Dest, (short)Exchange, (short)Comparand);
	}

	static FORCEINLINE int32 InterlockedCompareExchange(volatile int32* Dest, int32 Exchange, int32 Comparand) noexcept
	{
		return (int32)::_InterlockedCompareExchange((volatile long*)Dest, (long)Exchange, (long)Comparand);
	}

	static FORCEINLINE uint32 InterlockedCompareExchange(volatile uint32* Dest, uint32 Exchange, uint32 Comparand) noexcept
	{
		return (uint32)::_InterlockedCompareExchange((volatile long*)Dest, (long)Exchange, (long)Comparand);
	}

	static FORCEINLINE int64 InterlockedCompareExchange(volatile int64* Dest, int64 Exchange, int64 Comparand) noexcept
	{
		return (int64)::_InterlockedCompareExchange64(Dest, Exchange, Comparand);
	}

	static FORCEINLINE uint64 InterlockedCompareExchange(volatile uint64* Dest, uint64 Exchange, uint64 Comparand) noexcept
	{
		return (uint64)::_InterlockedCompareExchange64((volatile int64*)Dest, Exchange, Comparand);
	}

	static FORCEINLINE int8 InterlockedAnd(volatile int8* Value, const int8 AndValue) noexcept
	{
		return (int8)::_InterlockedAnd8((volatile char*)Value, (char)AndValue);
	}

	static FORCEINLINE uint8 InterlockedAnd(volatile uint8* Value, const uint8 AndValue) noexcept
	{
		return (uint8)::_InterlockedAnd8((volatile char*)Value, (char)AndValue);
	}

	static FORCEINLINE int16 InterlockedAnd(volatile int16* Value, const int16 AndValue) noexcept
	{
		return (int16)::_InterlockedAnd16((volatile short*)Value, (short)AndValue);
	}

	static FORCEINLINE uint16 InterlockedAnd(volatile uint16* Value, const uint16 AndValue) noexcept
	{
		return (uint16)::_InterlockedAnd16((volatile short*)Value, (short)AndValue);
	}

	static FORCEINLINE int32 InterlockedAnd(volatile int32* Value, const int32 AndValue) noexcept
	{
		return (int32)::_InterlockedAnd((volatile long*)Value, (long)AndValue);
	}

	static FORCEINLINE uint32 InterlockedAnd(volatile uint32* Value, const uint32 AndValue) noexcept
	{
		return (uint32)::_InterlockedAnd((volatile long*)Value, (long)AndValue);
	}

	static FORCEINLINE int64 InterlockedAnd(volatile int64* Value, const int64 AndValue) noexcept
	{
		return (int64)::_InterlockedAnd64((volatile long long*)Value, (long long)AndValue);
	}

	static FORCEINLINE uint64 InterlockedAnd(volatile uint64* Value, const uint64 AndValue) noexcept
	{
		return (uint64)::_InterlockedAnd64((volatile long long*)Value, (long long)AndValue);
	}

	static FORCEINLINE int8 InterlockedOr(volatile int8* Value, const int8 OrValue) noexcept
	{
		return (int8)::_InterlockedOr8((volatile char*)Value, (char)OrValue);
	}

	static FORCEINLINE uint8 InterlockedOr(volatile uint8* Value, const uint8 OrValue) noexcept
	{
		return (uint8)::_InterlockedOr8((volatile char*)Value, (char)OrValue);
	}

	static FORCEINLINE int16 InterlockedOr(volatile int16* Value, const int16 OrValue) noexcept
	{
		return (int16)::_InterlockedOr16((volatile short*)Value, (short)OrValue);
	}

	static FORCEINLINE uint16 InterlockedOr(volatile uint16* Value, const uint16 OrValue) noexcept
	{
		return (uint16)::_InterlockedOr16((volatile short*)Value, (short)OrValue);
	}

	static FORCEINLINE int32 InterlockedOr(volatile int32* Value, const int32 OrValue) noexcept
	{
		return (int32)::_InterlockedOr((volatile long*)Value, (long)OrValue);
	}

	static FORCEINLINE uint32 InterlockedOr(volatile uint32* Value, const uint32 OrValue) noexcept
	{
		return (uint32)::_InterlockedOr((volatile long*)Value, (long)OrValue);
	}

	static FORCEINLINE int64 InterlockedOr(volatile int64* Value, const int64 OrValue) noexcept
	{
		return (int64)::_InterlockedOr64((volatile long long*)Value, (long long)OrValue);
	}

	static FORCEINLINE uint64 InterlockedOr(volatile uint64* Value, const uint64 OrValue) noexcept
	{
		return (uint64)::_InterlockedOr64((volatile long long*)Value, (long long)OrValue);
	}

	static FORCEINLINE int8 InterlockedXor(volatile int8* Value, const int8 XorValue) noexcept
	{
		return (int8)::_InterlockedXor8((volatile char*)Value, (char)XorValue);
	}

	static FORCEINLINE uint8 InterlockedXor(volatile uint8* Value, const uint8 XorValue) noexcept
	{
		return (uint8)::_InterlockedXor8((volatile char*)Value, (char)XorValue);
	}

	static FORCEINLINE int16 InterlockedXor(volatile int16* Value, const int16 XorValue) noexcept
	{
		return (int16)::_InterlockedXor16((volatile short*)Value, (short)XorValue);
	}

	static FORCEINLINE uint16 InterlockedXor(volatile uint16* Value, const uint16 XorValue) noexcept
	{
		return (uint16)::_InterlockedXor16((volatile short*)Value, (short)XorValue);
	}

	static FORCEINLINE int32 InterlockedXor(volatile int32* Value, const int32 XorValue) noexcept
	{
		return (int32)::_InterlockedXor((volatile long*)Value, (int32)XorValue);
	}

	static FORCEINLINE uint32 InterlockedXor(volatile uint32* Value, const uint32 XorValue) noexcept
	{
		return (uint32)::_InterlockedXor((volatile long*)Value, (int32)XorValue);
	}

	static FORCEINLINE int64 InterlockedXor(volatile int64* Value, const int64 XorValue) noexcept
	{
		return (int64)::_InterlockedXor64((volatile long long*)Value, (long long)XorValue);
	}

	static FORCEINLINE uint64 InterlockedXor(volatile uint64* Value, const uint64 XorValue) noexcept
	{
		return (uint64)::_InterlockedXor64((volatile long long*)Value, (long long)XorValue);
	}

	static FORCEINLINE int8 AtomicRead(volatile const int8* Src) noexcept
	{
		return InterlockedCompareExchange((volatile int8*)Src, 0, 0);
	}

	static FORCEINLINE uint8 AtomicRead(volatile const uint8* Src) noexcept
	{
		return InterlockedCompareExchange((volatile int8*)Src, 0, 0);
	}

	static FORCEINLINE int16 AtomicRead(volatile const int16* Src) noexcept
	{
		return InterlockedCompareExchange((volatile int16*)Src, 0, 0);
	}

	static FORCEINLINE uint16 AtomicRead(volatile const uint16* Src) noexcept
	{
		return InterlockedCompareExchange((volatile int16*)Src, 0, 0);
	}

	static FORCEINLINE int32 AtomicRead(volatile const int32* Src) noexcept
	{
		return InterlockedCompareExchange((volatile int32*)Src, 0, 0);
	}

	static FORCEINLINE uint32 AtomicRead(volatile const uint32* Src) noexcept
	{
		return InterlockedCompareExchange((volatile uint32*)Src, 0, 0);
	}

	static FORCEINLINE int64 AtomicRead(volatile const int64* Src) noexcept
	{
		return InterlockedCompareExchange((volatile int64*)Src, 0, 0);
	}

	static FORCEINLINE uint64 AtomicRead(volatile const uint64* Src) noexcept
	{
		return InterlockedCompareExchange((volatile int64*)Src, 0, 0);
	}

	static FORCEINLINE int8 AtomicRead_Relaxed(volatile const int8* Src) noexcept
	{
		return *Src;
	}

	static FORCEINLINE uint8 AtomicRead_Relaxed(volatile const uint8* Src) noexcept
	{
		return *Src;
	}

	static FORCEINLINE int16 AtomicRead_Relaxed(volatile const int16* Src) noexcept
	{
		return *Src;
	}

	static FORCEINLINE uint16 AtomicRead_Relaxed(volatile const uint16* Src) noexcept
	{
		return *Src;
	}

	static FORCEINLINE int32 AtomicRead_Relaxed(volatile const int32* Src) noexcept
	{
		return *Src;
	}

	static FORCEINLINE uint32 AtomicRead_Relaxed(volatile const uint32* Src) noexcept
	{
		return *Src;
	}

	static FORCEINLINE int64 AtomicRead_Relaxed(volatile const int64* Src) noexcept
	{
		return *Src;
	}

	static FORCEINLINE uint64 AtomicRead_Relaxed(volatile const uint64* Src) noexcept
	{
		return *Src;
	}

	static FORCEINLINE void AtomicStore(volatile int8* Src, int8 Val) noexcept
	{
		InterlockedExchange(Src, Val);
	}

	static FORCEINLINE void AtomicStore(volatile uint8* Src, uint8 Val) noexcept
	{
		InterlockedExchange(Src, Val);
	}

	static FORCEINLINE void AtomicStore(volatile int16* Src, int16 Val) noexcept
	{
		InterlockedExchange(Src, Val);
	}

	static FORCEINLINE void AtomicStore(volatile uint16* Src, uint16 Val) noexcept
	{
		InterlockedExchange(Src, Val);
	}

	static FORCEINLINE void AtomicStore(volatile int32* Src, int32 Val) noexcept
	{
		InterlockedExchange(Src, Val);
	}

	static FORCEINLINE void AtomicStore(volatile uint32* Src, uint32 Val) noexcept
	{
		InterlockedExchange(Src, Val);
	}

	static FORCEINLINE void AtomicStore(volatile int64* Src, int64 Val) noexcept
	{
		InterlockedExchange(Src, Val);
	}

	static FORCEINLINE void AtomicStore(volatile uint64* Src, uint64 Val) noexcept
	{
		InterlockedExchange(Src, Val);
	}

	static FORCEINLINE void AtomicStore_Relaxed(volatile int8* Src, int8 Val) noexcept
	{
		*Src = Val;
	}

	static FORCEINLINE void AtomicStore_Relaxed(volatile uint8* Src, uint8 Val) noexcept
	{
		*Src = Val;
	}

	static FORCEINLINE void AtomicStore_Relaxed(volatile int16* Src, int16 Val) noexcept
	{
		*Src = Val;
	}

	static FORCEINLINE void AtomicStore_Relaxed(volatile uint16* Src, uint16 Val) noexcept
	{
		*Src = Val;
	}

	static FORCEINLINE void AtomicStore_Relaxed(volatile int32* Src, int32 Val) noexcept
	{
		*Src = Val;
	}

	static FORCEINLINE void AtomicStore_Relaxed(volatile uint32* Src, uint32 Val) noexcept
	{
		*Src = Val;
	}

	static FORCEINLINE void AtomicStore_Relaxed(volatile int64* Src, int64 Val) noexcept
	{
		*Src = Val;
	}

	static FORCEINLINE void AtomicStore_Relaxed(volatile uint64* Src, uint64 Val) noexcept
	{
		*Src = Val;
	}

	static FORCEINLINE void* InterlockedCompareExchangePointer(void* volatile* Dest, void* Exchange, void* Comparand) noexcept
	{
		return ::_InterlockedCompareExchangePointer(Dest, Exchange, Comparand);
	}

	static void InitializeSpinlock(void*& LockVal) noexcept;
	static void DestroySpinlock(void*& LockVal) noexcept;
	static void AcquireSpinlock(void*& LockVal, bool bShared) noexcept;
	static void ReleaseSpinlock(void*& LockVal, bool bShared) noexcept;
	static bool TryAcquireSpinlock(void*& LockVal, bool bShared) noexcept;

	static void InitializeSpinlockConditionVariable(void*& CondVal) noexcept;
	static void DestroySpinlockConditionVariable(void*& CondVal) noexcept;
	static void WaitSpinlockConditionVariable(void*& CondVal, void*& LockVal, bool bShared) noexcept;
	static bool WaitForSpinlockConditionVariable(void*& CondVal, void*& LockVal, size_t Sleep, bool bShared) noexcept;
	static void NotifyOneSpinlockConditionVariable(void*& CondVal) noexcept;
	static void NotifyAllSpinlockConditionVariable(void*& CondVal) noexcept;
};

using PlatformAtomics = WindowsPlatformAtomics;

#endif