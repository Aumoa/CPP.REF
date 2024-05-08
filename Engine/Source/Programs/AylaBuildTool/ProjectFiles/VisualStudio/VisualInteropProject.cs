// Copyright 2020-2024 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;
using System.Text.Json;
using System.Text.Json.Nodes;
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

        Name = assembly.Name;
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

    public override async Task GenerateProjectFilesAsync(List<VisualStudioProject> projects, CancellationToken cancellationToken = default)
    {
        Dictionary<string, VisualInteropProject>? map = null;
        bool FindReferenceProject(string projectName, [NotNullWhen(true)] out VisualInteropProject? found)
        {
            if (map == null)
            {
                map = new Dictionary<string, VisualInteropProject>();
                foreach (var project in projects)
                {
                    if (project is VisualInteropProject interopProject)
                    {
                        map.Add(interopProject.Name, interopProject);
                    }
                }
            }

            return map.TryGetValue(projectName, out found);
        }

        XmlDocument doc = new();
        var project = doc.AddElement("Project");
        project.SetAttribute("Sdk", "Microsoft.NET.Sdk");

        string outputType = m_ModuleInfo.TargetType switch
        {
            Rules.ModuleRules.ModuleType.Library => "Library",
            Rules.ModuleRules.ModuleType.ConsoleApplication => "Exe",
            Rules.ModuleRules.ModuleType.Application => "WinExe",
            _ => string.Empty
        };

        var buildGroup = project.AddPropertyGroup();
        buildGroup.AddElement("TargetFramework").InnerText = DotNETVersion;
        buildGroup.AddElement("ImplicitUsings").InnerText = "enable";
        buildGroup.AddElement("Nullable").InnerText = "enable";
        buildGroup.AddElement("IsPublishable").InnerText = "False";
        buildGroup.AddElement("OutputType").InnerText = outputType;
        buildGroup.AddElement("AppendTargetFrameworkToOutputPath").InnerText = "False";
        buildGroup.AddElement("RootNamespace").InnerText = TargetDirectory.IsEngineDirectory() ? "AylaEngine" : m_Assembly.Name;
        buildGroup.AddElement("OutputPath").InnerText = Path.GetRelativePath(SourceDirectory.AbsolutePath, TargetDirectory.Binaries.Interop.GetChild("$(Configuration)").AbsolutePath);

        var projectOptions = project.AddElement("ItemGroup");
        projectOptions.AddElement("Compile").Configure(p => p.SetAttribute("Remove", m_Assembly.ScriptFile.FileName));
        projectOptions.AddElement("None").Configure(p => p.SetAttribute("Include", m_Assembly.ScriptFile.FileName));

        var references = project.AddElement("ItemGroup");
        foreach (var depend in m_ModuleInfo.DependModules)
        {
            if (FindReferenceProject(depend, out var found))
            {
                references.AddElement("ProjectReference").Configure(p => p.SetAttribute("Include", found.Project.AbsolutePath));
            }
            else
            {
                Console.Error.WriteLine("Referenced module {0} is not found in {1} module.", depend, m_ModuleInfo.Name);
            }
        }

        var compilerOptions = project.AddElement("ItemGroup");
        compilerOptions.AddElement("Using").Configure(p => p.SetAttribute("Include", "AylaEngine"));
        compilerOptions.AddElement("Using").Configure(p => p.SetAttribute("Include", "AylaEngine.Object")).AddElement("Alias").InnerText = "Object";

        using var writer = new StringWriter();
        var xmlWriter = new XmlTextWriter(writer);
        xmlWriter.Formatting = Formatting.Indented;
        doc.WriteTo(xmlWriter);

        var projectFile = SourceDirectory.GetFile(m_Assembly.Name + ".csproj");
        await IOUtility.CompareAndWriteAsync(projectFile.AbsolutePath, writer.ToString(), cancellationToken);

        string includePaths = TargetDirectory.Binaries.BinariesOut(TargetPlatform.Win64, Configuration.Development);
        if (TargetDirectory.IsEngineDirectory() == false)
        {
            includePaths += ";" + Global.EngineDirectory.Binaries.BinariesOut(TargetPlatform.Win64, Configuration.Development);
        }

        var launchSettingsJson = new JsonObject
        {
            ["profiles"] = new JsonObject
            {
                [m_Assembly.Name] = new JsonObject
                {
                    ["commandName"] = "Project",
                    ["workingDirectory"] = TargetDirectory.Root.AbsolutePath,
                    ["environmentVariables"] = new JsonObject
                    {
                        ["PATH"] = includePaths + ";%PATH%"
                    }
                }
            }
        };

        var launchSettings = SourceDirectory.GetChild("Properties").GetFile("launchSettings.json");
        var serializerOptions = new JsonSerializerOptions
        {
            WriteIndented = true
        };
        await IOUtility.CompareAndWriteAsync(launchSettings.AbsolutePath, launchSettingsJson.ToJsonString(serializerOptions), cancellationToken);
    }
}
