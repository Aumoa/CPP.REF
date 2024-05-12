// Copyright 2020-2024 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;

namespace AylaEngine;

public class CSharpAssembly : ScriptableAssembly
{
    [SetsRequiredMembers]
    public CSharpAssembly(ProjectDirectory target, FileReference script)
    {
        var directory = script.GetDirectory();
        Name = directory.FileName;
        SourceDirectory = directory;
        ProjectDirectory = target;
        ScriptFile = script;
    }

    public override string Name { get; }

    public override DirectoryReference SourceDirectory { get; }

    public override ProjectDirectory ProjectDirectory { get; }

    public override FileReference ScriptFile { get; }

    protected override bool HasAssembly => false;

    protected override string ScriptIdentify => "CSharp";
}
