// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Rules;
using AE.Source;

using System.Diagnostics;
using System.Runtime.Serialization;

namespace AE.Makefiles;

public class MakefileRule : MakefileBase
{
    private readonly string _MakefileCachePath;
    public override string MakefileCachePath => _MakefileCachePath;

    private TargetRules? _Rule;
    public TargetRules Rule
    {
        get
        {
            return _Rule ?? throw new InvalidOperationException("Internal error. Rule is not loaded.");
        }
        set
        {
            Debug.Assert(_Rule == null);
            _Rule = value;
            IsLoaded = true;
        }
    }

    public MakefileRule(ProjectDirectory Directory, string MakefilePath)
        : base(Directory, MakefilePath)
    {
        string TargetName = Path.GetFileNameWithoutExtension(MakefilePath);
        _MakefileCachePath = Path.Combine(Directory.Intermediate.Makefiles, TargetName, "Makefile");
    }

    public override async Task<bool> TryLoadAsync(object Arg, CancellationToken SToken = default)
    {
        if (File.Exists(MakefileCachePath) == false)
        {
            return false;
        }

        if (Arg is not TargetInfo Info)
        {
            throw new ArgumentException();
        }

        byte[] Content = await File.ReadAllBytesAsync(MakefileCachePath, SToken);
        using var Reader = new BinaryReader(new MemoryStream(Content));

        throw new Exception();
    }
}
