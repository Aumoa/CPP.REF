namespace AylaEngine;

internal static class BootstrapRunner
{
    public static async ValueTask RunAsync(BootstrapOptions options, CancellationToken cancellationToken)
    {
        var projectName = options.ProjectName;
        var className = projectName.Replace('.', '_');
        var targetDir = Path.GetFullPath(options.TargetDirectory);

        var aprojectPath = Path.Combine(targetDir, projectName + ".aproject");
        var sourceDir = Path.Combine(targetDir, "Source", projectName);
        var publicDir = Path.Combine(sourceDir, "Public");
        var privateDir = Path.Combine(sourceDir, "Private");

        Directory.CreateDirectory(publicDir);
        Directory.CreateDirectory(privateDir);

        await Task.WhenAll(
            File.WriteAllTextAsync(aprojectPath, string.Empty, cancellationToken),
            File.WriteAllTextAsync(Path.Combine(sourceDir, projectName + ".Module.cs"), MakeModuleCs(className), cancellationToken),
            File.WriteAllTextAsync(Path.Combine(publicDir, projectName + ".h"), MakeProjectHeader(projectName), cancellationToken)
        );

        Console.WriteLine("Bootstrap complete. Project '{0}' created at '{1}'.", projectName, targetDir);

        await GenerateRunner.RunAsync(new GenerateOptions
        {
            ProjectFile = aprojectPath,
            GeneratorType = options.GeneratorType,
        }, cancellationToken);
    }

    private static string MakeModuleCs(string className) => $$"""
        using AylaEngine;

        public class {{className}} : ModuleRules
        {
            public {{className}}()
            {
                Type = ModuleType.Game;
                AddPublicIncludePaths("Public");
                AddPrivateIncludePaths("Private");
                AddPrivateDependencyModuleNames("Core", "Engine");
            }
        }
        """.ReplaceLineEndings("\n");

    private static string MakeProjectHeader(string projectName) => $$"""
        #pragma once

        #include "CoreMinimal.h"

        namespace {{projectName}}
        {
            using namespace Ayla;
        }
        """.ReplaceLineEndings("\n");
}
