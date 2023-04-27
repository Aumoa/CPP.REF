// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;

using AE.Misc;

namespace AE.SourceTree;

public struct SourceCodeHash
{
    public DateTime WriteTime;

    public ulong HashCode;

    public void Serialize(BinaryWriter Writer)
    {
        Writer.Write(WriteTime.Ticks);
        Writer.Write(HashCode);
    }

    public static SourceCodeHash Deserialize(BinaryReader Reader)
    {
        long Ticks = Reader.ReadInt64();
        ulong HashCode = Reader.ReadUInt64();
        return new SourceCodeHash
        {
            WriteTime = new DateTime(Ticks),
            HashCode = HashCode
        };
    }

    public static async Task<SourceCodeHash> GenerateHashAsync(string SourceCode, CancellationToken SToken = default)
    {
        if (File.Exists(SourceCode))
        {
            DateTime WriteTime = File.GetLastWriteTimeUtc(SourceCode);
            ulong Hash = CRC64.Generate64(await File.ReadAllTextAsync(SourceCode, SToken));
            return new SourceCodeHash
            {
                WriteTime = WriteTime,
                HashCode = Hash
            };
        }
        else
        {
            return new SourceCodeHash
            {
                HashCode = 0
            };
        }
    }

    public static bool operator ==(SourceCodeHash L, SourceCodeHash R)
    {
        return L.WriteTime == R.WriteTime
            && L.HashCode == R.HashCode;
    }

    public static bool operator !=(SourceCodeHash L, SourceCodeHash R)
    {
        return L.WriteTime != R.WriteTime
            || L.HashCode != R.HashCode;
    }

    public override int GetHashCode()
    {
        int V1 = (int)(HashCode >> 32);
        int V2 = (int)HashCode;
        int V3 = (int)(WriteTime.Ticks >> 32);
        int V4 = (int)WriteTime.Ticks;
        return V1 ^ V2 ^ V3 ^ V4;
    }

    public bool Equals(SourceCodeHash R)
    {
        return WriteTime == R.WriteTime
            && HashCode == R.HashCode;
    }

    public override bool Equals([NotNullWhen(true)] object? Obj)
    {
        if (Obj is SourceCodeHash S)
        {
            return Equals(S);
        }
        else
        {
            return false;
        }
    }

    public async Task<bool> IsNewer(string SourceCode, CancellationToken SToken = default)
    {
        if (WriteTime == File.GetLastWriteTimeUtc(SourceCode))
        {
            return false;
        }

        if (File.Exists(SourceCode) == false)
        {
            return true;
        }

        ulong H = CRC64.Generate64(await File.ReadAllTextAsync(SourceCode, SToken));
        return HashCode != H;
    }
}
