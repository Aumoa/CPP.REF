// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;
using AE.IO;
using AE.Source;

namespace AE.Assemblies;

public class CSharpAssembly : ModuleAssembly
{
    [SetsRequiredMembers]
    public CSharpAssembly(ProjectDirectory target, FileReference script)
    {
        var directory = script.GetDirectory();
        Name = directory.Name;
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
