// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_COMPILER_CLANG

#include "Platform/Generic/GenericPlatformAtomics.h"
#include "Platform/PlatformMacros.h"
#include "System/IntegralTypes.h"

struct CORE_API ClangPlatformAtomics : public GenericPlatformAtomics
{
	static FORCEINLINE int8 InterlockedIncrement(volatile int8* Value) noexcept
	{
		return __sync_fetch_and_add(Value, 1) + 1;
	}

	static FORCEINLINE uint8 InterlockedIncrement(volatile uint8* Value) noexcept
	{
		return __sync_fetch_and_add(Value, 1) + 1;
	}

	static FORCEINLINE int16 InterlockedIncrement(volatile int16* Value) noexcept
	{
		return __sync_fetch_and_add(Value, 1) + 1;
	}

	static FORCEINLINE uint16 InterlockedIncrement(volatile uint16* Value) noexcept
	{
		return __sync_fetch_and_add(Value, 1) + 1;
	}

	static FORCEINLINE int32 InterlockedIncrement(volatile int32* Value) noexcept
	{
		return __sync_fetch_and_add(Value, 1) + 1;
	}

	static FORCEINLINE uint32 InterlockedIncrement(volatile uint32* Value) noexcept
	{
		return __sync_fetch_and_add(Value, 1) + 1;
	}

	static FORCEINLINE int64 InterlockedIncrement(volatile int64* Value) noexcept
	{
		return __sync_fetch_and_add(Value, 1) + 1;
	}

	static FORCEINLINE uint64 InterlockedIncrement(volatile uint64* Value) noexcept
	{
		return __sync_fetch_and_add(Value, 1) + 1;
	}

	static FORCEINLINE int8 InterlockedDecrement(volatile int8* Value) noexcept
	{
		return __sync_fetch_and_sub(Value, 1) - 1;
	}

	static FORCEINLINE uint8 InterlockedDecrement(volatile uint8* Value) noexcept
	{
		return __sync_fetch_and_sub(Value, 1) - 1;
	}

	static FORCEINLINE int16 InterlockedDecrement(volatile int16* Value) noexcept
	{
		return __sync_fetch_and_sub(Value, 1) - 1;
	}

	static FORCEINLINE uint16 InterlockedDecrement(volatile uint16* Value) noexcept
	{
		return __sync_fetch_and_sub(Value, 1) - 1;
	}

	static FORCEINLINE int32 InterlockedDecrement(volatile int32* Value) noexcept
	{
		return __sync_fetch_and_sub(Value, 1) - 1;
	}

	static FORCEINLINE uint32 InterlockedDecrement(volatile uint32* Value) noexcept
	{
		return __sync_fetch_and_sub(Value, 1) - 1;
	}

	static FORCEINLINE int64 InterlockedDecrement(volatile int64* Value) noexcept
	{
		return __sync_fetch_and_sub(Value, 1) - 1;
	}

	static FORCEINLINE uint64 InterlockedDecrement(volatile uint64* Value) noexcept
	{
		return __sync_fetch_and_sub(Value, 1) - 1;
	}

	static FORCEINLINE int8 InterlockedAdd(volatile int8* Value, int8 Amount) noexcept
	{
		return __sync_fetch_and_add(Value, Amount);
	}

	static FORCEINLINE uint8 InterlockedAdd(volatile uint8* Value, uint8 Amount) noexcept
	{
		return __sync_fetch_and_add(Value, Amount);
	}

	static FORCEINLINE int16 InterlockedAdd(volatile int16* Value, int16 Amount) noexcept
	{
		return __sync_fetch_and_add(Value, Amount);
	}

	static FORCEINLINE uint16 InterlockedAdd(volatile uint16* Value, uint16 Amount) noexcept
	{
		return __sync_fetch_and_add(Value, Amount);
	}

	static FORCEINLINE int32 InterlockedAdd(volatile int32* Value, int32 Amount) noexcept
	{
		return __sync_fetch_and_add(Value, Amount);
	}

	static FORCEINLINE uint32 InterlockedAdd(volatile uint32* Value, uint32 Amount) noexcept
	{
		return __sync_fetch_and_add(Value, Amount);
	}

	static FORCEINLINE int64 InterlockedAdd(volatile int64* Value, int64 Amount) noexcept
	{
		return __sync_fetch_and_add(Value, Amount);
	}

	static FORCEINLINE uint64 InterlockedAdd(volatile uint64* Value, uint64 Amount) noexcept
	{
		return __sync_fetch_and_add(Value, Amount);
	}

	static FORCEINLINE int8 InterlockedExchange(volatile int8* Value, int8 Exchange) noexcept
	{
		return __sync_lock_test_and_set(Value, Exchange);
	}

	static FORCEINLINE uint8 InterlockedExchange(volatile uint8* Value, uint8 Exchange) noexcept
	{
		return __sync_lock_test_and_set(Value, Exchange);
	}

	static FORCEINLINE int16 InterlockedExchange(volatile int16* Value, int16 Exchange) noexcept
	{
		return __sync_lock_test_and_set(Value, Exchange);
	}

	static FORCEINLINE uint16 InterlockedExchange(volatile uint16* Value, uint16 Exchange) noexcept
	{
		return __sync_lock_test_and_set(Value, Exchange);
	}

	static FORCEINLINE int32 InterlockedExchange(volatile int32* Value, int32 Exchange) noexcept
	{
		return __sync_lock_test_and_set(Value, Exchange);
	}

	static FORCEINLINE uint32 InterlockedExchange(volatile uint32* Value, uint32 Exchange) noexcept
	{
		return __sync_lock_test_and_set(Value, Exchange);
	}

	static FORCEINLINE int64 InterlockedExchange(volatile int64* Value, int64 Exchange) noexcept
	{
		return __sync_lock_test_and_set(Value, Exchange);
	}

	static FORCEINLINE uint64 InterlockedExchange(volatile uint64* Value, uint64 Exchange) noexcept
	{
		return __sync_lock_test_and_set(Value, Exchange);
	}

	static FORCEINLINE void* InterlockedExchangePtr(void* volatile* Dest, void* Exchange) noexcept
	{
		return __sync_lock_test_and_set(Dest, Exchange);
	}

	static FORCEINLINE int8 InterlockedCompareExchange(volatile int8* Dest, int8 Exchange, int8 Comperand) noexcept
	{
		return __sync_val_compare_and_swap(Dest, Comperand, Exchange);
	}

	static FORCEINLINE uint8 InterlockedCompareExchange(volatile uint8* Dest, uint8 Exchange, uint8 Comperand) noexcept
	{
		return __sync_val_compare_and_swap(Dest, Comperand, Exchange);
	}

	static FORCEINLINE int16 InterlockedCompareExchange(volatile int16* Dest, int16 Exchange, int16 Comperand) noexcept
	{
		return __sync_val_compare_and_swap(Dest, Comperand, Exchange);
	}

	static FORCEINLINE uint16 InterlockedCompareExchange(volatile uint16* Dest, uint16 Exchange, uint16 Comperand) noexcept
	{
		return __sync_val_compare_and_swap(Dest, Comperand, Exchange);
	}

	static FORCEINLINE int32 InterlockedCompareExchange(volatile int32* Dest, int32 Exchange, int32 Comperand) noexcept
	{
		return __sync_val_compare_and_swap(Dest, Comperand, Exchange);
	}

	static FORCEINLINE uint32 InterlockedCompareExchange(volatile uint32* Dest, uint32 Exchange, uint32 Comperand) noexcept
	{
		return __sync_val_compare_and_swap(Dest, Comperand, Exchange);
	}

	static FORCEINLINE int64 InterlockedCompareExchange(volatile int64* Dest, int64 Exchange, int64 Comperand) noexcept
	{
		return __sync_val_compare_and_swap(Dest, Comperand, Exchange);
	}

	static FORCEINLINE uint64 InterlockedCompareExchange(volatile uint64* Dest, uint64 Exchange, uint64 Comperand) noexcept
	{
		return __sync_val_compare_and_swap(Dest, Comperand, Exchange);
	}

	static FORCEINLINE int8 InterlockedAnd(volatile int8* Value, const int8 AndValue) noexcept
	{
		return __sync_fetch_and_and(Value, AndValue);
	}

	static FORCEINLINE uint8 InterlockedAnd(volatile uint8* Value, const uint8 AndValue) noexcept
	{
		return __sync_fetch_and_and(Value, AndValue);
	}

	static FORCEINLINE int16 InterlockedAnd(volatile int16* Value, const int16 AndValue) noexcept
	{
		return __sync_fetch_and_and(Value, AndValue);
	}

	static FORCEINLINE uint16 InterlockedAnd(volatile uint16* Value, const uint16 AndValue) noexcept
	{
		return __sync_fetch_and_and(Value, AndValue);
	}

	static FORCEINLINE int32 InterlockedAnd(volatile int32* Value, const int32 AndValue) noexcept
	{
		return __sync_fetch_and_and(Value, AndValue);
	}

	static FORCEINLINE uint32 InterlockedAnd(volatile uint32* Value, const uint32 AndValue) noexcept
	{
		return __sync_fetch_and_and(Value, AndValue);
	}

	static FORCEINLINE int64 InterlockedAnd(volatile int64* Value, const int64 AndValue) noexcept
	{
		return __sync_fetch_and_and(Value, AndValue);
	}

	static FORCEINLINE uint64 InterlockedAnd(volatile uint64* Value, const uint64 AndValue) noexcept
	{
		return __sync_fetch_and_and(Value, AndValue);
	}

	static FORCEINLINE int8 InterlockedOr(volatile int8* Value, const int8 OrValue) noexcept
	{
		return __sync_fetch_and_or(Value, OrValue);
	}

	static FORCEINLINE uint8 InterlockedOr(volatile uint8* Value, const uint8 OrValue) noexcept
	{
		return __sync_fetch_and_or(Value, OrValue);
	}

	static FORCEINLINE int16 InterlockedOr(volatile int16* Value, const int16 OrValue) noexcept
	{
		return __sync_fetch_and_or(Value, OrValue);
	}

	static FORCEINLINE uint16 InterlockedOr(volatile uint16* Value, const uint16 OrValue) noexcept
	{
		return __sync_fetch_and_or(Value, OrValue);
	}

	static FORCEINLINE int32 InterlockedOr(volatile int32* Value, const int32 OrValue) noexcept
	{
		return __sync_fetch_and_or(Value, OrValue);
	}

	static FORCEINLINE uint32 InterlockedOr(volatile uint32* Value, const uint32 OrValue) noexcept
	{
		return __sync_fetch_and_or(Value, OrValue);
	}

	static FORCEINLINE int64 InterlockedOr(volatile int64* Value, const int64 OrValue) noexcept
	{
		return __sync_fetch_and_or(Value, OrValue);
	}

	static FORCEINLINE uint64 InterlockedOr(volatile uint64* Value, const uint64 OrValue) noexcept
	{
		return __sync_fetch_and_or(Value, OrValue);
	}

	static FORCEINLINE int8 InterlockedXor(volatile int8* Value, const int8 XorValue) noexcept
	{
		return __sync_fetch_and_xor(Value, XorValue);
	}

	static FORCEINLINE uint8 InterlockedXor(volatile uint8* Value, const uint8 XorValue) noexcept
	{
		return __sync_fetch_and_xor(Value, XorValue);
	}

	static FORCEINLINE int16 InterlockedXor(volatile int16* Value, const int16 XorValue) noexcept
	{
		return __sync_fetch_and_xor(Value, XorValue);
	}

	static FORCEINLINE uint16 InterlockedXor(volatile uint16* Value, const uint16 XorValue) noexcept
	{
		return __sync_fetch_and_xor(Value, XorValue);
	}

	static FORCEINLINE int32 InterlockedXor(volatile int32* Value, const int32 XorValue) noexcept
	{
		return __sync_fetch_and_xor(Value, XorValue);
	}

	static FORCEINLINE uint32 InterlockedXor(volatile uint32* Value, const uint32 XorValue) noexcept
	{
		return __sync_fetch_and_xor(Value, XorValue);
	}

	static FORCEINLINE int64 InterlockedXor(volatile int64* Value, const int64 XorValue) noexcept
	{
		return __sync_fetch_and_xor(Value, XorValue);
	}

	static FORCEINLINE uint64 InterlockedXor(volatile uint64* Value, const uint64 XorValue) noexcept
	{
		return __sync_fetch_and_xor(Value, XorValue);
	}

	static FORCEINLINE int8 AtomicRead(volatile const int8* Src) noexcept
	{
		int8 Result;
		__atomic_load((volatile int8*)Src, &Result, __ATOMIC_SEQ_CST);
		return Result;
	}

	static FORCEINLINE uint8 AtomicRead(volatile const uint8* Src) noexcept
	{
		uint8 Result;
		__atomic_load((volatile uint8*)Src, &Result, __ATOMIC_SEQ_CST);
		return Result;
	}

	static FORCEINLINE int16 AtomicRead(volatile const int16* Src) noexcept
	{
		int16 Result;
		__atomic_load((volatile int16*)Src, &Result, __ATOMIC_SEQ_CST);
		return Result;
	}

	static FORCEINLINE uint16 AtomicRead(volatile const uint16* Src) noexcept
	{
		uint16 Result;
		__atomic_load((volatile uint16*)Src, &Result, __ATOMIC_SEQ_CST);
		return Result;
	}

	static FORCEINLINE int32 AtomicRead(volatile const int32* Src) noexcept
	{
		int32 Result;
		__atomic_load((volatile int32*)Src, &Result, __ATOMIC_SEQ_CST);
		return Result;
	}

	static FORCEINLINE uint32 AtomicRead(volatile const uint32* Src) noexcept
	{
		uint32 Result;
		__atomic_load((volatile uint32*)Src, &Result, __ATOMIC_SEQ_CST);
		return Result;
	}

	static FORCEINLINE int64 AtomicRead(volatile const int64* Src) noexcept
	{
		int64 Result;
		__atomic_load((volatile int64*)Src, &Result, __ATOMIC_SEQ_CST);
		return Result;
	}

	static FORCEINLINE uint64 AtomicRead(volatile const uint64* Src) noexcept
	{
		uint64 Result;
		__atomic_load((volatile uint64*)Src, &Result, __ATOMIC_SEQ_CST);
		return Result;
	}

	static FORCEINLINE int8 AtomicRead_Relaxed(volatile const int8* Src) noexcept
	{
		int8 Result;
		__atomic_load((volatile int8*)Src, &Result, __ATOMIC_RELAXED);
		return Result;
	}

	static FORCEINLINE uint8 AtomicRead_Relaxed(volatile const uint8* Src) noexcept
	{
		uint8 Result;
		__atomic_load((volatile uint8*)Src, &Result, __ATOMIC_RELAXED);
		return Result;
	}

	static FORCEINLINE int16 AtomicRead_Relaxed(volatile const int16* Src) noexcept
	{
		int16 Result;
		__atomic_load((volatile int16*)Src, &Result, __ATOMIC_RELAXED);
		return Result;
	}

	static FORCEINLINE uint16 AtomicRead_Relaxed(volatile const uint16* Src) noexcept
	{
		uint16 Result;
		__atomic_load((volatile uint16*)Src, &Result, __ATOMIC_RELAXED);
		return Result;
	}

	static FORCEINLINE int32 AtomicRead_Relaxed(volatile const int32* Src) noexcept
	{
		int32 Result;
		__atomic_load((volatile int32*)Src, &Result, __ATOMIC_RELAXED);
		return Result;
	}

	static FORCEINLINE uint32 AtomicRead_Relaxed(volatile const uint32* Src) noexcept
	{
		uint32 Result;
		__atomic_load((volatile uint32*)Src, &Result, __ATOMIC_RELAXED);
		return Result;
	}

	static FORCEINLINE int64 AtomicRead_Relaxed(volatile const int64* Src) noexcept
	{
		int64 Result;
		__atomic_load((volatile int64*)Src, &Result, __ATOMIC_RELAXED);
		return Result;
	}

	static FORCEINLINE uint64 AtomicRead_Relaxed(volatile const uint64* Src) noexcept
	{
		uint64 Result;
		__atomic_load((volatile uint64*)Src, &Result, __ATOMIC_RELAXED);
		return Result;
	}

	static FORCEINLINE void AtomicStore(volatile int8* Src, int8 Val) noexcept
	{
		__atomic_store((volatile int8*)Src, &Val, __ATOMIC_SEQ_CST);
	}

	static FORCEINLINE void AtomicStore(volatile uint8* Src, uint8 Val) noexcept
	{
		__atomic_store((volatile uint8*)Src, &Val, __ATOMIC_SEQ_CST);
	}

	static FORCEINLINE void AtomicStore(volatile int16* Src, int16 Val) noexcept
	{
		__atomic_store((volatile int16*)Src, &Val, __ATOMIC_SEQ_CST);
	}

	static FORCEINLINE void AtomicStore(volatile uint16* Src, uint16 Val) noexcept
	{
		__atomic_store((volatile uint16*)Src, &Val, __ATOMIC_SEQ_CST);
	}

	static FORCEINLINE void AtomicStore(volatile int32* Src, int32 Val) noexcept
	{
		__atomic_store((volatile int32*)Src, &Val, __ATOMIC_SEQ_CST);
	}

	static FORCEINLINE void AtomicStore(volatile uint32* Src, uint32 Val) noexcept
	{
		__atomic_store((volatile uint32*)Src, &Val, __ATOMIC_SEQ_CST);
	}

	static FORCEINLINE void AtomicStore(volatile int64* Src, int64 Val) noexcept
	{
		__atomic_store((volatile int64*)Src, &Val, __ATOMIC_SEQ_CST);
	}

	static FORCEINLINE void AtomicStore(volatile uint64* Src, uint64 Val) noexcept
	{
		__atomic_store((volatile uint64*)Src, &Val, __ATOMIC_SEQ_CST);
	}

	static FORCEINLINE void AtomicStore_Relaxed(volatile int8* Src, int8 Val) noexcept
	{
		__atomic_store((volatile int8*)Src, &Val, __ATOMIC_RELAXED);
	}

	static FORCEINLINE void AtomicStore_Relaxed(volatile uint8* Src, uint8 Val) noexcept
	{
		__atomic_store((volatile uint8*)Src, &Val, __ATOMIC_RELAXED);
	}

	static FORCEINLINE void AtomicStore_Relaxed(volatile int16* Src, int16 Val) noexcept
	{
		__atomic_store((volatile int16*)Src, &Val, __ATOMIC_RELAXED);
	}

	static FORCEINLINE void AtomicStore_Relaxed(volatile uint16* Src, uint16 Val) noexcept
	{
		__atomic_store((volatile uint16*)Src, &Val, __ATOMIC_RELAXED);
	}

	static FORCEINLINE void AtomicStore_Relaxed(volatile int32* Src, int32 Val) noexcept
	{
		__atomic_store((volatile int32*)Src, &Val, __ATOMIC_RELAXED);
	}

	static FORCEINLINE void AtomicStore_Relaxed(volatile uint32* Src, uint32 Val) noexcept
	{
		__atomic_store((volatile uint32*)Src, &Val, __ATOMIC_RELAXED);
	}

	static FORCEINLINE void AtomicStore_Relaxed(volatile int64* Src, int64 Val) noexcept
	{
		__atomic_store((volatile int64*)Src, &Val, __ATOMIC_RELAXED);
	}

	static FORCEINLINE void AtomicStore_Relaxed(volatile uint64* Src, uint64 Val) noexcept
	{
		__atomic_store((volatile uint64*)Src, &Val, __ATOMIC_RELAXED);
	}

	static FORCEINLINE void* InterlockedCompareExchangePointer(void* volatile* Dest, void* Exchange, void* Comperand) noexcept
	{
		return __sync_val_compare_and_swap(Dest, Comperand, Exchange);
	}
};

#endif