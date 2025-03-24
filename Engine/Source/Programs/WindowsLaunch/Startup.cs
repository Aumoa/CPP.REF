using System.Runtime.InteropServices;

nint[] nativeArgs = new nint[args.Length];
for (int i = 0; i < args.Length; ++i)
{
    nativeArgs[i] = Marshal.StringToHGlobalUni(args[i]);
}

try
{
    return Ayla__Launch__StartApplication(nativeArgs, nativeArgs.Length);
}
finally
{
    foreach (var arg in nativeArgs)
    {
        Marshal.FreeHGlobal(arg);
    }

    nativeArgs = [];
}

[DllImport("Launch.dll", CharSet = CharSet.Unicode)]
static extern int Ayla__Launch__StartApplication(nint[] args, int length);