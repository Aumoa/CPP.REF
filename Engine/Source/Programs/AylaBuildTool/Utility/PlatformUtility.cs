namespace AylaEngine;

internal static class PlatformUtility
{
    public static string GetExecutableFileName(string baseName)
    {
        if (OperatingSystem.IsWindows())
        {
            return baseName + ".exe";
        }
        else
        {
            return baseName;
        }
    }
}
