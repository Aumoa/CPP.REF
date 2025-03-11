namespace AylaEngine;

internal static class Global
{
    public static readonly string EngineDirectory;

    static Global()
    {
        var assemblyLocation = typeof(Global).Assembly.Location;
        if (assemblyLocation == null)
        {
            throw new NotSupportedException("AylaBuildTool.dll not support Self-contained assembly.");
        }

        // AylaBuildTool.dll is locate in $(EngineDir)\Binaries\DotNET
        EngineDirectory = Path.GetFullPath(Path.Combine(Path.GetDirectoryName(assemblyLocation) ?? throw new InvalidOperationException(), "..", ".."));
    }
}
