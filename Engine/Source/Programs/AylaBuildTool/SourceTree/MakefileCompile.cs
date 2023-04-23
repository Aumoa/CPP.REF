// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Misc;

namespace AE.SourceTree;

public sealed record MakefileCompile : IEquatable<MakefileCompile>
{
    public required string SourceCode { get; init; }

    public required string Output { get; set; }

    public required SourceCodeHash Hash { get; init; }

    public required string ModuleName { get; init; }

    public required string Intermediate { get; init; }

    public required string[] DependModules { get; init; }

    public required string[] IncludePaths { get; init; }

    public required string[] AdditionalMacros { get; init; }

    public required Dictionary<string, SourceCodeHash> IncludeHashes { get; set; }

    public void Serialize(BinaryWriter Writer)
    {
        Writer.Write(SourceCode);
        Writer.Write(Output);
        Hash.Serialize(Writer);
        Writer.Write(ModuleName);
        Writer.Write(Intermediate);
        Writer.Write(DependModules);
        Writer.Write(IncludePaths);
        Writer.Write(AdditionalMacros);
        Writer.Write(IncludeHashes.Count);
        foreach (var (Filename, Hash) in IncludeHashes)
        {
            Writer.Write(Filename);
            Hash.Serialize(Writer);
        }
    }

    public static MakefileCompile Deserialize(BinaryReader Reader)
    {
        string SourceCode = Reader.ReadString();
        string Output = Reader.ReadString();
        SourceCodeHash Hash = SourceCodeHash.Deserialize(Reader);
        string ModuleName = Reader.ReadString();
        string Intermediate = Reader.ReadString();
        string[] DependModules = Reader.ReadStringArray();
        string[] IncludePaths = Reader.ReadStringArray();
        string[] AdditionalMacros = Reader.ReadStringArray();
        int IncludeHashes_Length = Reader.ReadInt32();
        Dictionary<string, SourceCodeHash> IncludeHashes = new();
        for (int i = 0; i < IncludeHashes_Length; ++i)
        {
            string Filename = Reader.ReadString();
            IncludeHashes[Filename] = SourceCodeHash.Deserialize(Reader);
        }

        return new MakefileCompile
        {
            SourceCode = SourceCode,
            Hash = Hash,
            ModuleName = ModuleName,
            Intermediate = Intermediate,
            DependModules = DependModules,
            IncludePaths = IncludePaths,
            AdditionalMacros = AdditionalMacros,
            Output = Output,
            IncludeHashes = IncludeHashes,
        };
    }

    public bool Equals(MakefileCompile? Item)
    {
        if (Item == null)
        {
            return false;
        }

        if (SourceCode != Item.SourceCode ||
            Output != Item.Output ||
            Hash != Item.Hash ||
            ModuleName != Item.ModuleName ||
            Intermediate != Item.Intermediate ||
            DependModules.Length != Item.DependModules.Length ||
            IncludePaths.Length != Item.IncludePaths.Length ||
            AdditionalMacros.Length != Item.AdditionalMacros.Length ||
            IncludeHashes.Count != Item.IncludeHashes.Count)
        {
            return false;
        }

        for (int N = 0; N < DependModules.Length; ++N)
        {
            if (DependModules[N] != Item.DependModules[N])
            {
                return false;
            }
        }

        for (int N = 0; N < IncludePaths.Length; ++N)
        {
            if (IncludePaths[N] != Item.IncludePaths[N])
            {
                return false;
            }
        }

        for (int N = 0; N < AdditionalMacros.Length; ++N)
        {
            if (AdditionalMacros[N] != Item.AdditionalMacros[N])
            {
                return false;
            }
        }

        foreach (var (Name, Value) in IncludeHashes)
        {
            if (Item.IncludeHashes.TryGetValue(Name, out var Hash) == false)
            {
                return false;
            }

            if (Value != Hash)
            {
                return false;
            }
        }

        return true;
    }

    public override int GetHashCode()
    {
        throw new InvalidOperationException("GetHashCode is not allowed.");
    }
}
