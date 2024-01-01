// Copyright 2020-2023 Aumoa.lib. All right reserved.

using AE.Misc;

namespace AE.Extensions;

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

    public static string[] GetFiles(string directory, string searchPattern = "*.*", SearchOption searchOption = SearchOption.TopDirectoryOnly)
    {
        if (Directory.Exists(directory))
        {
            return Directory.GetFiles(directory, searchPattern, searchOption);
        }
        return Array.Empty<string>();
    }

    public static string[] GetDirectories(string directory, SearchOption searchOption = SearchOption.TopDirectoryOnly)
    {
        if (Directory.Exists(directory))
        {
            return Directory.GetDirectories(directory, "*.*", searchOption);
        }
        return Array.Empty<string>();
    }
}
