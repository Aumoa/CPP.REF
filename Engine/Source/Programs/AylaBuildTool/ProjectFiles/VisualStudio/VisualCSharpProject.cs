// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;
using AE.Assemblies;
using AE.BuildSettings;
using AE.Extensions;
using AE.IO;
using AE.Misc;
using AE.Source;

namespace AE.ProjectFiles.VisualStudio;

public class VisualCSharpProject : VisualStudioProject
{
    public override string Name { get; }

    public override string GUID { get; }

    public override string Filter { get; }

    public override FileReference Project { get; }

    public override ProjectDirectory TargetDirectory { get; }

    public override DirectoryReference SourceDirectory { get; }

    public VisualCSharpProject(ScriptableAssembly assembly)
    {
        Name = assembly.Name;
        GUID = VSUtility.MakeGUID(assembly.ScriptFile.Value);
        Project = assembly.ScriptFile;
        TargetDirectory = assembly.ProjectDirectory;
        SourceDirectory = assembly.SourceDirectory;

        if (assembly.IsProgram)
        {
            Filter = "Programs";
        }
        else
        {
            Filter = assembly.RelativeDirectory.Replace(Path.DirectorySeparatorChar, '/');
        }
    }

    public override (string, string) MapConfiguration(Configuration configuration, bool isEditor, TargetPlatform platform)
    {
        if (configuration == Configuration.Debug)
        {
            return ("Debug", "Any CPU");
        }
        else
        {
            return ("Release", "Any CPU");
        }
    }

    public override Task GenerateProjectFilesAsync(CancellationToken cancellationToken = default)
    {
        return Task.CompletedTask;
    }
}
