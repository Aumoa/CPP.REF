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

    public static ProjectDirectory EngineDirectory { get; }

    public static async Task CompareAndWriteAsync(string Filename, string Text, CancellationToken SToken)
    {
        if (File.Exists(Filename) == false)
        {
            await File.WriteAllTextAsync(Filename, Text, SToken);
            return;
        }

        string PreviousText = await File.ReadAllTextAsync(Filename, SToken);
        if (PreviousText.Trim() == Text.Trim())
        {
            return;
        }

        await File.WriteAllTextAsync(Filename, Text, SToken);
    }
}
