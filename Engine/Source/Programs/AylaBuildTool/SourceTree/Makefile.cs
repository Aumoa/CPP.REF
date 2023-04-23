// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Rules;
using AE.System;

namespace AE.SourceTree;

public record Makefile
{
    public required List<MakefileCompile> Items { get; set; } = new();

    public List<MakefileCompile> CompileItems { get; private set; } = new();

    public Task SaveMakefileCacheAsync(TargetRules Rule, CancellationToken SToken = default)
    {
        Dictionary<string, string> MakefilePath = new();
        Dictionary<string, BinaryWriter> MakefileBuilder = new();
        var Config = Rule.Target.BuildConfiguration;

        foreach (var Item in Items)
        {
            if (MakefilePath.TryGetValue(Item.Output, out string? P) == false)
            {
                P = Path.Combine(Item.Output, "Makefile.abin");
                MakefilePath.Add(Item.Output, P);
                MakefileBuilder.Add(P, new(File.Create(P)));
            }

            BinaryWriter Builder = MakefileBuilder[P];
            Item.Serialize(Builder);
        }

        List<Task> Tasks = new();
        foreach (var (_, Builder) in MakefileBuilder)
        {
            var FS = (FileStream)Builder.BaseStream;
            FS.Flush();
        }

        return Task.WhenAll(Tasks);
    }

    public async Task ResolveMakefileCacheAsync(ToolChainInstallation ToolChain, TargetRules Rule, CancellationToken SToken = default)
    {
        var Config = Rule.Target.BuildConfiguration;
        HashSet<string> Makefiles = new();
        foreach (var Item in Items)
        {
            Makefiles.Add(Path.Combine(Item.Output, "Makefile.abin"));
        }

        List<Task> Tasks = new();
        Dictionary<string, MakefileCompile> Compiles = new();
        foreach (var MakefileName in Makefiles)
        {
            if (File.Exists(MakefileName))
            {
                Tasks.Add(File.ReadAllBytesAsync(MakefileName, SToken).ContinueWith(p =>
                {
                    var S = new MemoryStream(p.Result);
                    var R = new BinaryReader(S);

                    long Len = S.Length;
                    List<MakefileCompile> Compiles2 = new();
                    while (S.Position < Len)
                    {
                        Compiles2.Add(MakefileCompile.Deserialize(R));
                    }

                    foreach (var Compile in Compiles2)
                    {
                        lock (Compiles)
                        {
                            Compiles[Compile.SourceCode] = Compile;
                        }
                    }
                }));
            }
        }

        await Task.WhenAll(Tasks);

        Tasks.Clear();
        try
        {
            foreach (var Item in Items)
            {
                string Output = Item.Output;
                Output = Path.Combine(Output, Path.ChangeExtension(Path.GetFileName(Item.SourceCode), ToolChain.GetOutputExtension()));

                if (string.IsNullOrEmpty(Item.Output))
                {
                    lock (CompileItems)
                    {
                        CompileItems.Add(Item);
                    }
                    continue;
                }

                if (File.Exists(Output) == false)
                {
                    lock (CompileItems)
                    {
                        CompileItems.Add(Item);
                    }
                    continue;
                }

                if (Compiles.TryGetValue(Item.SourceCode, out MakefileCompile? Cache) == false)
                {
                    lock (CompileItems)
                    {
                        CompileItems.Add(Item);
                    }
                    continue;
                }

                if (Cache.SourceCode != Item.SourceCode)
                {
                    lock (CompileItems)
                    {
                        CompileItems.Add(Item);
                    }
                    continue;
                }

                async Task Compare()
                {
                    if (await Cache.Hash.IsNewer(Item.SourceCode, SToken))
                    {
                        lock (CompileItems)
                        {
                            CompileItems.Add(Item);
                            return;
                        }
                    }

                    foreach (var (Source, Hash) in Cache.IncludeHashes)
                    {
                        if (await Hash.IsNewer(Source, SToken))
                        {
                            lock (CompileItems)
                            {
                                CompileItems.Add(Item);
                                return;
                            }
                        }
                    }
                }

                Tasks.Add(Compare());
            }

            await Task.WhenAll(Tasks);
        }
        finally
        {
            var Dict = CompileItems.ToDictionary(p => p.SourceCode, p => p);
            List<MakefileCompile> NewItems = new();
            foreach (var Item in Items)
            {
                if (Dict.TryGetValue(Item.SourceCode, out var Newer))
                {
                    NewItems.Add(Newer);
                }
                else
                {
                    NewItems.Add(Item);
                }
            }

            Items = NewItems;
        }
    }
}
