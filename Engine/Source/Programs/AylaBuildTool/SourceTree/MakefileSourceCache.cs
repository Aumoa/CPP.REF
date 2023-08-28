// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.SourceTree;

public record MakefileSourceCache
{
    public required SourceCodeCache SourceCache;

    public required SourceCodeCache[] Includes;

    public required string ObjectOutputFile;

    public static MakefileSourceCache LoadCacheFromBinary(BinaryReader Reader)
    {
        var SourceCache = SourceCodeCache.LoadCacheFromBinary(Reader);
        var Includes = SourceCodeCache.LoadCacheArrayFromBinary(Reader);
        var ObjectOutputFile = Reader.ReadString();

        return new MakefileSourceCache()
        {
            SourceCache = SourceCache,
            Includes = Includes,
            ObjectOutputFile = ObjectOutputFile
        };
    }

    public void SaveCacheToBinary(BinaryWriter Writer)
    {
        SourceCache.SaveCacheToBinary(Writer);
        SourceCodeCache.SaveCacheToBinary(Includes, Writer);
        Writer.Write(ObjectOutputFile);
    }

    public bool IsNewer(string InSourceCodePath)
    {
        if (SourceCache.CompareTo(InSourceCodePath) == false)
        {
            return true;
        }

        foreach (var Include in Includes)
        {
            if (Include.CompareTo(Include.Path) == false)
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
