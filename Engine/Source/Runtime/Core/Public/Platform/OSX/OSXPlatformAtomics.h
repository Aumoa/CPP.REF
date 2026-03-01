// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_OSX

#include "Platform/Unix/UnixPlatformAtomics.h"
#include "IntegralTypes.h"

namespace Ayla
{
    struct CORE_API OSXPlatformAtomics : public UnixPlatformAtomics
    {
        static FORCEINLINE ssize_t InterlockedIncrement(volatile ssize_t* Value) noexcept
        {
            return __sync_add_and_fetch(Value, 1);
        }
        static FORCEINLINE size_t InterlockedIncrement(volatile size_t* Value) noexcept
        {
            return __sync_add_and_fetch(Value, 1);
        }

        static FORCEINLINE ssize_t InterlockedDecrement(volatile ssize_t* Value) noexcept
        {
            return __sync_sub_and_fetch(Value, 1);
        }
        static FORCEINLINE size_t InterlockedDecrement(volatile size_t* Value) noexcept
        {
            return __sync_sub_and_fetch(Value, 1);
        }

        static FORCEINLINE ssize_t InterlockedAdd(volatile ssize_t* Value, ssize_t Amount) noexcept
        {
            return __sync_fetch_and_add(Value, Amount);
        }
        static FORCEINLINE size_t InterlockedAdd(volatile size_t* Value, size_t Amount) noexcept
        {
            return __sync_fetch_and_add(Value, Amount);
        }

        static FORCEINLINE ssize_t InterlockedExchange(volatile ssize_t* Value, ssize_t Exchange) noexcept
        {
            return __sync_lock_test_and_set(Value, Exchange);
        }
        static FORCEINLINE size_t InterlockedExchange(volatile size_t* Value, size_t Exchange) noexcept
        {
            return __sync_lock_test_and_set(Value, Exchange);
        }

        static FORCEINLINE ssize_t InterlockedCompareExchange(volatile ssize_t* Dest, ssize_t Exchange, ssize_t Comparand) noexcept
        {
            return __sync_val_compare_and_swap(Dest, Comparand, Exchange);
        }
        static FORCEINLINE size_t InterlockedCompareExchange(volatile size_t* Dest, size_t Exchange, size_t Comparand) noexcept
        {
            return __sync_val_compare_and_swap(Dest, Comparand, Exchange);
        }

        static FORCEINLINE ssize_t InterlockedAnd(volatile ssize_t* Value, const ssize_t AndValue) noexcept
        {
            return __sync_fetch_and_and(Value, AndValue);
        }
        static FORCEINLINE size_t InterlockedAnd(volatile size_t* Value, const size_t AndValue) noexcept
        {
            return __sync_fetch_and_and(Value, AndValue);
        }

        static FORCEINLINE ssize_t InterlockedOr(volatile ssize_t* Value, const ssize_t OrValue) noexcept
        {
            return __sync_fetch_and_or(Value, OrValue);
        }
        static FORCEINLINE size_t InterlockedOr(volatile size_t* Value, const size_t OrValue) noexcept
        {
            return __sync_fetch_and_or(Value, OrValue);
        }

        static FORCEINLINE ssize_t InterlockedXor(volatile ssize_t* Value, const ssize_t XorValue) noexcept
        {
            return __sync_fetch_and_xor(Value, XorValue);
        }
        static FORCEINLINE size_t InterlockedXor(volatile size_t* Value, const size_t XorValue) noexcept
        {
            return __sync_fetch_and_xor(Value, XorValue);
        }

        static FORCEINLINE ssize_t AtomicRead(volatile const ssize_t* Src) noexcept
        {
            return __atomic_load_n(Src, __ATOMIC_SEQ_CST);
        }
        static FORCEINLINE size_t AtomicRead(volatile const size_t* Src) noexcept
        {
            return __atomic_load_n(Src, __ATOMIC_SEQ_CST);
        }

        static FORCEINLINE ssize_t AtomicRead_Relaxed(volatile const ssize_t* Src) noexcept
        {
            return __atomic_load_n(Src, __ATOMIC_RELAXED);
        }
        static FORCEINLINE size_t AtomicRead_Relaxed(volatile const size_t* Src) noexcept
        {
            return __atomic_load_n(Src, __ATOMIC_RELAXED);
        }

        static FORCEINLINE void AtomicStore(volatile ssize_t* Src, ssize_t Val) noexcept
        {
            __atomic_store_n(Src, Val, __ATOMIC_SEQ_CST);
        }
        static FORCEINLINE void AtomicStore(volatile size_t* Src, size_t Val) noexcept
        {
            __atomic_store_n(Src, Val, __ATOMIC_SEQ_CST);
        }

        static FORCEINLINE void AtomicStore_Relaxed(volatile ssize_t* Src, ssize_t Val) noexcept
        {
            __atomic_store_n(Src, Val, __ATOMIC_RELAXED);
        }
        static FORCEINLINE void AtomicStore_Relaxed(volatile size_t* Src, size_t Val) noexcept
        {
            __atomic_store_n(Src, Val, __ATOMIC_RELAXED);
        }
    };

    using PlatformAtomics = OSXPlatformAtomics;
}

#endif
