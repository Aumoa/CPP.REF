using System.Diagnostics.CodeAnalysis;

namespace AylaEngine;

internal abstract class Commands : IDisposable
{
    private static readonly Stack<Commands> s_Stack = new();
    public static bool IsMain => s_Stack.Count <= 1;

    public abstract ValueTask RenderPromptAsync(CancellationToken cancellationToken);

    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this);
    }

    protected virtual void Dispose(bool disposing)
    {
    }

    public static void Push<T>() where T : Commands, new()
    {
        s_Stack.Push(new T());
    }

    public static void Pop()
    {
        var pop = s_Stack.Pop();
        pop.Dispose();  
    }

    public static bool TryGetCurrent([MaybeNullWhen(false)] out Commands currentCommands)
    {
        return s_Stack.TryPeek(out currentCommands);
    }
}
