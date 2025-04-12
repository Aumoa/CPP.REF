using System.Reflection;
using System.Runtime.InteropServices;

try
{
    object? launch;

    if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
    {
        var launchDll = Assembly.LoadFrom("WindowsLaunch.Bindings.dll");
        var launchType = launchDll.GetType("Ayla.WindowsLaunch");
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

    var guardedMainMethod = launch.GetType().GetMethod("GuardedMain", BindingFlags.Public | BindingFlags.Instance | BindingFlags.FlattenHierarchy);
    if (guardedMainMethod == null)
    {
        throw new InvalidOperationException("Required method 'GuardedMain' not found.");
    }

    var returnValue = guardedMainMethod.Invoke(launch, []);
    if (returnValue != null)
    {
        return (int)returnValue;
    }

    return 0;
}
finally
{
    var coreAssembly = Assembly.LoadFrom("Core.Bindings.dll");
    var gcType = coreAssembly.GetType("Ayla.GC");
}