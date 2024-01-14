// Copyright 2020-2023 Aumoa.lib. All right reserved.

namespace AE.CompilerServices.Makefiles;

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
