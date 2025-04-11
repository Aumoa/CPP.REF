using System.Reflection;
using System.Runtime.InteropServices;

object? launch;

if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
{
    var launchDll = Assembly.LoadFrom("WindowsLaunch.Script.dll");
    var launchType = launchDll.GetType("Ayla.ScriptingLaunch");
    if (launchType == null)
    {
        throw new PlatformNotSupportedException("This host platform is currently not supported.");
    }

    launch = Activator.CreateInstance(launchType);
}
else
{
    throw new PlatformNotSupportedException("This host platform is currently not supported.");
}

var coreAssembly = Assembly.LoadFrom("Core.Bindings.dll");
var gcType = coreAssembly.GetType("Ayla.GC");