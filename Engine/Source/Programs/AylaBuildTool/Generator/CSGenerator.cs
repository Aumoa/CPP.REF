// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Text;

namespace AylaEngine;

internal static class CSGenerator
{
    public static string GenerateModule(Solution solution, ModuleProject project, params IEnumerable<TargetInfo> targets)
    {
        var platforms = string.Join(';', targets
            .Select(p => VSUtility.GetArchitectureName(p))
            .Distinct());

        var configurations = string.Join(';', targets
            .Select(p => VSUtility.GetCppConfigName(p))
            .Distinct());

        var builder = new StringBuilder();
        AppendFormatLine("""<Project Sdk="Microsoft.NET.Sdk">""");
        AppendFormatLine("""  """);
        AppendFormatLine("""  <PropertyGroup>""");
        AppendFormatLine("""    <TargetFramework>net9.0</TargetFramework>""");
        AppendFormatLine("""    <ImplictUsings>enable</ImplictUsings>""");
        AppendFormatLine("""    <Nullable>enable</Nullable>""");
        AppendFormatLine("""    <RootNamespace>{0}</RootNamespace>""", project.Descriptor.IsEngine ? "Ayla" : "Game");
        AppendFormatLine("""    <Platforms>{0}</Platforms>""", platforms);
        AppendFormatLine("""    <Configurations>{0}</Configurations>""", configurations);
        AppendFormatLine("""    <AppendTargetFrameworkToOutputPath>False</AppendTargetFrameworkToOutputPath>""", configurations);
        AppendFormatLine("""  </PropertyGroup>""");
        AppendFormatLine("""  """);
        foreach (var buildConfig in targets)
        {
            AppendFormatLine("""  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='{0}|{1}'">""", VSUtility.GetCppConfigName(buildConfig), VSUtility.GetArchitectureName(buildConfig));
            AppendFormatLine("""    <PropertyTarget>{0}</PropertyTarget>""", VSUtility.GetArchitectureName(buildConfig));
            AppendFormatLine("""    <Optimize>{0}</Optimize>""", buildConfig.Config.IsOptimized());
            AppendFormatLine("""    <OutputPath>{0}</OutputPath>""", project.Descriptor.Output(buildConfig, FolderPolicy.PathType.Windows));
            AppendFormatLine("""  </PropertyGroup>""");
        }
        AppendFormatLine("""  """);
        foreach (var buildConfig in targets)
        {
            AppendFormatLine("""  <ItemGroup Condition="'$(Configuration)|$(Platform)'=='{0}|{1}'">""", VSUtility.GetCppConfigName(buildConfig), VSUtility.GetArchitectureName(buildConfig));
            AppendFormatLine("""    <Reference Include="{0}.Bindings">""", project.Name);
            AppendFormatLine("""      <HintPath>{0}\{1}.Bindings.dll</HintPath>""", project.Descriptor.Output(buildConfig, FolderPolicy.PathType.Windows), project.Name);
            AppendFormatLine("""    </Reference>""");
            var rules = project.GetRule(buildConfig);
            var resolver = new ModuleRulesResolver(buildConfig, solution, rules, project.Descriptor);
            foreach (var depend in resolver.DependencyModuleNames)
            {
                var dependProject = solution.FindProject(depend);
                if (dependProject is ModuleProject mp)
                {
                    var dependProjectRule = mp.GetRule(buildConfig);
                    if (dependProjectRule.EnableScript)
                    {
                        AppendFormatLine("""    <ProjectReference Include="{0}" />""", Path.Combine(mp.SourceDirectory, "Script", mp.Name + ".Script.csproj"));
                    }
                    else if (dependProjectRule.DisableGenerateBindings == false)
                    {
                        AppendFormatLine("""    <Reference Include="{0}.Bindings">""", project.Name);
                        AppendFormatLine("""      <HintPath>{0}\{1}.Bindings.dll</HintPath>""", dependProject.Descriptor.Output(buildConfig, FolderPolicy.PathType.Windows), dependProject.Name);
                        AppendFormatLine("""    </Reference>""");
                    }
                }
            }
            AppendFormatLine("""  </ItemGroup>""");
        }
        AppendFormatLine("""  """);
        AppendFormatLine("""</Project>""");

        return builder.ToString();

        void AppendFormatLine(string format, params ReadOnlySpan<object?> args)
        {
            builder.AppendFormat(format + '\n', args);
        }
    }
}
