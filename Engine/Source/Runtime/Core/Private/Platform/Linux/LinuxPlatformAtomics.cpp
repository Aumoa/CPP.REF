// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Platform/Linux/LinuxPlatformAtomics.h"

#if PLATFORM_LINUX

#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <limits>
#include <algorithm>

namespace Ayla
{
    // 내부 유틸리티 함수
    namespace
    {
        inline pthread_rwlock_t* GetRWLock(void*& LockVal)
        {
            return reinterpret_cast<pthread_rwlock_t*>(LockVal);
        }

        inline pthread_cond_t* GetCondVar(void*& CondVal)
        {
            return reinterpret_cast<pthread_cond_t*>(CondVal);
        }
    }

    void LinuxPlatformAtomics::InitializeSpinlock(void*& LockVal) noexcept
    {
        auto* rwlock = new pthread_rwlock_t();
        pthread_rwlock_init(rwlock, nullptr);
        LockVal = rwlock;
    }

    void LinuxPlatformAtomics::DestroySpinlock(void*& LockVal) noexcept
    {
        if (LockVal)
        {
            pthread_rwlock_destroy(GetRWLock(LockVal));
            delete GetRWLock(LockVal);
            LockVal = nullptr;
        }
    }

    void LinuxPlatformAtomics::AcquireSpinlock(void*& LockVal, bool bShared) noexcept
    {
        if (bShared)
            pthread_rwlock_rdlock(GetRWLock(LockVal));
        else
            pthread_rwlock_wrlock(GetRWLock(LockVal));
    }

    void LinuxPlatformAtomics::ReleaseSpinlock(void*& LockVal, bool bShared) noexcept
    {
        (void)bShared; // 읽기/쓰기 구분 없이 unlock
        pthread_rwlock_unlock(GetRWLock(LockVal));
    }

    bool LinuxPlatformAtomics::TryAcquireSpinlock(void*& LockVal, bool bShared) noexcept
    {
        int ret;
        if (bShared)
            ret = pthread_rwlock_tryrdlock(GetRWLock(LockVal));
        else
            ret = pthread_rwlock_trywrlock(GetRWLock(LockVal));
        return ret == 0;
    }

    void LinuxPlatformAtomics::InitializeSpinlockConditionVariable(void*& CondVal) noexcept
    {
        auto* cond = new pthread_cond_t();
        pthread_cond_init(cond, nullptr);
        CondVal = cond;
    }

    void LinuxPlatformAtomics::DestroySpinlockConditionVariable(void*& CondVal) noexcept
    {
        if (CondVal)
        {
            pthread_cond_destroy(GetCondVar(CondVal));
            delete GetCondVar(CondVal);
            CondVal = nullptr;
        }
    }

    void LinuxPlatformAtomics::WaitSpinlockConditionVariable(void*& CondVal, void*& LockVal, bool bShared) noexcept
    {
        (void)bShared; // pthread_rwlock_t는 unlock 후 wait 필요
        pthread_cond_wait(GetCondVar(CondVal), GetRWLock(LockVal));
    }

    bool LinuxPlatformAtomics::WaitForSpinlockConditionVariable(void*& CondVal, void*& LockVal, size_t Sleep, bool bShared) noexcept
    {
        (void)bShared;
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

    void LinuxPlatformAtomics::NotifyOneSpinlockConditionVariable(void*& CondVal) noexcept
    {
        pthread_cond_signal(GetCondVar(CondVal));
    }

    void LinuxPlatformAtomics::NotifyAllSpinlockConditionVariable(void*& CondVal) noexcept
    {
        pthread_cond_broadcast(GetCondVar(CondVal));
    }
}

#endif