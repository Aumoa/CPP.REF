// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

public static class VSUtility
{
    public static string MakeGUID(string name)
    {
        unchecked
        {
            ulong val = CRC64.Generate64(name);
            Span<byte> bytes = stackalloc byte[16];
            bytes[0] = (byte)(val >> 0 & 0xFF);
            bytes[1] = (byte)(val >> 8 & 0xFF);
            bytes[2] = (byte)(val >> 16 & 0xFF);
            bytes[3] = (byte)(val >> 24 & 0xFF);
            bytes[4] = (byte)(val >> 32 & 0xFF);
            bytes[5] = (byte)(val >> 40 & 0xFF);
            bytes[6] = (byte)(val >> 48 & 0xFF);
            bytes[7] = (byte)(val >> 56 & 0xFF);
            val = val & 0xDEADBEEFDEADBEEF;
            bytes[0 + 8] = (byte)(val >> 0 & 0xFF);
            bytes[1 + 8] = (byte)(val >> 8 & 0xFF);
            bytes[2 + 8] = (byte)(val >> 16 & 0xFF);
            bytes[3 + 8] = (byte)(val >> 24 & 0xFF);
            bytes[4 + 8] = (byte)(val >> 32 & 0xFF);
            bytes[5 + 8] = (byte)(val >> 40 & 0xFF);
            bytes[6 + 8] = (byte)(val >> 48 & 0xFF);
            bytes[7 + 8] = (byte)(val >> 56 & 0xFF);
            var guid = new Guid(bytes);
            return guid.ToString().ToUpper();
        }
    }

    public static bool TryGetFilterPaths(string filterPath, out string outName, out string outParentPath)
    {
        var paths = filterPath.Split('/');
        outName = paths[^1];
        if (paths.Length > 1)
        {
            outParentPath = paths[^2];
            return true;
        }

        outParentPath = string.Empty;
        return false;
    }
}
