// Copyright 2020-2024 Aumoa.lib. All right reserved.

using System.Xml;
using AE.Assemblies;
using AE.BuildSettings;
using AE.CompilerServices;
using AE.Extensions;
using AE.IO;
using AE.Misc;
using AE.Projects;
using AE.Source;

namespace AE.ProjectFiles.VisualStudio;

public class VisualInteropProject : VisualStudioProject
{
    public const string DotNETVersion = "net8.0";

    public override string Name { get; }

    public override string GUID { get; }

    public override string Filter { get; }

    public override FileReference Project { get; }

    public override ProjectDirectory TargetDirectory { get; }

    public override DirectoryReference SourceDirectory { get; }

    private readonly InteropAssembly m_Assembly;
    private readonly ModuleInformation m_ModuleInfo;

    public VisualInteropProject(InteropAssembly assembly)
    {
        m_Assembly = assembly;
        m_ModuleInfo = ModuleDependencyCache.GetCached(assembly.Name, string.Empty);

        Name = assembly.Name + ".Interop";
        Filter = assembly.RelativeDirectory.Replace(Path.DirectorySeparatorChar, '/');
        GUID = VSUtility.MakeGUID(ProjectPath);
        SourceDirectory = assembly.SourceDirectory;
        Project = SourceDirectory.GetFile($"{Name}.csproj");
        TargetDirectory = assembly.ProjectDirectory;
    }

    public override (string, string) MapConfiguration(Configuration configuration, bool isEditor, TargetPlatform platform)
    {
        (string, string) result = (string.Empty, "Any CPU");
        switch (configuration)
        {
            case Configuration.Debug:
                result.Item1 = "Debug";
                break;
            case Configuration.DebugGame:
                result.Item1 = TargetDirectory.IsEngineDirectory() ? "Debug" : "Release";
                break;
            default:
                result.Item1 = "Release";
                break;
        }

        return result;
    }

    public override async Task GenerateProjectFilesAsync(CancellationToken cancellationToken = default)
    {
        XmlDocument doc = new();
        var project = doc.AddElement("Project");
        project.SetAttribute("Sdk", "Microsoft.NET.Sdk");

        var buildGroup = project.AddPropertyGroup();
        buildGroup.AddElement("TargetFramework").InnerText = DotNETVersion;
        buildGroup.AddElement("ImplicitUsings").InnerText = "enable";
        buildGroup.AddElement("Nullable").InnerText = "enable";
        buildGroup.AddElement("IsPublishable").InnerText = "False";
        buildGroup.AddElement("OutputType").InnerText = "Library";
        buildGroup.AddElement("AppendTargetFrameworkToOutputPath").InnerText = "False";
        buildGroup.AddElement("RootNamespace").InnerText = TargetDirectory.IsEngineDirectory() ? "AylaEngine" : m_Assembly.Name;
        buildGroup.AddElement("OutputPath").InnerText = Path.GetRelativePath(SourceDirectory.AbsolutePath, TargetDirectory.Binaries.Interop.GetChild("$(Configuration)").AbsolutePath);

        var projectOptions = project.AddElement("ItemGroup");
        projectOptions.AddElement("Compile").Configure(p => p.SetAttribute("Remove", m_Assembly.ScriptFile.FileName));

        var compilerOptions = project.AddElement("ItemGroup");
        compilerOptions.AddElement("Using").Configure(p => p.SetAttribute("Include", "AylaEngine"));
        compilerOptions.AddElement("Using").Configure(p => p.SetAttribute("Include", "AylaEngine.Object")).AddElement("Alias").InnerText = "Object";

        using var writer = new StringWriter();
        var xmlWriter = new XmlTextWriter(writer);
        xmlWriter.Formatting = Formatting.Indented;
        doc.WriteTo(xmlWriter);

        var projectFile = SourceDirectory.GetFile(m_Assembly.Name + ".Interop.csproj");
        await IOUtility.CompareAndWriteAsync(projectFile.AbsolutePath, writer.ToString(), cancellationToken);
    }
}
