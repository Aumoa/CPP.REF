// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;
using System.Xml;

using AE.BuildSettings;
using AE.Misc;
using AE.Platform.Windows;
using AE.Projects;
using AE.Source;

namespace AE.ProjectFiles.VisualStudio;

public class VisualCXXProject : IVisualStudioProject
{
    public required string TargetName { get; init; }

    public required string ProjectGuid { get; init; }

    public required string FilterPath { get; init; }

    public required string ProjectFile { get; init; }

    private ProjectDirectory ProjectDirectory { get; init; }

    private string SourceDirectory { get; init; }

    [SetsRequiredMembers]
    public VisualCXXProject(Workspace InWorkspace, ProjectDirectory ProjectDirectory, string ProjectName, string SourceDirectory, string FilterPath)
    {
        this.TargetName = ProjectName;
        this.ProjectGuid = CRC32.GenerateGuid(FilterPath + '/' + ProjectName).ToString().ToUpper();
        this.FilterPath = FilterPath;
        this.ProjectFile = Path.Combine(InWorkspace.CurrentTarget.Intermediate.ProjectFiles, Path.ChangeExtension(ProjectName, ".vcxproj"));
        this.ProjectDirectory = ProjectDirectory;
        this.SourceDirectory = SourceDirectory;
    }

    public (string, string) MapConfiguration(Configuration Configuration, bool bEditor, TargetPlatform Platform)
    {
        return (Configuration.ToString() + (bEditor ? "_Editor" : string.Empty), Platform.ToString());
    }

    public async Task GenerateProjectFilesAsync(CancellationToken SToken = default)
    {
        GenerateXmlDocument(out var Vcxproj, out var VcxprojFilter, out var VcxprojUser);
        await Task.WhenAll(new[]
        {
            WriteXml(Vcxproj, ProjectFile, ".vcxproj", SToken),
            WriteXml(VcxprojFilter, ProjectFile, ".vcxproj.filters", SToken),
            WriteXml(VcxprojUser, ProjectFile, ".vcxproj.user", SToken)
        });
    }

    private static async Task WriteXml(XmlDocument Doc, string SaveToBase, string SaveToExt, CancellationToken SToken = default)
    {
        StringWriter Writer = new();
        XmlTextWriter XmlWriter = new(Writer);
        XmlWriter.Formatting = Formatting.Indented;
        Doc.WriteTo(XmlWriter);
        await Global.CompareAndWriteAsync(Path.ChangeExtension(SaveToBase, SaveToExt), Writer.ToString(), SToken);
    }

    public void GenerateXmlDocument(out XmlDocument Vcxproj, out XmlDocument VcxprojFilters, out XmlDocument VcxprojUser)
    {
        HashSet<string> SourceFiles = new();

        XmlDocument Doc = new();
        Doc.AddXmlDeclaration("1.0", "utf-8");
        XmlElement Project = Doc.AddElement("Project", "http://schemas.microsoft.com/developer/msbuild/2003");
        {
            Project.SetAttribute("DefaultTargets", "Build");
            var ProjectConfigurations = Project.AddElement("ItemGroup");
            {
                ProjectConfigurations.SetAttribute("Label", "ProjectConfigurations");

                BuildConfiguration.ForEach((Configuration, bEditor, Platform) =>
                {
                    string App = bEditor ? "_Editor" : string.Empty;

                    var ProjectConfiguration = ProjectConfigurations.AddElement("ProjectConfiguration");
                    ProjectConfiguration.SetAttribute("Include", $"{Configuration}{App}|{Platform}");

                    ProjectConfiguration.AddElement("Configuration").InnerText = Configuration.ToString() + App;
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

            BuildConfiguration.ForEach((Configuration, bEditor, Platform) =>
            {
                string App = bEditor ? "_Editor" : string.Empty;
                var PropertyGroup = Project.AddElement("PropertyGroup");
                PropertyGroup.SetAttribute("Condition", $"'$(Configuration)|$(Platform)'=='{Configuration}{App}|{Platform}'");
                PropertyGroup.SetAttribute("Label", "Configuration");

                PropertyGroup.AddElement("ConfigurationType").InnerText = "Makefile";
                PropertyGroup.AddElement("PlatformToolset").InnerText = "v143";
                PropertyGroup.AddElement("CharacterSet").InnerText = "Unicode";
            });

            Import = Project.AddElement("Import");
            Import.SetAttribute("Project", "$(VCTargetsPath)\\Microsoft.Cpp.props");

            var Installation = VisualStudioInstallation.FindVisualStudioInstallations(CompilerVersion.VisualStudio2022).First();

            BuildConfiguration.ForEach((Configuration, bEditor, Platform) =>
            {
                string App = bEditor ? "_Editor" : "";
                string TargetApp = bEditor ? "Editor" : "";

                // Foreach Configurations
                var PropertyGroup = Project.AddElement("PropertyGroup");
                PropertyGroup.SetAttribute("Condition", $"'$(Configuration)|$(Platform)'=='{Configuration}{App}|{Platform}'");

                string BuildToolPath = Global.BuildToolPath;
                BuildToolPath = Path.ChangeExtension(BuildToolPath, ".exe");

                var IncludePaths = Installation.GetRequiredIncludePaths(Platform.Architecture);

                PropertyGroup.AddElement("NMakeBuildCommandLine").InnerText = $"{BuildToolPath} Build -Target {TargetName}{TargetApp}";
                PropertyGroup.AddElement("NMakeReBuildCommandLine").InnerText = $"{BuildToolPath} Build -Clean -Target {TargetName}{TargetApp}";
                PropertyGroup.AddElement("NMakeCleanCommandLine").InnerText = $"{BuildToolPath} Clean";
                PropertyGroup.AddElement("OutDir").InnerText = ProjectDirectory.Binaries.Win64;
                PropertyGroup.AddElement("IntDir").InnerText = ProjectDirectory.Intermediate.Unused;
            });

            var ItemGroup = Project.AddElement("ItemGroup");

            XmlElement? AddSourceFile(string Filename)
            {
                if (SourceFiles.Add(Filename) == false)
                {
                    return null;
                }

                string Extension = Path.GetExtension(Filename).ToLower();
                if (Global.IsSourceCode(Extension))
                {
                    var ClCompile = ItemGroup.AddElement("ClCompile");
                    ClCompile.SetAttribute("Include", Filename);
                    return ClCompile;
                }
                else if (Global.IsHeaderFile(Extension))
                {
                    var ClInclude = ItemGroup.AddElement("ClInclude");
                    ClInclude.SetAttribute("Include", Filename);
                    return null;
                }
                else if (Global.IsRuleFile(Extension))
                {
                    var None = ItemGroup.AddElement("None");
                    None.SetAttribute("Include", Filename);
                    return null;
                }
                else if (Global.IsNatvisFile(Extension))
                {
                    var None = ItemGroup.AddElement("Natvis");
                    None.SetAttribute("Include", Filename);
                    return null;
                }

                return null;
            }

            foreach (var SourceFile in Directory.GetFiles(SourceDirectory, "*", SearchOption.AllDirectories))
            {
                if (SourceDirectory.StartsWith(ProjectDirectory.Source.Programs) == false && SourceFile.StartsWith(ProjectDirectory.Source.Programs))
                {
                    continue;
                }

                AddSourceFile(SourceFile);
            }

            //foreach (var ModuleName in CXXProject.GetModules())
            //{
            //    var Resolved = CXXProject.GetModuleRule(ModuleName);
            //    if (Resolved == null)
            //    {
            //        continue;
            //    }

            //    foreach (var Filename in Resolved.SourceFiles)
            //    {
            //        XmlElement? Elem = AddSourceFile(Filename);
            //        if (Elem != null)
            //        {
            //            string FilterSharedLibrary(string p)
            //                => p.Replace("${CMAKE_SHARED_LIBRARY_EXPORT}", "__declspec(dllexport)")
            //                    .Replace("${CMAKE_SHARED_LIBRARY_IMPORT}", "__declspec(dllimport)");

            //            var PreprocessorDefinitions = Elem.AddElement("PreprocessorDefinitions");
            //            PreprocessorDefinitions.InnerText = $"{string.Join(';', Resolved.AdditionalMacros.Select(FilterSharedLibrary))};%(PreprocessorDefinitions)";

            //            var AdditionalIncludeDirectories = Elem.AddElement("AdditionalIncludeDirectories");
            //            AdditionalIncludeDirectories.InnerText = $"{string.Join(';', Resolved.IncludePaths)};%(AdditionalIncludeDirectories)";
            //        }
            //    }
            //}

            //AddSourceFile(CXXProject.TargetFile);

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
            foreach (var Filename in SourceFiles)
            {
                string? FilterPath = Path.GetRelativePath(SourceDirectory, Filename);
                FilterPath = Path.GetDirectoryName(FilterPath)!;
                if (SourceDirectory.StartsWith(ProjectDirectory.Source.Programs) == false)
                {
                    FilterPath = Path.Combine("Source", FilterPath ?? "/");
                }

                string[] Splices = FilterPath.Split(Path.DirectorySeparatorChar);
                string Composed = string.Empty;
                foreach (var Splice in Splices)
                {
                    Composed = Path.Combine(Composed, Splice);
                    Filters.Add(Composed);
                }

                string Extension = Path.GetExtension(Filename);
                XmlElement? InnerElement = null;
                if (Global.IsSourceCode(Extension))
                {
                    var ClCompile = ItemGroup.AddElement("ClCompile");
                    ClCompile.SetAttribute("Include", Filename);
                    InnerElement = ClCompile;
                }
                else if (Global.IsHeaderFile(Extension))
                {
                    var ClInclude = ItemGroup.AddElement("ClInclude");
                    ClInclude.SetAttribute("Include", Filename);
                    InnerElement = ClInclude;
                }
                else if (Global.IsRuleFile(Extension))
                {
                    var None = ItemGroup.AddElement("None");
                    None.SetAttribute("Include", Filename);
                    InnerElement = None;
                }
                else if (Global.IsNatvisFile(Extension))
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
                if (string.IsNullOrEmpty(FilterPath) == false)
                {
                    var Filter = ItemGroup.AddElement("Filter");
                    Filter.SetAttribute("Include", FilterPath);
                    Filter.AddElement("UniqueIdentifier").InnerText = $"{{{CRC32.GenerateGuid(FilterPath).ToString().ToUpper()}}}";
                }
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

                //string Executable = Path.Combine(CXXProject.Workspace.BinariesDirectory, Platform.ToString(), CXXProject.Rules.TargetModuleName);
                //Executable = Path.ChangeExtension(Executable, ".exe");
                //PropertyGroup.AddElement("LocalDebuggerCommand").InnerText = Executable;

                //string WorkingDirectory = Path.GetDirectoryName(Executable)!;
                //PropertyGroup.AddElement("LocalDebuggerWorkingDirectory").InnerText = WorkingDirectory;

                //PropertyGroup.AddElement("DebuggerFlavor").InnerText = "WindowsLocalDebugger";
                //PropertyGroup.AddElement("LocalDebuggerDebuggerType").InnerText = "Auto";
            }
        }
        VcxprojUser = Doc;
    }
}
