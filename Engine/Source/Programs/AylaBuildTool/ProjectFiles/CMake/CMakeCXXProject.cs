// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.Projects;

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

    public string Name => ProjectName;

    public string MakefilePath => Path.Combine(CXXProject.Workspace.ProjectFilesDirectory, CXXProject.Rules.Name, "CMakeLists.txt");

    public async Task GenerateCMakeProject(Workspace _, CancellationToken CToken = default)
    {
        var TargetDirectory = CXXProject.Workspace;
        var TargetRule = CXXProject.Rules;

        string OutputDir = Directory.GetParent(MakefilePath)!.FullName;
        Directory.CreateDirectory(OutputDir);

        bool bExecutable = CXXProject.Rules.Type == Rules.TargetType.ConsoleApplication;
        var Modules = CXXProject.GetModules();

        List<string> Subdirectories = new();
        foreach (var DependProject in CXXProject.DependencyProjects)
        {
            foreach (var ModuleName in DependProject.GetModules())
            {
                await GenerateModuleMakefile(DependProject, Subdirectories, ModuleName, OutputDir, false, CToken);
            }
        }
        foreach (var ModuleName in Modules)
        {
            await GenerateModuleMakefile(CXXProject, Subdirectories, ModuleName, OutputDir, bExecutable, CToken);
        }

        List<string> Definitions = new();
        static char Int(bool Value)
        {
            return Value ? '1' : '0';
        }

        PlatformGroup.ForEach(p =>
        {
            bool bOn = TargetRule.Platform.Group == p;
            Definitions.Add($"{p.ToDefinition()}={Int(bOn)}");
        });

        Definitions.Add($"SHIPPING={1}");
        Definitions.Add($"DO_CHECK={0}");

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

    private static async Task GenerateModuleMakefile(CXXProject Project, List<string> Subdirectories, string ModuleName, string OutputDir, bool bExecutable, CancellationToken CToken)
    {
        var Resolved = Project.GetModuleRule(ModuleName);
        if (Resolved == null)
        {
            return;
        }

        string Name = Resolved.Rules.Name;
        string InnerDir = Path.Combine(OutputDir, Name);
        string SourceDirectory = Directory.GetParent(ModuleName)!.FullName;
        Directory.CreateDirectory(InnerDir);

        static string AsDefinition(string p) => $"\"-D{p}\"";
        static string AsIncludeDirectory(string p) => $"\"{p.Replace(Path.DirectorySeparatorChar, '/')}\"";
        static string AsLibrary(string p) => $"\"{p.Replace(Path.DirectorySeparatorChar, '/')}\"";

        string LinkType = bExecutable ? "ADD_EXECUTABLE" : "ADD_LIBRARY";
        string ShareType = bExecutable ? "" : "SHARED";

        IEnumerable<string> Links = Resolved.DependencyModules;
        Links = Links.Concat(Resolved.AdditionalLibraries.Select(AsLibrary));

        string SubCMakeLists = $@"
CMAKE_MINIMUM_REQUIRED(VERSION 3.22)

PROJECT({Name})

INCLUDE_DIRECTORIES(
    {string.Join("\n\t", Resolved.IncludePaths.Select(AsIncludeDirectory))}
)

FILE(GLOB_RECURSE CXX_FILES ""{SourceDirectory.Replace(Path.DirectorySeparatorChar, '/')}/*.cpp"")
FILE(GLOB_RECURSE CC_FILES ""{SourceDirectory.Replace(Path.DirectorySeparatorChar, '/')}/*.cc"")

{LinkType}({Name} {ShareType} ${{CXX_FILES}} ${{CC_FILES}})

TARGET_LINK_LIBRARIES({Name}
    {string.Join("\n\t", Links)}
)

TARGET_COMPILE_DEFINITIONS({Name} PRIVATE
    {string.Join("\n\t", Resolved.AdditionalMacros.Select(AsDefinition))}
)
";

        await File.WriteAllTextAsync(Path.Combine(InnerDir, "CMakeLists.txt"), SubCMakeLists, CToken);
        Subdirectories.Add($"ADD_SUBDIRECTORY({Name})");
    }
}
