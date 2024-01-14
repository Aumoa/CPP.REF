// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.CompilerServices.Makefiles;

public record MakefileSourceCache
{
    public required SourceCodeCache SourceCache;

    public required SourceCodeCache[] Dependencies;

    public required string ObjectOutputFile;

    public static MakefileSourceCache LoadCacheFromBinary(BinaryReader reader, CancellationToken cancellationToken)
    {
        var SourceCache = SourceCodeCache.LoadCacheFromBinary(reader);
        var Includes = SourceCodeCache.LoadCacheArrayFromBinary(reader);
        var ObjectOutputFile = reader.ReadString();

        cancellationToken.ThrowIfCancellationRequested();

        return new MakefileSourceCache()
        {
            SourceCache = SourceCache,
            Dependencies = Includes,
            ObjectOutputFile = ObjectOutputFile
        };
    }

    public void SaveCacheToBinary(BinaryWriter Writer)
    {
        SourceCache.SaveCacheToBinary(Writer);
        SourceCodeCache.SaveCacheToBinary(Dependencies, Writer);
        Writer.Write(ObjectOutputFile);
    }

    public bool IsNewer()
    {
        if (SourceCache.IsNewer() == false)
        {
            return true;
        }

        foreach (var depend in Dependencies)
        {
            if (depend.IsNewer() == false)
            {
                return true;
            }
        }

        if (string.IsNullOrEmpty(ObjectOutputFile) == false)
        {
            if (File.Exists(ObjectOutputFile) == false)
            {
                return true;
            }
        }

        return false;
    }
}
