// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;

using AE.BuildSettings;
using AE.Misc;
using AE.Projects;
using AE.Source;

namespace AE.ProjectFiles.VisualStudio;

public class VisualCSharpProject : IVisualStudioProject
{
    public required IAModule Module { get; init; }

    public string TargetName => Module.ModuleName;

    public string BaseNamespace => "AE." + Module.ModuleName;

    public required string ProjectGuid { get; init; }

    public required string ProjectFile { get; init; }

    public required string FilterPath { get; init; }

    private readonly bool bGenerateProject;

    public bool bInterop { get; }

    private readonly List<VisualCSharpProject> ReferencedProjects = new();

    [SetsRequiredMembers]
    public VisualCSharpProject(IAModule Module, bool bGenerateProject, bool bInterop)
    {
        this.Module = Module;
        this.bGenerateProject = bGenerateProject;
        this.bInterop = bInterop;

        ProjectGuid = CRC32.GenerateGuid(Module.SourcePath + $"_{bGenerateProject}_{bInterop}").ToString().ToUpper();

        if (Module.IsInProgramsDirectory)
        {
            FilterPath = "Programs";
        }
        else
        {
            if (Module.ProjectDirectory.IsEngineDirectory())
            {
                FilterPath = "Engine";
            }
            else
            {
                FilterPath = "Game";
            }
        }

        if (bGenerateProject)
        {
            if (bInterop)
            {
                FilterPath += "\\Interop";
                ProjectFile = Path.Combine(Module.ProjectDirectory.Intermediate.CSharp, Module.ModuleName + ".Interop", Module.ModuleName + ".Interop.csproj");
            }
            else
            {
                FilterPath += "\\Scripts";
                ProjectFile = Path.Combine(Module.SourcePath, "Scripts", Module.ModuleName + ".CSharp.csproj");
            }
        }
        else
        {
            ProjectFile = Path.Combine(Module.SourcePath, Module.ModuleName + ".csproj");
        }
    }

    public (string, string) MapConfiguration(Configuration Configuration, bool bEditor, TargetPlatform Platform)
    {
        if (Configuration == Configuration.Debug)
        {
            return ("Debug", "Any CPU");
        }
        else
        {
            return ("Release", "Any CPU");
        }
    }

    public void ResolveDependencies(IEnumerable<IVisualStudioProject> VSProjects)
    {
        if (Module is AScriptModule ScriptModule)
        {
            var Sources = VSProjects
                .OfType<VisualCSharpProject>()
                .Where(p => p.bInterop)
                .ToDictionary(p => p.TargetName, p => p);

            foreach (var DependModule in ScriptModule.DependModules)
            {
                if (Sources.TryGetValue(DependModule, out var DependProject))
                {
                    ReferencedProjects.Add(DependProject);
                }
            }
        }
    }

    public Task GenerateProjectFilesAsync(CancellationToken SToken = default)
    {
        if (bGenerateProject == false)
        {
            return Task.CompletedTask;
        }

        const string Template = """
<Project Sdk="Microsoft.NET.Sdk">

  <PropertyGroup>
    <TargetFramework>net8.0</TargetFramework>
    <ImplicitUsings>enable</ImplicitUsings>
    <Nullable>enable</Nullable>
    <RootNamespace>AE.{1}</RootNamespace>
    <OutputPath>{0}</OutputPath>
    <AppendTargetFrameworkToOutputPath>false</AppendTargetFrameworkToOutputPath>
    <AppendRuntimeIdentifierToOutputPath>false</AppendRuntimeIdentifierToOutputPath>
    <PlatformTarget>x64</PlatformTarget>
    <IsPublishable>False</IsPublishable>
    <AssemblyName>$(MSBuildProjectName)</AssemblyName>
    <OutputType>{3}</OutputType>
  </PropertyGroup>

  <ItemGroup>
{2}
  </ItemGroup>

  <ItemGroup>
    <Using Include="AE.CoreAObject.Object">
      <Alias>Object</Alias>
    </Using>
{4}
  </ItemGroup>

</Project>
""";

        const string ProjectReference = """
    <ProjectReference Include="{0}" />
""";

        const string NamespaceReference = """
    <Using Include="AE.{0}" />
""";

        List<string> ProjectReferences = new();
        List<string> NamespaceReferences = new();
        if (bInterop == false)
        {
            string InteropModule = Path.Combine(Module.ProjectDirectory.Intermediate.CSharp, Module.ModuleName + ".Interop", Module.ModuleName + ".Interop.csproj");
            ProjectReferences.Add(string.Format(ProjectReference, InteropModule));
        }
        else
        {
            string InteropModule = Path.Combine(Global.EngineDirectory.Source.Root, "Runtime", "CoreAObject.CSharp", "CoreAObject.CSharp.csproj");
            ProjectReferences.Add(string.Format(ProjectReference, InteropModule));

            foreach (var Referenced in ReferencedProjects)
            {
                ProjectReferences.Add(string.Format(ProjectReference, Referenced.ProjectFile));
                NamespaceReferences.Add(string.Format(NamespaceReference, Referenced.TargetName));
            }
        }

        string InteropOutDir = bInterop ? Module.ProjectDirectory.Binaries.Interop : Module.ProjectDirectory.Binaries.CSharp;
        string ModuleName = Module.ModuleName;
        string ProjectReferencesStr = string.Join(Environment.NewLine, ProjectReferences);
        string ExecutablePolicy = !bInterop && ModuleName.Contains("Launch") ? "WinExe" : "Library";
        string NamespaceReferencesStr = string.Join(Environment.NewLine, NamespaceReferences);

        return File.WriteAllTextAsync(ProjectFile, string.Format(Template, InteropOutDir, ModuleName, ProjectReferencesStr, ExecutablePolicy, NamespaceReferencesStr), SToken);
    }
}
