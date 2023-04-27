// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;

using AE.Misc;

namespace AE.SourceTree;

public record MakefileCache
{
    public required string SourceCodePath;

    public required ulong SourceCodeHash;

    public required DateTime LastWriteTimeInUtc;

    public required string[] Dependencies;

    public required string[] Includes;

    public required string IntermediateOutput;

    public required string ObjectOutput;

    public required string InterfaceOutput;

    public static MakefileCache LoadCacheFromBinary(BinaryReader Reader)
    {
        MakefileCache Instance = new()
        {
            SourceCodePath = Reader.ReadString(),
            SourceCodeHash = Reader.ReadUInt64(),
            LastWriteTimeInUtc = DateTime.FromBinary(Reader.ReadInt64()),
            Dependencies = Reader.ReadStringArray(),
            Includes = Reader.ReadStringArray(),
            IntermediateOutput = Reader.ReadString(),
            ObjectOutput = Reader.ReadString(),
            InterfaceOutput = Reader.ReadString(),
        };
        return Instance;
    }

    public void SaveCacheToBinary(BinaryWriter Writer)
    {
        Writer.Write(SourceCodePath);
        Writer.Write(SourceCodeHash);
        Writer.Write(LastWriteTimeInUtc.ToBinary());
        Writer.Write(Dependencies);
        Writer.Write(Includes);
        Writer.Write(IntermediateOutput);
        Writer.Write(ObjectOutput);
        Writer.Write(InterfaceOutput);
    }

    public bool CompareTo(string InSourceCodePath, out ulong OutSourceCodeHash)
    {
        Debug.Assert(SourceCodePath == InSourceCodePath);

        DateTime LastWriteTimeInUtc = File.GetLastWriteTimeUtc(InSourceCodePath);
        if (LastWriteTimeInUtc == this.LastWriteTimeInUtc)
        {
            OutSourceCodeHash = 0;
            return true;
        }

        ulong SourceCodeHash = Global.GenerateSourceCodeHash(InSourceCodePath);
        if (SourceCodeHash == this.SourceCodeHash)
        {
            OutSourceCodeHash = 0;
            return true;
        }

        OutSourceCodeHash = SourceCodeHash;
        return false;
    }

    public bool IsNewer(string InSourceCodePath, out ulong OutSourceCodeHash)
    {
        if (CompareTo(InSourceCodePath, out OutSourceCodeHash) == false)
        {
            return true;
        }

        if (string.IsNullOrEmpty(ObjectOutput) == false)
        {
            if (File.Exists(ObjectOutput) == false)
            {
                return true;
            }
        }

        if (string.IsNullOrEmpty(InterfaceOutput) == false)
        {
            if (File.Exists(InterfaceOutput) == false)
            {
                return true;
            }
        }

        return false;
    }
}
