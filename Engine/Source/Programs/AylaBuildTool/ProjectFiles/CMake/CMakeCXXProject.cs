// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.Projects;
using AE.Rules;

using Microsoft.CodeAnalysis;

using System.Diagnostics;

namespace AE.ProjectFiles.CMake;

[DebuggerDisplay("{CXXProject}")]
public class CMakeCXXProject : IProject
{
    private readonly string ProjectName;
    private readonly CXXProject CXXProject;

    public CMakeCXXProject(CXXProject TargetProject)
    {
        ProjectName = TargetProject.Rules.Name;
        CXXProject = TargetProject;
    }

    public string TargetName => ProjectName;

    public string Name => CXXProject.Name;

    public string MakefilePath => Path.Combine(CXXProject.Workspace.ProjectFilesDirectory, CXXProject.Name, "CMakeLists.txt");

    public bool bEmptyProject { get; private set; }

    public async Task GenerateCMakeProject(Workspace _, CancellationToken CToken = default)
    {
        var TargetDirectory = CXXProject.Workspace;
        var TargetRule = CXXProject.Rules;

        string OutputDir = Directory.GetParent(MakefilePath)!.FullName;
        Directory.CreateDirectory(OutputDir);

        var Modules = CXXProject.GetModules();

        List<string> Subdirectories = new();
        foreach (var ModuleName in Modules)
        {
            await GenerateModuleMakefile(CXXProject, Subdirectories, ModuleName, OutputDir, CXXProject.Rules.Type, CToken);
        }

        if (Subdirectories.Count == 0)
        {
            bEmptyProject = true;
            return;
        }

        string[] Definitions = CXXProject.GetProjectDefinitions();

        string CMakeLists = $@"
CMAKE_MINIMUM_REQUIRED(VERSION 3.22)

PROJECT({CXXProject.Rules.Name})

ADD_COMPILE_DEFINITIONS(
    {string.Join("\n\t", Definitions)}
)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ""{TargetDirectory.BinariesDirectory.Replace(Path.DirectorySeparatorChar, '/')}/Win64"")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ""{TargetDirectory.BinariesDirectory.Replace(Path.DirectorySeparatorChar, '/')}/Win64"")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ""{TargetDirectory.BinariesDirectory.Replace(Path.DirectorySeparatorChar, '/')}/Win64"")

LINK_DIRECTORIES(""{TargetDirectory.BinariesDirectory.Replace(Path.DirectorySeparatorChar, '/')}/Win64"")

SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_STANDARD 20)

{string.Join('\n', Subdirectories)}
";

        await File.WriteAllTextAsync(MakefilePath, CMakeLists, CToken);
    }

    private static async Task GenerateModuleMakefile(CXXProject Project, List<string> Subdirectories, string ModuleName, string OutputDir, TargetType Type, CancellationToken CToken)
    {
        var Resolved = Project.GetModuleRule(ModuleName);
        if (Resolved == null)
        {
            return;
        }

        string Name = Resolved.Rules.Name;
        string InnerDir = Path.Combine(OutputDir, Name);
        string SourceDirectory = Directory.GetParent(ModuleName)!.FullName.Replace(Path.DirectorySeparatorChar, '/');
        string GenDirectory = Path.Combine(Project.Workspace.GeneratedDirectory, Name).Replace(Path.DirectorySeparatorChar, '/');
        string BuildDirectory = Path.Combine(Project.Workspace.IntermediateDirectory, "Build").Replace(Path.DirectorySeparatorChar, '/');
        Directory.CreateDirectory(InnerDir);

        static string AsDefinition(string p) => $"\"-D{p}\"";
        static string AsIncludeDirectory(string p) => $"\"{p.Replace(Path.DirectorySeparatorChar, '/')}\"";
        static string AsLibrary(string p) => $"\"{p.Replace(Path.DirectorySeparatorChar, '/')}\"";

        if (Project.Rules.TargetModuleName != Resolved.Rules.Name)
        {
            Type = TargetType.Module;
        }

        string ExecutablePrefix = Type switch
        {
            TargetType.ConsoleApplication => $"ADD_EXECUTABLE({Name}",
            TargetType.SlateApplication => $"ADD_EXECUTABLE({Name} WIN32",
            _ => $"ADD_LIBRARY({Name} SHARED",
        };

        IEnumerable<string> Links = Resolved.DependencyModules;
        Links = Links.Concat(Resolved.AdditionalLibraries.Select(AsLibrary));

        string SubCMakeLists = $@"
CMAKE_MINIMUM_REQUIRED(VERSION 3.22)

PROJECT({Name})

INCLUDE_DIRECTORIES(
    {string.Join("\n\t", Resolved.IncludePaths.Select(AsIncludeDirectory))}
)

FILE(GLOB_RECURSE CXX_FILES ""{SourceDirectory}/*.cpp"")
FILE(GLOB_RECURSE CC_FILES ""{SourceDirectory}/*.cc"")
FILE(GLOB_RECURSE GEN_FILES ""{GenDirectory}/*.gen.cpp"")

{ExecutablePrefix} ${{CXX_FILES}} ${{CC_FILES}} ${{GEN_FILES}})

TARGET_LINK_LIBRARIES({Name}
    {string.Join("\n\t", Links)}
)

TARGET_COMPILE_DEFINITIONS({Name} PRIVATE
    {string.Join("\n\t", Resolved.AdditionalMacros.Select(AsDefinition))}
)

ADD_CUSTOM_COMMAND(TARGET {Name}
    PRE_BUILD
    COMMAND ${{CMAKE_RUNTIME_OUTPUT_DIRECTORY}}/AylaHeaderTool.exe -SourceLocation ""{SourceDirectory}"" -Build ""{BuildDirectory}"" -Includes ""{Project.Workspace.GeneratedDirectory.Replace(Path.DirectorySeparatorChar, '/')}"" -Output ""${{CMAKE_RUNTIME_OUTPUT_DIRECTORY}}""
    VERBATIM
)
";

        await File.WriteAllTextAsync(Path.Combine(InnerDir, "CMakeLists.txt"), SubCMakeLists, CToken);
        Subdirectories.Add($"ADD_SUBDIRECTORY({Name})");
    }
}
