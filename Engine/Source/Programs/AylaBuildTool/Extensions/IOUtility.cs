// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

public static class IOUtility
{
    private static readonly Dictionary<string, long> s_Hashes = new();

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

    public static long GetHash(string InPath)
    {
        lock (s_Hashes)
        {
            if (s_Hashes.TryGetValue(InPath, out long Hash))
            {
                return Hash;
            }
        }

        unchecked
        {
            string Content = File.ReadAllText(InPath);
            long Hash = (long)CRC64.Generate64(Content);

            lock (s_Hashes)
            {
                s_Hashes.TryAdd(InPath, Hash);
            }
            return Hash;
        }
    }

    public static DirectoryReference GetConfigDirectory(this DirectoryReference directory, BuildConfiguration config)
    {
        return directory.GetHierarchy(config.Platform.TargetName, config.Configuration.ToString());
    }
}
