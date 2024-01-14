// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;
using AE.CompilerServices;
using AE.IO;
using AE.Projects;
using AE.System;

namespace AE.CompilerServices.Makefiles;

public sealed class Makefile
{
    public required ModuleInformation ModuleInfo { get; init; }

    public required FileReference MakefilePath { get; init; }

    public required MakefileSourceCache[] Caches { get; init; }

    private static Makefile GetInvalid(ModuleInformation moduleInfo, FileReference file) => new Makefile()
    {
        ModuleInfo = moduleInfo,
        MakefilePath = file,
        Caches = Array.Empty<MakefileSourceCache>()
    };

    public static async Task<Makefile> LoadFromFileAsync(ModuleInformation moduleInfo, FileReference file, CancellationToken cancellationToken = default)
    {
        if (file.IsExists == false)
        {
            return GetInvalid(moduleInfo, file);
        }

        using var stream = new MemoryStream(await file.ReadAllBytesAsync(cancellationToken));
        var reader = new BinaryReader(stream);
        try
        {
            int makefileVersion = reader.ReadInt32();
            if (makefileVersion != Global.MakefileVersion)
            {
                Console.WriteLine("Makefile version is mismatched. {0} -> {1}", makefileVersion, Global.MakefileVersion);
                return GetInvalid(moduleInfo, file);
            }

            string toolChainSign = reader.ReadString();
            if (toolChainSign != ToolChain.Current.GetToolChainSignature())
            {
                Console.WriteLine("ToolChain signature is mismatched. {0} -> {1}", toolChainSign, ToolChain.Current.GetToolChainSignature());
                return GetInvalid(moduleInfo, file);
            }

            int count = reader.ReadInt32();
            var caches = new MakefileSourceCache[count];
            for (int i = 0; i < count; ++i)
            {
                caches[i] = MakefileSourceCache.LoadCacheFromBinary(reader, cancellationToken);
            }

            return new()
            {
                ModuleInfo = moduleInfo,
                MakefilePath = file,
                Caches = caches
            };
        }
        catch (OperationCanceledException)
        {
            throw;
        }
        catch (Exception E)
        {
            Console.WriteLine("Makefile is not compatible with current version: {0}: {1}", E.GetType().Name, E.Message);
#if DEBUG
            Console.Error.WriteLine(E.StackTrace);
#endif
            return GetInvalid(moduleInfo, file);
        }
    }

    public async Task SaveMakefileCacheAsync(CancellationToken cancellationToken = default)
    {
        using var stream = new MemoryStream();
        var writer = new BinaryWriter(stream);
        writer.Write(Global.MakefileVersion);

        writer.Write(ToolChain.ToolChainSignature);

        MakefileSourceCache[] allCaches = MakefileCompiles.Select(p => p.Cache!).Where(p => p != null).ToArray();
        writer.Write(allCaches.Length);
        foreach (var cache in allCaches)
        {
            cache.SaveCacheToBinary(writer);
        }

        var directory = MakefilePath!.GetDirectory();
        if (directory.IsExists == false)
        {
            directory.Create();
        }

        await File.WriteAllBytesAsync(MakefilePath, stream.ToArray(), cancellationToken);
    }

    public MakefileCompile[] MakefileCompiles { get; set; } = Array.Empty<MakefileCompile>();

    public MakefileLink MakefileLink { get; set; } = null!;
}
