using System.Runtime.InteropServices;

return Ayla__WindowsLaunch__StartApplication(args, args.Length);

[DllImport("WindowsLaunch.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
static extern int Ayla__WindowsLaunch__StartApplication(string[] args, int length);