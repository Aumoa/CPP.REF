// Copyright 2020-2024 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;

namespace AylaEngine;

public class CppAssembly : ModuleAssembly
{
    [SetsRequiredMembers]
    public CppAssembly(ProjectDirectory projectDirectory, FileReference scriptFile, string name)
    {
        Name = name;
        ProjectDirectory = projectDirectory;
        SourceDirectory = scriptFile.GetDirectory();
        ScriptFile = scriptFile;
    }

    public override string Name { get; }

    public override ProjectDirectory ProjectDirectory { get; }

    public override DirectoryReference SourceDirectory { get; }

    public override FileReference ScriptFile { get; }

    protected override string ScriptIdentify => "Module";
}
