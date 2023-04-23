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

    public static string BuildToolPath => Path.Combine(EngineDirectory.Binaries.DotNET, "AylaBuildTool");

    public static async Task CompareAndWriteAsync(string Filename, string Text, CancellationToken SToken)
    {
        if (File.Exists(Filename) == false)
        {
            Directory.CreateDirectory(Path.GetDirectoryName(Filename)!);
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

    public static bool IsSourceCode(string Name)
    {
        string Ext = Path.GetExtension(Name).ToLower();
        return Ext == ".cpp";
    }

    public static bool IsHeaderFile(string Name)
    {
        string Ext = Path.GetExtension(Name).ToLower();
        return Ext == ".h" || Ext == ".inl";
    }

    public static bool IsRuleFile(string Name)
    {
        string Ext = Path.GetExtension(Name).ToLower();
        return Ext == ".cs";
    }

    public static bool IsNatvisFile(string Name)
    {
        string Ext = Path.GetExtension(Name).ToLower();
        return Ext == ".natvis";
    }

    public static bool IsSourceFile(string Name)
    {
        return IsSourceCode(Name)
            || IsHeaderFile(Name)
            || IsRuleFile(Name)
            || IsNatvisFile(Name);
    }
}
