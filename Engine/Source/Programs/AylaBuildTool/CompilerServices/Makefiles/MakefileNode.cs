// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

public abstract class MakefileNode
{
    private readonly TaskCompletionSource completionSource = new();

    public Task CompletedTask { get; }

    public Task DependsOn { get; set; } = Task.CompletedTask;

    protected MakefileNode()
    {
        CompletedTask = completionSource.Task;
    }

    public void MarkCompleted()
    {
        completionSource.TrySetResult();
    }

    public void MarkCanceled()
    {
        completionSource.TrySetCanceled();
    }

    public void MarkError(Exception exception)
    {
        completionSource.TrySetException(exception);
    }
}
