// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Text;
using Microsoft.CodeAnalysis;

namespace AylaEngine;

internal static class CSGenerator
{
    public static string GenerateModule(Solution solution, ModuleProject project, bool includeSelfBindings, params IEnumerable<TargetInfo> targets)
    {
        var platforms = targets
            .Select(p => VSUtility.GetArchitectureName(p))
            .Distinct();

        var configurations = targets
            .Select(p => VSUtility.GetConfigName(p))
            .Distinct();

        var csproj = new CSharpProject(
            [
                new CSharpProject.PropertyGroup
                {
                    TargetFramework = "net9.0",
                    Nullable = NullableContextOptions.Enable,
                    RootNamespace = project.Group.IsEngine ? "Ayla" : "Game",
                    Platforms = platforms,
                    Configurations = configurations,
                    AppendTargetFrameworkToOutputPath = false
                },
                ..targets.Select(buildConfig => new CSharpProject.PropertyGroup
                {
                    Condition = new CSharpProject.Condition
                    {
                        Configuration = VSUtility.GetConfigName(buildConfig),
                        Platform = VSUtility.GetArchitectureName(buildConfig)
                    },
                    PropertyTarget = VSUtility.GetArchitectureName(buildConfig),
                    Optimize = buildConfig.Config.IsOptimized(),
                    OutputPath = project.Group.Output(buildConfig, FolderPolicy.PathType.Windows)
                })
            ],
            targets.Select(buildConfig =>
            {
                var rules = project.GetRule(buildConfig);
                var resolver = project.GetResolver(buildConfig);

                List<CSharpProject.ReferenceBase> references = [];
                if (includeSelfBindings)
                {
                    if (rules.DisableGenerateBindings == false)
                    {
                        references.Add(new CSharpProject.Reference
                        {
                            Include = project.Name + ".Bindings",
                            HintPath = Path.Combine(project.Group.Output(buildConfig, FolderPolicy.PathType.Windows), project.Name + ".Bindings.dll")
                        });
                    }
                }

                foreach (var depend in resolver.DependencyModuleNames)
                {
                    var dependProject = solution.FindProject(depend);
                    if (dependProject is ModuleProject mp)
                    {
                        var dependProjectRule = mp.GetRule(buildConfig);
                        if (dependProjectRule.EnableScript)
                        {
                            references.Add(new CSharpProject.ProjectReference
                            {
                                Include = Path.Combine(mp.SourceDirectory, "Script", mp.Name + ".Script.csproj")
                            });
                        }

                        if (dependProjectRule.DisableGenerateBindings == false)
                        {
                            references.Add(new CSharpProject.Reference
                            {
                                Include = dependProject.Name + ".Bindings",
                                HintPath = Path.Combine(dependProject.Group.Output(buildConfig, FolderPolicy.PathType.Windows), dependProject.Name + ".Bindings.dll")
                            });
                        }
                    }
                }

                return new CSharpProject.ItemGroup
                {
                    Condition = new CSharpProject.Condition
                    {
                        Configuration = VSUtility.GetConfigName(buildConfig),
                        Platform = VSUtility.GetArchitectureName(buildConfig)
                    },
                    References = references
                };
            })
        );

        return csproj.GenerateXml();
    }
}
