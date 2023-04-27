// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;

using AE.Rules;
using AE.System;

namespace AE.SourceTree;

public sealed class Makefile
{
    [SetsRequiredMembers]
    private Makefile()
    {
        Caches = Array.Empty<MakefileCache>();
    }

    public required MakefileCache[] Caches { get; init; }

    public static async Task<Makefile> LoadMakefileCacheAsync(string InPath, CancellationToken SToken = default)
    {
        string Filename = Path.Combine(InPath, "Makefile.abin");
        if (File.Exists(Filename) == false)
        {
            return new();
        }

        using var Stream = new MemoryStream(await File.ReadAllBytesAsync(Filename, SToken));
        var Reader = new BinaryReader(Stream);
        try
        {
            int MakefileVersion = Reader.ReadInt32();
            if (MakefileVersion != Global.MakefileVersion)
            {
                return new();
            }

            int Count = Reader.ReadInt32();
            var Caches = new MakefileCache[Count];
            for (int i = 0; i < Count; ++i)
            {
                Caches[i] = MakefileCache.LoadCacheFromBinary(Reader);
            }

            return new()
            {
                Caches = Caches
            };
        }
        catch
        {
            return new();
        }
    }

    public async Task SaveMakefileCacheAsync(string InPath, CancellationToken SToken = default)
    {
        string Filename = Path.Combine(InPath, "Makefile.abin");

        using var Stream = new MemoryStream();
        var Writer = new BinaryWriter(Stream);
        Writer.Write(Global.MakefileVersion);
        foreach (var Cache in Caches)
        {
            Cache.SaveCacheToBinary(Writer);
        }

        await File.WriteAllBytesAsync(Filename, Stream.ToArray(), SToken);
    }

    public MakefileCompile[] CompileItems { get; set; } = Array.Empty<MakefileCompile>();
}
