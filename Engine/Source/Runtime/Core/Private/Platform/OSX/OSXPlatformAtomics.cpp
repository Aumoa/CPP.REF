// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Platform/OSX/OSXPlatformAtomics.h"

#if PLATFORM_OSX

#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <limits>
#include <algorithm>

namespace Ayla
{
    namespace
    {
        inline pthread_mutex_t* GetRWLock(void*& LockVal)
        {
            return reinterpret_cast<pthread_mutex_t*>(LockVal);
        }

        inline pthread_cond_t* GetCondVar(void*& CondVal)
        {
            return reinterpret_cast<pthread_cond_t*>(CondVal);
        }
    }

    void OSXPlatformAtomics::InitializeSpinlock(void*& LockVal) noexcept
    {
        auto* mutex = new pthread_mutex_t();
        pthread_mutex_init(mutex, nullptr);
        LockVal = mutex;
    }

    void OSXPlatformAtomics::DestroySpinlock(void*& LockVal) noexcept
    {
        if (LockVal)
        {
            pthread_mutex_destroy(GetRWLock(LockVal));
            delete GetRWLock(LockVal);
            LockVal = nullptr;
        }
    }

    void OSXPlatformAtomics::AcquireSpinlock(void*& LockVal, bool bShared) noexcept
    {
        PLATFORM_UNREFERENCED_PARAMETER(bShared);
        pthread_mutex_lock(GetRWLock(LockVal));
    }

    void OSXPlatformAtomics::ReleaseSpinlock(void*& LockVal, bool bShared) noexcept
    {
        PLATFORM_UNREFERENCED_PARAMETER(bShared);
        pthread_mutex_unlock(GetRWLock(LockVal));
    }

    bool OSXPlatformAtomics::TryAcquireSpinlock(void*& LockVal, bool bShared) noexcept
    {
        PLATFORM_UNREFERENCED_PARAMETER(bShared);
        return pthread_mutex_trylock(GetRWLock(LockVal)) == 0;
    }

    void OSXPlatformAtomics::InitializeSpinlockConditionVariable(void*& CondVal) noexcept
    {
        auto* cond = new pthread_cond_t();
        pthread_cond_init(cond, nullptr);
        CondVal = cond;
    }

    void OSXPlatformAtomics::DestroySpinlockConditionVariable(void*& CondVal) noexcept
    {
        if (CondVal)
        {
            pthread_cond_destroy(GetCondVar(CondVal));
            delete GetCondVar(CondVal);
            CondVal = nullptr;
        }
    }

    void OSXPlatformAtomics::WaitSpinlockConditionVariable(void*& CondVal, void*& LockVal, bool bShared) noexcept
    {
        PLATFORM_UNREFERENCED_PARAMETER(bShared);
        pthread_cond_wait(GetCondVar(CondVal), GetRWLock(LockVal));
    }

    bool OSXPlatformAtomics::WaitForSpinlockConditionVariable(void*& CondVal, void*& LockVal, size_t Sleep, bool bShared) noexcept
    {
        PLATFORM_UNREFERENCED_PARAMETER(bShared);
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        size_t sec = Sleep / 1000;
        size_t nsec = (Sleep % 1000) * 1000000;
        ts.tv_sec += sec;
        ts.tv_nsec += nsec;
        if (ts.tv_nsec >= 1000000000)
        {
            ts.tv_sec += 1;
            ts.tv_nsec -= 1000000000;
        }
        int ret = pthread_cond_timedwait(GetCondVar(CondVal), GetRWLock(LockVal), &ts);
        return ret == 0;
    }

    void OSXPlatformAtomics::NotifyOneSpinlockConditionVariable(void*& CondVal) noexcept
    {
        pthread_cond_signal(GetCondVar(CondVal));
    }

    void OSXPlatformAtomics::NotifyAllSpinlockConditionVariable(void*& CondVal) noexcept
    {
        pthread_cond_broadcast(GetCondVar(CondVal));
    }
}

#endif
