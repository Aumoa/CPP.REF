// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Misc;

file class AsyncLockState : IDisposable
{
    private readonly SemaphoreSlim Semaphore;

    public AsyncLockState(SemaphoreSlim Semaphore)
    {
        this.Semaphore = Semaphore;
    }

    public void Dispose()
    {
        this.Semaphore.Release();
        GC.SuppressFinalize(this);
    }
}

public class AsyncLock
{
    private readonly SemaphoreSlim Semaphore = new(1, 1);

    public AsyncLock(int Initial = 1, int Max = 1)
    {
        Semaphore = new SemaphoreSlim(Initial, Max);
    }

    public IDisposable Lock()
    {
        Semaphore.Wait();
        return new AsyncLockState(Semaphore);
    }

    public Task<IDisposable> LockAsync(CancellationToken SToken = default)
    {
        return Semaphore.WaitAsync(SToken).ContinueWith(p =>
        {
            return (IDisposable)new AsyncLockState(Semaphore);
        });
    }
}
