// Copyright 2020-2023 Aumoa.lib. All right reserved.

using System.Runtime.InteropServices;

using AE.CoreAObject;

namespace AE.Launch.Windows;

public static class WindowsLaunch
{
    private static int Main(string[] args)
    {
        string lpCmdLine = string.Join(' ', args.Select(p => $"\"{p}\""));
        return NativeCall_WindowsStartup(lpCmdLine);
    }

    [DllImport("Launch", EntryPoint = "WindowsStartup", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    private static extern int NativeCall_WindowsStartup(string lpCmdLine);
}
