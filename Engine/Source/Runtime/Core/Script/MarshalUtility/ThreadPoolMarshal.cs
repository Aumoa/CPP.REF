using System.Runtime.InteropServices;

namespace Ayla;

internal static class ThreadPoolMarshal
{
    public static void QueueUserWorkItem()
    {
        ThreadPool.QueueUserWorkItem(_ => HandleUserWorkItem());
    }

    public static void GetMinThreads(out int workerThreads, out int completionPortThreads)
    {
        ThreadPool.GetMinThreads(out workerThreads, out completionPortThreads);
    }

    public static void GetMaxThreads(out int workerThreads, out int completionPortThreads)
    {
        ThreadPool.GetMaxThreads(out workerThreads, out completionPortThreads);
    }

    public static void SetMinThreads(int workerThreads, int completionPortThreads)
    {
        ThreadPool.SetMinThreads(workerThreads, completionPortThreads);
    }

    public static void SetMaxThreads(int workerThreads, int completionPortThreads)
    {
        ThreadPool.SetMaxThreads(workerThreads, completionPortThreads);
    }

    [DllImport("Core", EntryPoint = "Ayla__ThreadPool__HandleUserWorkItem")]
    private static extern void HandleUserWorkItem();
}
