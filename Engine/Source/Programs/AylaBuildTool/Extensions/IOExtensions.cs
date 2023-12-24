// Copyright 2020-2023 Aumoa.lib. All right reserved.

namespace AE.Extensions;

public static class IOExtensions
{
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
}
