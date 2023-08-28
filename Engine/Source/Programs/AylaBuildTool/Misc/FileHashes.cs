// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Misc;

public static class FileHashes
{
    private static Dictionary<string, long> Hashes = new();

    public static void Clear()
    {
        lock (Hashes)
        {
            Hashes.Clear();
        }
    }

    public static long GetHash(string InPath)
    {
        lock (Hashes)
        {
            if (Hashes.TryGetValue(InPath, out long Hash))
            {
                return Hash;
            }
        }

        unchecked
        {
            string Content = File.ReadAllText(InPath);
            long Hash = (long)CRC64.Generate64(Content);

            lock (Hashes)
            {
                Hashes.TryAdd(InPath, Hash);
            }
            return Hash;
        }
    }
}
