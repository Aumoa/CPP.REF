// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;

using AE.CompilerServices;
using AE.Projects;
using AE.System;

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

    public static async Task<Makefile> LoadMakefileCacheAsync(string path, CancellationToken cancellationToken = default)
    {
        string filename = Path.Combine(path, "Makefile.abin");
        if (File.Exists(filename) == false)
        {
            return Empty(filename);
        }

        using var stream = new MemoryStream(await File.ReadAllBytesAsync(filename, cancellationToken));
        var reader = new BinaryReader(stream);
        try
        {
            int MakefileVersion = reader.ReadInt32();
            if (MakefileVersion != Global.MakefileVersion)
            {
                Console.WriteLine("Makefile version is mismatched. {0} -> {1}", MakefileVersion, Global.MakefileVersion);
                return Empty(filename);
            }

            string toolChainSign = reader.ReadString();
            if (toolChainSign != ToolChain.Current.GetToolChainSignature())
            {
                Console.WriteLine("ToolChain signature is mismatched. {0} -> {1}", toolChainSign, ToolChain.Current.GetToolChainSignature());
                return Empty(filename);
            }

            int Count = reader.ReadInt32();
            var Caches = new MakefileSourceCache[Count];
            for (int i = 0; i < Count; ++i)
            {
                Caches[i] = MakefileSourceCache.LoadCacheFromBinary(reader);
            }

            return new(filename)
            {
                Caches = Caches
            };
        }
        catch (Exception E)
        {
            Console.WriteLine("Makefile is not compatible with current version: {0}: {1}", E.GetType().Name, E.Message);
#if DEBUG
            Console.Error.WriteLine(E.StackTrace);
#endif
            return Empty(filename);
        }
    }

    public async Task SaveMakefileCacheAsync(CancellationToken SToken = default)
    {
        using var Stream = new MemoryStream();
        var Writer = new BinaryWriter(Stream);
        Writer.Write(Global.MakefileVersion);

        Writer.Write(ToolChain.ToolChainSignature);

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

    public static Makefile Empty(string filename) => new Makefile(filename);

    public ModuleInformation ModuleInfo { get; set; } = null!;

    public MakefileCompile[] CompileItems { get; set; } = Array.Empty<MakefileCompile>();
}
