// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;

using AE.Misc;

namespace AE.SourceTree;

public record SourceCodeCache
{
    public required string Path;

    public required long SourceCodeHash;

    public required DateTime LastWriteTimeInUtc;

    public static SourceCodeCache LoadCacheFromBinary(BinaryReader Reader)
    {
        return new SourceCodeCache
        {
            Path = Reader.ReadString(),
            SourceCodeHash = Reader.ReadInt64(),
            LastWriteTimeInUtc = DateTime.FromBinary(Reader.ReadInt64())
        };
    }

    public static SourceCodeCache[] LoadCacheArrayFromBinary(BinaryReader Reader)
    {
        int Count = Reader.ReadInt32();
        var Array = new SourceCodeCache[Count];
        for (int i = 0; i < Count; ++i)
        {
            Array[i] = SourceCodeCache.LoadCacheFromBinary(Reader);
        }
        return Array;
    }

    public void SaveCacheToBinary(BinaryWriter Writer)
    {
        Writer.Write(Path);
        Writer.Write(SourceCodeHash);
        Writer.Write(LastWriteTimeInUtc.ToBinary());
    }

    public bool CompareTo(string InSourceCodePath)
    {
        Debug.Assert(Path.Equals(InSourceCodePath, StringComparison.OrdinalIgnoreCase));

        // first: compare file time.
        DateTime WriteTime = File.GetLastWriteTimeUtc(InSourceCodePath);
        if (WriteTime == this.LastWriteTimeInUtc)
        {
            return true;
        }

        // second: compare source code hash.
        long Hash = FileHashes.GetHash(InSourceCodePath);
        if (Hash == SourceCodeHash)
        {
            return true;
        }

        return false;
    }

    public static void SaveCacheToBinary(SourceCodeCache[] Array, BinaryWriter Writer)
    {
        Writer.Write(Array.Length);
        foreach (var Cache in Array)
        {
            Cache.SaveCacheToBinary(Writer);
        }
    }

    public static SourceCodeCache Generate(string InPath)
    {
        return new SourceCodeCache()
        {
            Path = InPath,
            SourceCodeHash = FileHashes.GetHash(InPath),
            LastWriteTimeInUtc = File.GetLastWriteTimeUtc(InPath)
        };
    }
}
