// Copyright 2020-2022 Aumoa.lib. All right reserved.

using DotNETUtilities;

using System.Xml;

namespace BuildTool.Compilers.VisualStudio;

public class VcxProject
{
    public record BuildConfiguration
    {
        private BuildConfiguration()
        {
        }

        public string Name { get; set; } = null!;

        public string Platform { get; set; } = null!;

        public string UseDebugLibraries { get; set; } = null!;

        public string WholeProgramOptimization { get; set; } = null!;

        public string LinkIncremental { get; set; } = null!;

        public static BuildConfiguration Debug { get; set; } = new()
        {
            Name = "Debug",
            Platform = "x64",
            UseDebugLibraries = "True",
            WholeProgramOptimization = "False",
            LinkIncremental = "True"
        };

        public static BuildConfiguration DebugGame { get; set; } = new()
        {
            Name = "DebugGame",
            Platform = "x64",
            UseDebugLibraries = "True",
            WholeProgramOptimization = "True",
            LinkIncremental = "False"
        };

        public static BuildConfiguration Development { get; set; } = new()
        {
            Name = "Development",
            Platform = "x64",
            UseDebugLibraries = "False",
            WholeProgramOptimization = "True",
            LinkIncremental = "False"
        };

        public static BuildConfiguration Shipping { get; set; } = new()
        {
            Name = "Shipping",
            Platform = "x64",
            UseDebugLibraries = "False",
            WholeProgramOptimization = "True",
            LinkIncremental = "False"
        };
    }

    public readonly static BuildConfiguration[] Configurations = new[]
    {
        BuildConfiguration.Debug,
        BuildConfiguration.DebugGame,
        BuildConfiguration.Development,
        BuildConfiguration.Shipping
    };

    public enum VCProjectVersion
    {
        V16_0
    }

    public enum VCProjectKeyword
    {
        Win32Proj
    }

    public enum WindowsPlatformTargetVersion
    {
        V10_0
    }

    public record GlobalConfiguration
    {
        public VCProjectVersion ProjectVersion { get; set; } = VCProjectVersion.V16_0;

        public VCProjectKeyword Keyword { get; set; } = VCProjectKeyword.Win32Proj;

        public Guid ProjectGuid { get; set; } = Guid.NewGuid();

        public string RootNamespace { get; set; } = "";

        public WindowsPlatformTargetVersion TargetPlatformVersion { get; set; } = WindowsPlatformTargetVersion.V10_0;
    }

    public GlobalConfiguration Globals { get; set; } = new();

    public VcxProject()
    {
    }

    public XmlDocument GenerateXml(SolutionDirectoryReference solution)
    {
        XmlDocument document = new();
        document.NewDeclaration();

        var project = document.NewElement("Project");
        GenerateProject(project, solution);

        return document;
    }

    private void GenerateProject(XmlElement project, SolutionDirectoryReference solution)
    {
        project.SetAttribute("DefaultTargets", "Build");
        project.SetAttribute("xmlns", "http://schemas.microsoft.com/developer/msbuild/2003");

        // Generate globals.
        GenerateProjectConfigurations(project.NewElement("ItemGroup"));
        GenerateGlobals(project.NewElement("PropertyGroup"));

        // Import global basic settings.
        var defaultImport = project.NewElement("Import");
        defaultImport.SetAttribute("Project", "$(VCTargetsPath)\\Microsoft.Cpp.Default.props");

        // Import basic settings.
        foreach (var config in Configurations)
        {
            GenerateBasicProperties(project.NewElement("PropertyGroup"), config);
        }

        // Import extend global settings.
        project.NewElementImport("$(VCTargetsPath)\\Microsoft.Cpp.props");
        project.NewElementImportGroup("ExtensionSettings");
        project.NewElementImportGroup("Shared");
        GenerateExtensionImports(project);

        // Generate property groups.
        project.NewElementPropertyGroup(Label: "UserMacros");
        foreach (var config in Configurations)
        {
            GeneratePropertyGroup(project.NewElement("PropertyGroup"), config, solution);
        }
    }

    private void GeneratePropertyGroup(XmlElement element, BuildConfiguration config, SolutionDirectoryReference solution)
    {
        element.SetAttribute("Condition", $"'$(Configuration)|$(Platform)'=='{config.Name}|{config.Platform}'");

        var path = solution.Build.FullPath;

        element.NewElement(nameof(config.LinkIncremental), config.LinkIncremental);
        element.NewElement("OutDir", solution.Build + "\\Win64\\$(Configuration)\\");
    }

    private static void GenerateExtensionImports(XmlElement project)
    {
        foreach (var config in Configurations)
        {
            var importGroup = project.NewElementImportGroup("PropertySheets", $"`$(Configuration)|$(Platform)'=='{config.Name}|{config.Platform}'");
            importGroup.NewElementImport("$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props", "exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')", "LocalAppDataPlatform");
        }
    }

    private void GenerateBasicProperties(XmlElement element, BuildConfiguration config)
    {
        element.SetAttribute("Condition", $"'$(Configuration)|$(Platform)'=='{config.Name}|{config.Platform}'");
        element.SetAttribute("Label", "Configuration");
        element.NewElement("ConfigurationType", "DynamicLibrary");
        element.NewElement(nameof(config.UseDebugLibraries), config.UseDebugLibraries);
        element.NewElement("PlatformToolset", Globals.ProjectVersion switch
        {
            VCProjectVersion.V16_0 => "v143",
            _ => throw new InvalidDataException()
        });
        element.NewElement(nameof(config.WholeProgramOptimization), config.WholeProgramOptimization);
        element.NewElement("CharacterSet", "Unicode");
        element.NewElement("EnableUnitySupport", "False");
    }

    private void GenerateGlobals(XmlElement element)
    {
        element.SetAttribute("Label", "Globals");

        element.NewElement("VCProjectVersion", Globals.ProjectVersion switch
        {
            VCProjectVersion.V16_0 => "16.0",
            _ => throw new InvalidDataException()
        });
        element.NewElement("Keyword", Globals.Keyword switch
        {
            VCProjectKeyword.Win32Proj => "Win32Proj",
            _ => throw new InvalidDataException()
        });
        element.NewElement("ProjectGuid", $"{{{Globals.ProjectGuid}}}");
        element.NewElement("RootNamespace", Globals.RootNamespace);
        element.NewElement("WindowsTargetPlatformVersion", Globals.TargetPlatformVersion switch
        {
            WindowsPlatformTargetVersion.V10_0 => "10.0",
            _ => throw new InvalidDataException()
        });
    }

    private static void GenerateProjectConfigurations(XmlElement element)
    {
        element.SetAttribute("Label", "ProjectConfigurations");
        foreach (var config in Configurations)
        {
            var item = element.NewElement("ProjectConfiguration");
            item.SetAttribute("Include", $"{config.Name}|{config.Platform}");
            item.NewElement("Configuration", config.Name);
            item.NewElement("Platform", config.Platform);
        }
    }
}
