// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Runtime.InteropServices;

namespace Ayla;

public static class GC
{
    [DllImport("Core")]
    private static extern nint Injected__Ayla__GC__New();

    [DllImport("Core")]
    private static extern void Injected__Ayla__GC__Finalize(nint self_);

    public static void Collect(int generation = 2)
    {
        Injected__Ayla__GC__Collect(generation);
    }


    [DllImport("Core")]
    private static extern void Injected__Ayla__GC__Collect(int generation = 2);

    public static void WaitForCompleteToFinalize()
    {
        Injected__Ayla__GC__WaitForCompleteToFinalize();
    }


    [DllImport("Core")]
    private static extern void Injected__Ayla__GC__WaitForCompleteToFinalize();
}
