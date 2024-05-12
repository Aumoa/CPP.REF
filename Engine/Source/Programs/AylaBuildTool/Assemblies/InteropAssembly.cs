// Copyright 2020-2024 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;

namespace AylaEngine;

public class InteropAssembly : ModuleAssembly
{
    public override string Name { get; }

    public override ProjectDirectory ProjectDirectory { get; }

    public override DirectoryReference SourceDirectory { get; }

    public override FileReference ScriptFile { get; }

    protected override string ScriptIdentify => "Interop";

    protected override string ClassName => base.ClassName.Replace(".Interop", string.Empty);

    [SetsRequiredMembers]
    public InteropAssembly(ProjectDirectory projectDirectory, FileReference scriptFile, string name)
    {
        Name = name;
        ProjectDirectory = projectDirectory;
        SourceDirectory = scriptFile.GetDirectory();
        ScriptFile = scriptFile;
    }
}
