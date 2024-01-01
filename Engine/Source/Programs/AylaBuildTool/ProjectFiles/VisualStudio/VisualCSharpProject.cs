// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;

using AE.BuildSettings;
using AE.Extensions;
using AE.Misc;
using AE.Projects;

namespace AE.ProjectFiles.VisualStudio;

public class VisualCSharpProject : VisualStudioProject
{
    public required Module Module { get; init; }

    public override string name => Module.Name;

    public string BaseNamespace => "AE." + Module.Name;

    public override string guid { get; }

    public override string project { get; }

    public override string filter { get; }

    private readonly bool bGenerateProject;

    public bool bInterop { get; }

    [SetsRequiredMembers]
    public VisualCSharpProject(Module Module, bool bGenerateProject, bool bInterop)
    {
        this.Module = Module;
        this.bGenerateProject = bGenerateProject;
        this.bInterop = bInterop;

        guid = CRC32.GenerateGuid(Module.SourcePath + $"_{bGenerateProject}_{bInterop}").ToString().ToUpper();

        if (Module.isProgram)
        {
            filter = "Programs";
        }
        else
        {
            if (Module.ProjectDirectory.IsEngineDirectory())
            {
                filter = "Engine";
            }
            else
            {
                filter = "Game";
            }
        }

        if (bGenerateProject)
        {
            if (bInterop)
            {
                filter += "\\Interop";
                project = Path.Combine(Module.ProjectDirectory.Intermediate.CSharp, Module.Name + ".Interop", Module.Name + ".Interop.csproj");
            }
            else
            {
                filter += "\\Scripts";
                project = Path.Combine(Module.SourcePath, "Scripts", Module.Name + ".CSharp.csproj");
            }
        }
        else
        {
            project = Path.Combine(Module.SourcePath, Module.Name + ".csproj");
        }
    }

    public override (string, string) MapConfiguration(Configuration Configuration, bool bEditor, TargetPlatform Platform)
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

    public override Task GenerateProjectFilesAsync(CancellationToken SToken = default)
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

//        const string NamespaceReference = """
//    <Using Include="AE.{0}" />
//""";

        List<string> ProjectReferences = new();
        List<string> NamespaceReferences = new();
        if (bInterop == false)
        {
            string InteropModule = Path.Combine(Module.ProjectDirectory.Intermediate.CSharp, Module.Name + ".Interop", Module.Name + ".Interop.csproj");
            ProjectReferences.Add(string.Format(ProjectReference, InteropModule));
        }
        else
        {
            string InteropModule = Path.Combine(Global.EngineDirectory.Source.Root, "Runtime", "CoreAObject.CSharp", "CoreAObject.CSharp.csproj");
            ProjectReferences.Add(string.Format(ProjectReference, InteropModule));

            //foreach (var Referenced in ReferencedProjects)
            //{
            //    ProjectReferences.Add(string.Format(ProjectReference, Referenced.project));
            //    NamespaceReferences.Add(string.Format(NamespaceReference, Referenced.name));
            //}
        }

        string InteropOutDir = bInterop ? Module.ProjectDirectory.Binaries.Interop : Module.ProjectDirectory.Binaries.CSharp;
        string ModuleName = Module.Name;
        string ProjectReferencesStr = string.Join(Environment.NewLine, ProjectReferences);
        string ExecutablePolicy = !bInterop && ModuleName.Contains("Launch") ? "WinExe" : "Library";
        string NamespaceReferencesStr = string.Join(Environment.NewLine, NamespaceReferences);

        return File.WriteAllTextAsync(project, string.Format(Template, InteropOutDir, ModuleName, ProjectReferencesStr, ExecutablePolicy, NamespaceReferencesStr), SToken);
    }

    public override void ResolveDependencies(IEnumerable<VisualStudioProject> VSProjects)
    {
    }
}
