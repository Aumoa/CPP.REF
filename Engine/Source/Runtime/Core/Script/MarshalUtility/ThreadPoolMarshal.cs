using System.Runtime.InteropServices;

namespace Ayla;

internal static class ThreadPoolMarshal
{
    public static void QueueUserWorkItem()
    {
        ThreadPool.QueueUserWorkItem(_ => HandleUserWorkItem());
    }

    public static void GetMaxThreads(out int workerThreads, out int completionPortThreads)
    {
        ThreadPool.GetMaxThreads(out workerThreads, out completionPortThreads);
    }

    [DllImport("Core", EntryPoint = "Ayla__ThreadPool__HandleUserWorkItem")]
    private static extern void HandleUserWorkItem();
}
