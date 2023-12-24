// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Reflection;

using AE.Source;

namespace AE;

public static class Global
{
    static Global()
    {
        string Location = Assembly.GetEntryAssembly()!.Location;
        EngineDirectory = new ProjectDirectory() { Root = Path.GetFullPath(Path.Combine(Location, "..", "..", "..")) };
    }

    public static int MakefileVersion = 7;

    public static ProjectDirectory EngineDirectory { get; }

    public static string BuildToolPath => Path.Combine(EngineDirectory.Binaries.DotNET, "AylaBuildTool");
}
