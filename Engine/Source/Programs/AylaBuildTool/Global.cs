// Copyright 2020-2024 Aumoa.lib. All right reserved.

using System.Reflection;

namespace AylaEngine;

public static class Global
{
    static Global()
    {
        string Location = Assembly.GetEntryAssembly()!.Location;
        EngineDirectory = new ProjectDirectory() { Root = Path.GetFullPath(Path.Combine(Location, "..", "..", "..")) };
    }

    public static int MakefileVersion = 9;

    public static ProjectDirectory EngineDirectory { get; }

    public static string BuildToolPath => Path.Combine(EngineDirectory.Binaries.DotNET, "AylaBuildTool");
}
