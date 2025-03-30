using System.Runtime.InteropServices;

if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
{
    return Ayla__WindowsLaunch__StartApplication(args, args.Length);
}
else
{
    throw new PlatformNotSupportedException("This host platform is currently not supported.");
}

[DllImport("WindowsLaunch.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
static extern int Ayla__WindowsLaunch__StartApplication(string[] args, int length);
