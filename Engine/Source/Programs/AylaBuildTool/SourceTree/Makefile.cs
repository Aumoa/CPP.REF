// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;

using AE.Rules;
using AE.System;

namespace AE.SourceTree;

public sealed class Makefile
{
    [SetsRequiredMembers]
    private Makefile(string InMakefilePath)
    {
        MakefilePath = InMakefilePath;
        Caches = Array.Empty<MakefileCache>();
    }

    public required string MakefilePath { get; init; }

    public required MakefileCache[] Caches { get; init; }

    public static async Task<Makefile> LoadMakefileCacheAsync(string InPath, CancellationToken SToken = default)
    {
        string Filename = Path.Combine(InPath, "Makefile.abin");
        if (File.Exists(Filename) == false)
        {
            return new(Filename);
        }

        using var Stream = new MemoryStream(await File.ReadAllBytesAsync(Filename, SToken));
        var Reader = new BinaryReader(Stream);
        try
        {
            int MakefileVersion = Reader.ReadInt32();
            if (MakefileVersion != Global.MakefileVersion)
            {
                return new(Filename);
            }

            int Count = Reader.ReadInt32();
            var Caches = new MakefileCache[Count];
            for (int i = 0; i < Count; ++i)
            {
                Caches[i] = MakefileCache.LoadCacheFromBinary(Reader);
            }

            return new(Filename)
            {
                Caches = Caches
            };
        }
        catch
        {
            return new(Filename);
        }
    }

    public async Task SaveMakefileCacheAsync(CancellationToken SToken = default)
    {
        using var Stream = new MemoryStream();
        var Writer = new BinaryWriter(Stream);
        Writer.Write(Global.MakefileVersion);

        MakefileCache[] AllCaches = CompileItems.Select(p => p.Cache).ToArray();
        Writer.Write(AllCaches.Length);
        foreach (var Cache in AllCaches)
        {
            Cache.SaveCacheToBinary(Writer);
        }

        await File.WriteAllBytesAsync(MakefilePath, Stream.ToArray(), SToken);
    }

    public MakefileCompile[] CompileItems { get; set; } = Array.Empty<MakefileCompile>();
}
