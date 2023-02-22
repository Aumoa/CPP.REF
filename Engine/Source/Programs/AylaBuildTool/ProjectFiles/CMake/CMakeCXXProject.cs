// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.Projects;
using AE.Rules;

namespace AE.ProjectFiles.CMake;

public class CMakeCXXProject : IProject
{
    private readonly string ProjectName;
    private readonly CXXProject CXXProject;

    public CMakeCXXProject(CXXProject CXXProject)
    {
        ProjectName = CXXProject.Rules.Name;
        this.CXXProject = CXXProject;
    }

    public string Name => ProjectName;

    public string MakefilePath => Path.Combine(CXXProject.Workspace.ProjectFilesDirectory, CXXProject.Rules.Name, "CMakeLists.txt");

    public async Task GenerateCMakeProject(Workspace _, CancellationToken CToken = default)
    {
        var TargetDirectory = CXXProject.Workspace;
        var TargetRule = CXXProject.Rules;

        string OutputDir = Directory.GetParent(MakefilePath)!.FullName;
        Directory.CreateDirectory(OutputDir);

        List<string> Subdirectories = new();
        foreach (var ModuleName in CXXProject.GetModules())
        {
            var Resolved = CXXProject.GetModuleRule(ModuleName);
            string Name = Resolved.Rules.Name;
            string InnerDir = Path.Combine(OutputDir, Name);
            string SourceDirectory = Directory.GetParent(ModuleName)!.FullName;
            Directory.CreateDirectory(InnerDir);

            static string AsDefinition(string p) => $"\"-D{p}\"";
            static string AsIncludeDirectory(string p) => $"\"{p.Replace(Path.DirectorySeparatorChar, '/')}\"";
            static string AsLibrary(string p) => $"\"{p.Replace(Path.DirectorySeparatorChar, '/')}\"";

            string SubCMakeLists = $@"
CMAKE_MINIMUM_REQUIRED(VERSION 3.22)

PROJECT({Name})

ADD_DEFINITIONS(
    {string.Join(Environment.NewLine + '\t', Resolved.AdditionalMacros.Select(AsDefinition))}
)

INCLUDE_DIRECTORIES(
    {string.Join(Environment.NewLine + '\t', Resolved.IncludePaths.Select(AsIncludeDirectory))}
)

FILE(GLOB_RECURSE SRC_FILES ""{SourceDirectory.Replace(Path.DirectorySeparatorChar, '/')}/*.cpp"")

ADD_LIBRARY({Name} SHARED ${{SRC_FILES}})

TARGET_LINK_LIBRARIES({Name}
    {string.Join(Environment.NewLine + '\t', Resolved.AdditionalLibraries.Select(AsLibrary))}
)
";

            await File.WriteAllTextAsync(Path.Combine(InnerDir, "CMakeLists.txt"), SubCMakeLists, CToken);
            Subdirectories.Add($"ADD_SUBDIRECTORY({Name})");
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
    {string.Join(Environment.NewLine + '\t', Definitions)}
)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ""{TargetDirectory.BinariesDirectory.Replace(Path.DirectorySeparatorChar, '/')}/Win64"")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ""{TargetDirectory.BinariesDirectory.Replace(Path.DirectorySeparatorChar, '/')}/Win64"")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ""{TargetDirectory.BinariesDirectory.Replace(Path.DirectorySeparatorChar, '/')}/Win64"")

SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_FLAGS ""/EHsc"")

{string.Join(Environment.NewLine + '\t', Subdirectories)}
";

        await File.WriteAllTextAsync(MakefilePath, CMakeLists, CToken);
    }
}
