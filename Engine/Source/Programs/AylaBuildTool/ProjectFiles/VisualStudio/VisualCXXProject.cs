// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.Misc;
using AE.Platform.Windows;
using AE.Projects;
using AE.Rules;

using System.Xml;

namespace AE.ProjectFiles.VisualStudio;

public class VisualCXXProject : IProject
{
    private readonly string ProjectName;
    private readonly CXXProject CXXProject;

    private static string GetFilterName(TargetClass Class)
    {
        return Class switch
        {
            TargetClass.Engine => "Engine",
            TargetClass.Game => "Games",
            TargetClass.Program => "Programs",
            _ => throw new ArgumentException("Invalid enum.", nameof(Class))
        };
    }

    public VisualCXXProject(CXXProject CXXProject)
    {
        ProjectName = CXXProject.Rules.Name;
        FilterPath = GetFilterName(CXXProject.Rules.Class);
        this.CXXProject = CXXProject;
        ProjectGuid = CRC32.GenerateGuid(Path.Combine(FilterPath, ProjectName)).ToString().ToUpper();
    }

    public string TargetName => ProjectName;

    public string FilterPath { get; init; }

    public string ProjectGuid { get; init; }

    public void GenerateXmlDocument(Workspace InWorkspace, out XmlDocument Vcxproj, out XmlDocument VcxprojFilters, out XmlDocument VcxprojUser)
    {
        XmlDocument Doc = new();
        Doc.AddXmlDeclaration("1.0", "utf-8");
        XmlElement Project = Doc.AddElement("Project", "http://schemas.microsoft.com/developer/msbuild/2003");
        {
            Project.SetAttribute("DefaultTargets", "Build");
            var ProjectConfigurations = Project.AddElement("ItemGroup");
            {
                ProjectConfigurations.SetAttribute("Label", "ProjectConfigurations");

                BuildConfiguration.ForEach((Configuration, Platform) =>
                {
                    var ProjectConfiguration = ProjectConfigurations.AddElement("ProjectConfiguration");
                    ProjectConfiguration.SetAttribute("Include", $"{Configuration}|{Platform}");

                    ProjectConfiguration.AddElement("Configuration").InnerText = Configuration.ToString();
                    ProjectConfiguration.AddElement("Platform").InnerText = Platform.ToString();

                    ProjectConfiguration = ProjectConfigurations.AddElement("ProjectConfiguration");
                    ProjectConfiguration.SetAttribute("Include", $"{Configuration}_Editor|{Platform}");

                    ProjectConfiguration.AddElement("Configuration").InnerText = Configuration.ToString() + "_Editor";
                    ProjectConfiguration.AddElement("Platform").InnerText = Platform.ToString();
                });
            }

            var Globals = Project.AddElement("PropertyGroup");
            {
                Globals.SetAttribute("Label", "Globals");
                Globals.AddElement("VCProjectVersion").InnerText = "16.0";
                Globals.AddElement("Keyword").InnerText = "Win32Proj";
                Globals.AddElement("ProjectGuid").InnerText = $"{{{ProjectGuid}}}";
                Globals.AddElement("RootNamespace").InnerText = FilterPath.Replace(Path.DirectorySeparatorChar, '.');
                Globals.AddElement("WindowsTargetPlatformVersion").InnerText = "10.0";
            }

            var Import = Project.AddElement("Import");
            {
                Import.SetAttribute("Project", "$(VCTargetsPath)\\Microsoft.Cpp.Default.props");
            }

            BuildConfiguration.ForEach((Configuration, Platform) =>
            {
                var PropertyGroup = Project.AddElement("PropertyGroup");
                PropertyGroup.SetAttribute("Condition", $"'$(Configuration)|$(Platform)'=='{Configuration}|{Platform}'");
                PropertyGroup.SetAttribute("Label", "Configuration");

                PropertyGroup.AddElement("ConfigurationType").InnerText = "Makefile";
                PropertyGroup.AddElement("PlatformToolset").InnerText = "v143";
                PropertyGroup.AddElement("CharacterSet").InnerText = "Unicode";

                PropertyGroup = Project.AddElement("PropertyGroup");
                PropertyGroup.SetAttribute("Condition", $"'$(Configuration)|$(Platform)'=='{Configuration}_Editor|{Platform}'");
                PropertyGroup.SetAttribute("Label", "Configuration");

                PropertyGroup.AddElement("ConfigurationType").InnerText = "Makefile";
                PropertyGroup.AddElement("PlatformToolset").InnerText = "v143";
                PropertyGroup.AddElement("CharacterSet").InnerText = "Unicode";
            });

            Import = Project.AddElement("Import");
            Import.SetAttribute("Project", "$(VCTargetsPath)\\Microsoft.Cpp.props");

            // Common
            var PropertyGroup = Project.AddElement("PropertyGroup");
            PropertyGroup.AddElement("NMakePreprocessorDefinitions").InnerText = string.Join(';', CXXProject.GetProjectDefinitions());
            PropertyGroup.AddElement("AdditionalOptions").InnerText = "/std:c++20";

            var Installation = VisualStudioInstallation.FindVisualStudioInstallations(Compiler.VisualStudio2022).First();

            BuildConfiguration.ForEach((Configuration, Platform) =>
            {
                // Foreach Configurations
                var PropertyGroup = Project.AddElement("PropertyGroup");
                PropertyGroup.SetAttribute("Condition", $"'$(Configuration)|$(Platform)'=='{Configuration}|{Platform}'");

                string BuildToolPath = InWorkspace.BuildTool;
                BuildToolPath = Path.ChangeExtension(BuildToolPath, ".exe");

                var IncludePaths = Installation.GetRequiredIncludePaths(Platform.Architecture);

                PropertyGroup.AddElement("NMakeBuildCommandLine").InnerText = $"{BuildToolPath} Build -Target {CXXProject.Rules.Name}";
                PropertyGroup.AddElement("NMakeReBuildCommandLine").InnerText = $"{BuildToolPath} Build -Clean -Target {CXXProject.Rules.Name}";
                PropertyGroup.AddElement("NMakeIncludeSearchPath").InnerText = $"{string.Join(';', IncludePaths)};%(NMakeIncludeSearchPath)";

                // Foreach Configurations
                PropertyGroup = Project.AddElement("PropertyGroup");
                PropertyGroup.SetAttribute("Condition", $"'$(Configuration)|$(Platform)'=='{Configuration}_Editor|{Platform}'");

                PropertyGroup.AddElement("NMakeBuildCommandLine").InnerText = $"{BuildToolPath} Build -Target {CXXProject.Rules.Name} -Editor";
                PropertyGroup.AddElement("NMakeReBuildCommandLine").InnerText = $"{BuildToolPath} Build -Clean -Target {CXXProject.Rules.Name} -Editor";
                PropertyGroup.AddElement("NMakeIncludeSearchPath").InnerText = $"{string.Join(';', IncludePaths)};%(NMakeIncludeSearchPath)";
            });

            var ItemGroup = Project.AddElement("ItemGroup");
            HashSet<string> Checker = new();

            XmlElement? AddSourceFile(string Filename)
            {
                if (Checker.Add(Filename) == false)
                {
                    return null;
                }

                string Extension = Path.GetExtension(Filename).ToLower();
                if (IsSourceFile(Extension))
                {
                    var ClCompile = ItemGroup.AddElement("ClCompile");
                    ClCompile.SetAttribute("Include", Filename);
                    return ClCompile;
                }
                else if (IsHeaderFile(Extension))
                {
                    var ClInclude = ItemGroup.AddElement("ClInclude");
                    ClInclude.SetAttribute("Include", Filename);
                    return null;
                }
                else if (IsNoneFile(Extension))
                {
                    var None = ItemGroup.AddElement("None");
                    None.SetAttribute("Include", Filename);
                    return null;
                }
                else if (IsNatvisFile(Extension))
                {
                    var None = ItemGroup.AddElement("Natvis");
                    None.SetAttribute("Include", Filename);
                    return null;
                }

                return null;
            }

            foreach (var ModuleName in CXXProject.GetModules())
            {
                var Resolved = CXXProject.GetModuleRule(ModuleName);
                if (Resolved == null)
                {
                    continue;
                }

                foreach (var Filename in Resolved.SourceFiles)
                {
                    XmlElement? Elem = AddSourceFile(Filename);
                    if (Elem != null)
                    {
                        string FilterSharedLibrary(string p)
                            => p.Replace("${CMAKE_SHARED_LIBRARY_EXPORT}", "__declspec(dllexport)")
                                .Replace("${CMAKE_SHARED_LIBRARY_IMPORT}", "__declspec(dllimport)");

                        var PreprocessorDefinitions = Elem.AddElement("PreprocessorDefinitions");
                        PreprocessorDefinitions.InnerText = $"{string.Join(';', Resolved.AdditionalMacros.Select(FilterSharedLibrary))};%(PreprocessorDefinitions)";

                        var AdditionalIncludeDirectories = Elem.AddElement("AdditionalIncludeDirectories");
                        AdditionalIncludeDirectories.InnerText = $"{string.Join(';', Resolved.IncludePaths)};%(AdditionalIncludeDirectories)";
                    }
                }
            }

            AddSourceFile(CXXProject.TargetFile);

            Import = Project.AddElement("Import");
            Import.SetAttribute("Project", "$(VCTargetsPath)\\Microsoft.Cpp.targets");

            var ImportGroup = Project.AddElement("ImportGroup");
            ImportGroup.SetAttribute("Label", "ExtensionTargets");
        }
        Vcxproj = Doc;

        Doc = new();
        Doc.AddXmlDeclaration("1.0", "utf-8");
        Project = Doc.AddElement("Project", "http://schemas.microsoft.com/developer/msbuild/2003");
        {
            Project.SetAttribute("ToolVersion", "4.0");
            var ItemGroup = Project.AddElement("ItemGroup");

            HashSet<string> Filters = new();
            foreach (var Filename in CXXProject.GetAllModuleSourceFiles())
            {
                string? FilterPath = Path.GetRelativePath(CXXProject.SourceCodeDirectory, Filename);
                FilterPath = Path.GetDirectoryName(FilterPath);
                FilterPath = Path.Combine("Source", FilterPath ?? "/");

                string[] Splices = FilterPath.Split(Path.DirectorySeparatorChar);
                string Composed = string.Empty;
                foreach (var Splice in Splices)
                {
                    Composed = Path.Combine(Composed, Splice);
                    Filters.Add(Composed);
                }

                string Extension = Path.GetExtension(Filename);
                XmlElement? InnerElement = null;
                if (IsSourceFile(Extension))
                {
                    var ClCompile = ItemGroup.AddElement("ClCompile");
                    ClCompile.SetAttribute("Include", Filename);
                    InnerElement = ClCompile;
                }
                else if (IsHeaderFile(Extension))
                {
                    var ClInclude = ItemGroup.AddElement("ClInclude");
                    ClInclude.SetAttribute("Include", Filename);
                    InnerElement = ClInclude;
                }
                else if (IsNoneFile(Extension))
                {
                    var None = ItemGroup.AddElement("None");
                    None.SetAttribute("Include", Filename);
                    InnerElement = None;
                }
                else if (IsNatvisFile(Extension))
                {
                    var None = ItemGroup.AddElement("Natvis");
                    None.SetAttribute("Include", Filename);
                    InnerElement = None;
                }

                if (InnerElement != null && string.IsNullOrEmpty(FilterPath) == false)
                {
                    InnerElement.AddElement("Filter").InnerText = FilterPath;
                }
            }

            foreach (var FilterPath in Filters)
            {
                var Filter = ItemGroup.AddElement("Filter");
                Filter.SetAttribute("Include", FilterPath);
                Filter.AddElement("UniqueIdentifier").InnerText = $"{{{CRC32.GenerateGuid(FilterPath).ToString().ToUpper()}}}";
            }
        }
        VcxprojFilters = Doc;

        Doc = new();
        Doc.AddXmlDeclaration("1.0", "utf-8");
        Project = Doc.AddElement("Project", "http://schemas.microsoft.com/developer/msbuild/2003");
        Project.SetAttribute("ToolsVersion", "Current");
        {
            foreach (var Platform in BuildConfiguration.Platforms)
            {
                var PropertyGroup = Project.AddElement("PropertyGroup");
                PropertyGroup.SetAttribute("Condition", $"'$(Platform)'=='{Platform}'");

                string Executable = Path.Combine(CXXProject.Workspace.BinariesDirectory, Platform.ToString(), CXXProject.Rules.TargetModuleName);
                Executable = Path.ChangeExtension(Executable, ".exe");
                PropertyGroup.AddElement("LocalDebuggerCommand").InnerText = Executable;

                string WorkingDirectory = Path.GetDirectoryName(Executable)!;
                PropertyGroup.AddElement("LocalDebuggerWorkingDirectory").InnerText = WorkingDirectory;

                PropertyGroup.AddElement("DebuggerFlavor").InnerText = "WindowsLocalDebugger";
                PropertyGroup.AddElement("LocalDebuggerDebuggerType").InnerText = "Auto";
            }
        }
        VcxprojUser = Doc;
    }

    private static bool IsHeaderFile(string Extensions)
    {
        return Extensions == ".h";
    }

    private static bool IsSourceFile(string Extensions)
    {
        return Extensions == ".cpp" || Extensions == ".cc";
    }

    private static bool IsNoneFile(string Extensions)
    {
        return Extensions == ".cs";
    }

    private static bool IsNatvisFile(string Extensions)
    {
        return Extensions == ".natvis";
    }
}
