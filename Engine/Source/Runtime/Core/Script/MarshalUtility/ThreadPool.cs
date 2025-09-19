using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using SystemThreadPool = System.Threading.ThreadPool;

namespace Ayla;

internal static class ThreadPool
{
    public static void QueueUserWorkItem()
    {
        SystemThreadPool.QueueUserWorkItem(_ => HandleUserWorkItem());
    }

    public static void GetMaxThreads(out int workerThreads, out int completionPortThreads)
    {
        SystemThreadPool.GetMaxThreads(out workerThreads, out completionPortThreads);
    }

    [DllImport("Core", EntryPoint = "Ayla__ThreadPool__HandleUserWorkItem")]
    private static extern void HandleUserWorkItem();
}
