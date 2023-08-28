// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;

namespace AE.SourceTree;

public sealed class Makefile
{
    [SetsRequiredMembers]
    private Makefile(string InMakefilePath)
    {
        MakefilePath = InMakefilePath;
        Caches = Array.Empty<MakefileSourceCache>();
    }

    public required string MakefilePath { get; init; }

    public required MakefileSourceCache[] Caches { get; init; }

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
            var Caches = new MakefileSourceCache[Count];
            for (int i = 0; i < Count; ++i)
            {
                Caches[i] = MakefileSourceCache.LoadCacheFromBinary(Reader);
            }

            return new(Filename)
            {
                Caches = Caches
            };
        }
        catch (Exception E)
        {
            Console.WriteLine("Makefile is not compatible with current version: {0}: {1}", E.GetType().Name, E.Message);
            return new(Filename);
        }
    }

    public async Task SaveMakefileCacheAsync(CancellationToken SToken = default)
    {
        using var Stream = new MemoryStream();
        var Writer = new BinaryWriter(Stream);
        Writer.Write(Global.MakefileVersion);

        MakefileSourceCache[] AllCaches = CompileItems.Select(p => p.Cache!).Where(p => p != null).ToArray();
        Writer.Write(AllCaches.Length);
        foreach (var Cache in AllCaches)
        {
            Cache.SaveCacheToBinary(Writer);
        }

        string DirectoryName = Path.GetDirectoryName(MakefilePath)!;
        if (Directory.Exists(DirectoryName) == false)
        {
            Directory.CreateDirectory(DirectoryName);
        }

        await File.WriteAllBytesAsync(MakefilePath, Stream.ToArray(), SToken);
    }

    public MakefileCompile[] CompileItems { get; set; } = Array.Empty<MakefileCompile>();
}
