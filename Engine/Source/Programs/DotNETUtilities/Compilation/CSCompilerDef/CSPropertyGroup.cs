using System.Security;
using System.Xml;
using Microsoft.CodeAnalysis;

namespace AylaEngine;

public record CSPropertyGroup(
    CSCondition? Condition,
    OutputKind? OutputType,
    CSTargetFramework? TargetFramework,
    bool? ImplicitUsings,
    NullableContextOptions? Nullable,
    string? AssemblyName,
    string? RootNamespace,
    bool? AllowUnsafeBlocks,
    bool? EnableRuntimeMarshalling,
    bool? EnableDefaultNamespace,
    bool? AppendTargetFrameworkToOutputPath,
    bool? AppendRuntimeIdentifierToOutputPath,
    string[] Configurations,
    string[] Platforms,
    string? OutputPath,
    bool? Optimize,
    string[] DefineConstants,
    string? PlatformTarget
    ) : CSElement
{
    public override string GenerateXml()
    {
        List<string> attributes = [];
        if (OutputType.HasValue)
        {
            var outputTypeStr = OutputType.Value switch
            {
                OutputKind.DynamicallyLinkedLibrary => "Library",
                OutputKind.ConsoleApplication => "Exe",
                OutputKind.WindowsApplication => "WinExe",
                _ => throw new InvalidOperationException($"Unsupported OutputType '{OutputType.Value}'."),
            };
            attributes.Add($"  <OutputType>{outputTypeStr}</OutputType>");
        }
        if (TargetFramework.HasValue)
        {
            var targetFrameworkStr = TargetFramework.Value switch
            {
                CSTargetFramework.Net0900 => "net9.0",
                _ => throw new InvalidOperationException($"Unsupported TargetFramework '{TargetFramework.Value}'."),
            };
            attributes.Add($"  <TargetFramework>{targetFrameworkStr}</TargetFramework>");
        }
        if (ImplicitUsings.HasValue)
        {
            var implicitUsingsStr = ImplicitUsings.Value ? "enable" : "disable";
            attributes.Add($"  <ImplicitUsings>{implicitUsingsStr}</ImplicitUsings>");
        }
        if (Nullable.HasValue)
        {
            var nullableStr = Nullable.Value switch
            {
                NullableContextOptions.Enable => "enable",
                NullableContextOptions.Annotations => "annotations",
                NullableContextOptions.Warnings => "warnings",
                NullableContextOptions.Disable => "disable",
                _ => throw new InvalidOperationException($"Unsupported Nullable '{Nullable.Value}'."),
            };
            attributes.Add($"  <Nullable>{nullableStr}</Nullable>");
        }
        if (AssemblyName != null)
        {
            attributes.Add($"  <AssemblyName>{SecurityElement.Escape(AssemblyName)}</AssemblyName>");
        }
        if (RootNamespace != null)
        {
            attributes.Add($"  <RootNamespace>{SecurityElement.Escape(RootNamespace)}</RootNamespace>");
        }
        if (AllowUnsafeBlocks.HasValue)
        {
            attributes.Add($"  <AllowUnsafeBlocks>{AllowUnsafeBlocks.Value.ToString().ToLower()}</AllowUnsafeBlocks>");
        }
        if (EnableRuntimeMarshalling.HasValue)
        {
            attributes.Add($"  <EnableRuntimeMarshalling>{EnableRuntimeMarshalling.Value.ToString().ToLower()}</EnableRuntimeMarshalling>");
        }
        if (EnableDefaultNamespace.HasValue)
        {
            attributes.Add($"  <EnableDefaultNamespace>{EnableDefaultNamespace.Value.ToString().ToLower()}</EnableDefaultNamespace>");
        }
        if (AppendTargetFrameworkToOutputPath.HasValue)
        {
            attributes.Add($"  <AppendTargetFrameworkToOutputPath>{AppendTargetFrameworkToOutputPath.Value.ToString().ToLower()}</AppendTargetFrameworkToOutputPath>");
        }
        if (AppendRuntimeIdentifierToOutputPath.HasValue)
        {
            attributes.Add($"  <AppendRuntimeIdentifierToOutputPath>{AppendRuntimeIdentifierToOutputPath.Value.ToString().ToLower()}</AppendRuntimeIdentifierToOutputPath>");
        }
        if (Configurations.Length > 0)
        {
            attributes.Add($"  <Configurations>{string.Join(';', Configurations)}</Configurations>");
        }
        if (Platforms.Length > 0)
        {
            attributes.Add($"  <Platforms>{string.Join(';', Platforms)}</Platforms>");
        }
        if (OutputPath != null)
        {
            attributes.Add($"  <OutputPath>{SecurityElement.Escape(OutputPath)}</OutputPath>");
        }
        if (Optimize.HasValue)
        {
            attributes.Add($"  <Optimize>{Optimize.Value.ToString().ToLower()}</Optimize>");
        }
        if (DefineConstants.Length > 0)
        {
            attributes.Add($"  <DefineConstants>{string.Join(';', DefineConstants)}</DefineConstants>");
        }
        if (PlatformTarget != null)
        {
            attributes.Add($"  <PlatformTarget>{SecurityElement.Escape(PlatformTarget)}</PlatformTarget>");
        }

        string conditionStr = Condition != null ? $" Condition=\"{SecurityElement.Escape(Condition.ToString())}\"" : "";
        return $"""
<PropertyGroup{conditionStr}>
{string.Join("\n", attributes)}
</PropertyGroup>
""";
    }

    public static CSPropertyGroup Parse(XmlElement propertyGroup)
    {
        var conditionStr = propertyGroup.GetAttribute("Condition");
        CSCondition? condition = null;
        if (string.IsNullOrEmpty(conditionStr) == false)
        {
            condition = CSCondition.Parse(conditionStr);
        }

        OutputKind? outputType = null;
        CSTargetFramework? targetFramework = null;
        bool? implicitUsings = null;
        NullableContextOptions? nullable = null;
        string? assemblyName = null;
        string? rootNamespace = null;
        bool? allowUnsafeBlocks = null;
        bool? enableRuntimeMarshalling = null;
        bool? enableDefaultNamespace = null;
        bool? appendTargetFrameworkToOutputPath = null;
        bool? appendRuntimeIdentifierToOutputPath = null;
        string[] configurations = [];
        string[] platforms = [];
        string? outputPath = null;
        bool? optimize = null;
        string[] defineConstants = [];
        string? platformTarget = null;
        foreach (var childNode in propertyGroup.ChildNodes.OfType<XmlElement>())
        {
            switch (childNode.Name)
            {
                case "OutputType":
                    outputType = childNode.InnerText switch
                    {
                        "Library" => OutputKind.DynamicallyLinkedLibrary,
                        "Exe" => OutputKind.ConsoleApplication,
                        "WinExe" => OutputKind.WindowsApplication,
                        _ => throw new FormatException($"Unknown OutputType '{childNode.InnerText}'."),
                    };
                    break;
                case "TargetFramework":
                    targetFramework = childNode.InnerText switch
                    {
                        "net9.0" => CSTargetFramework.Net0900,
                        _ => throw new FormatException($"Unknown TargetFramework '{childNode.InnerText}'."),
                    };
                    break;
                case "ImplicitUsings":
                    implicitUsings = childNode.InnerText switch
                    {
                        "enable" => true,
                        "disable" => false,
                        _ => throw new FormatException($"Unknown ImplicitUsings value '{childNode.InnerText}'."),
                    };
                    break;
                case "Nullable":
                    nullable = childNode.InnerText switch
                    {
                        "enable" => NullableContextOptions.Enable,
                        "annotations" => NullableContextOptions.Annotations,
                        "warnings" => NullableContextOptions.Warnings,
                        "disable" => NullableContextOptions.Disable,
                        _ => throw new FormatException($"Unknown Nullable value '{childNode.InnerText}'."),
                    };
                    break;
                case "AssemblyName":
                    assemblyName = childNode.InnerText;
                    break;
                case "RootNamespace":
                    rootNamespace = childNode.InnerText;
                    break;
                case "AllowUnsafeBlocks":
                    allowUnsafeBlocks = bool.Parse(childNode.InnerText);
                    break;
                case "EnableRuntimeMarshalling":
                    enableRuntimeMarshalling = bool.Parse(childNode.InnerText);
                    break;
                case "EnableDefaultNamespace":
                    enableDefaultNamespace = bool.Parse(childNode.InnerText);
                    break;
                case "AppendTargetFrameworkToOutputPath":
                    appendTargetFrameworkToOutputPath = bool.Parse(childNode.InnerText);
                    break;
                case "AppendRuntimeIdentifierToOutputPath":
                    appendRuntimeIdentifierToOutputPath = bool.Parse(childNode.InnerText);
                    break;
                case "Configurations":
                    configurations = childNode.InnerText.Split(';', StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
                    break;
                case "Platforms":
                    platforms = childNode.InnerText.Split(';', StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
                    break;
                case "OutputPath":
                    outputPath = childNode.InnerText;
                    break;
                case "Optimize":
                    optimize = bool.Parse(childNode.InnerText);
                    break;
                case "DefineConstants":
                    defineConstants = childNode.InnerText.Split(';', StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
                    break;
                case "PlatformTarget":
                    platformTarget = childNode.InnerText;
                    break;
                default:
                    throw new FormatException($"Unknown element '{childNode.Name}' in PropertyGroup.");
            }
        }

        return new CSPropertyGroup(
            condition,
            outputType,
            targetFramework,
            implicitUsings,
            nullable,
            assemblyName,
            rootNamespace,
            allowUnsafeBlocks,
            enableRuntimeMarshalling,
            enableDefaultNamespace,
            appendTargetFrameworkToOutputPath,
            appendRuntimeIdentifierToOutputPath,
            configurations,
            platforms,
            outputPath,
            optimize,
            defineConstants,
            platformTarget
        );
    }
}
