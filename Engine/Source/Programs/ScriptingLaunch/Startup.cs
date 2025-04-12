using System.Reflection;
using System.Runtime.InteropServices;

object? launch;

if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
{
    var platformDll = Assembly.LoadFrom("WindowsLaunch.Bindings.dll");
    var launchType = platformDll.GetType("Ayla.WindowsLaunch");
    if (launchType == null)
    {
        throw new PlatformNotSupportedException("This host platform is currently not supported.");
    }

    launch = Activator.CreateInstance(launchType);
    if (launch == null)
    {
        throw new InvalidOperationException($"Cannot create '{launchType.FullName}' instance.");
    }
}
else
{
    throw new PlatformNotSupportedException("This host platform is currently not supported.");
}

var launchDll = Assembly.LoadFrom("Launch.Script.dll");
var scriptingLaunchType = launchDll.GetType("Ayla.ScriptingLaunch");
if (scriptingLaunchType == null)
{
    throw new InvalidOperationException("The build version of the `ScriptingLaunch` assembly is different.");
}

var guardedMainMethod = scriptingLaunchType.GetMethod("GuardedMain", BindingFlags.Public | BindingFlags.Static);
if (guardedMainMethod == null)
{
    throw new InvalidOperationException("The build version of the `ScriptingLaunch` assembly is different.");
}

var returnValue = guardedMainMethod.Invoke(null, [launch, args]);
if (returnValue != null)
{
    return (int)returnValue;
}

return 0;