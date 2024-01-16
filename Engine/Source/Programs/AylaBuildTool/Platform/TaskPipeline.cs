// Copyright 2020-2023 Aumoa.lib. All right reserved.

namespace AE.Platform;

public class TaskPipeline<TReturn, TArg1>
{
    private SemaphoreSlim access;
    private Func<TArg1, CancellationToken, Task<TReturn>> function;

    public TaskPipeline(Func<TArg1, CancellationToken, Task<TReturn>> function)
    {
        access = new SemaphoreSlim(Environment.ProcessorCount);
        this.function = function;
    }

    public async Task<TReturn> PublishAsync(TArg1 arg1, CancellationToken cancellationToken = default)
    {
        await access.WaitAsync(cancellationToken);
        try
        {
            return await function(arg1, cancellationToken);
        }
        finally
        {
            access.Release();
        }
    }
}
