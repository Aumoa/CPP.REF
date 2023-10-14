// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;

using AE.Misc;

namespace AE.SourceTree;

public record SourceCodeCache
{
    public required string Path;

    public required DateTime LastWriteTimeInUtc;

    public static SourceCodeCache LoadCacheFromBinary(BinaryReader Reader)
    {
        return new SourceCodeCache
        {
            Path = Reader.ReadString(),
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
        Writer.Write(File.GetLastWriteTimeUtc(Path).ToBinary());
    }

    public bool CompareTo(string InSourceCodePath)
    {
        Debug.Assert(Path.Equals(InSourceCodePath, StringComparison.OrdinalIgnoreCase));

        DateTime WriteTime = File.GetLastWriteTimeUtc(InSourceCodePath);
        if (WriteTime == this.LastWriteTimeInUtc)
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
            LastWriteTimeInUtc = File.GetLastWriteTimeUtc(InPath)
        };
    }
}
