// Copyright 2020-2023 Aumoa.lib. All right reserved.

using System.Runtime.InteropServices;

using AE.CoreAObject;

namespace AE.Launch.Windows;

public static class WindowsLaunch
{
    private static int Main(string[] args)
    {
        var instance = new MySampleClass();
        instance.value = 10;
        var instance2 = new SampleClass();
        instance2.Assign(instance);
        return WindowsStartup(string.Join(' ', args.Select(p => $"\"{p}\"")));
    }

    public static int WindowsStartup(string lpCmdLine) => NativeCall_WindowsStartup(lpCmdLine);

    [DllImport("Launch.dll", EntryPoint = "WindowsStartup", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    private static extern int NativeCall_WindowsStartup(string lpCmdLine);
}
