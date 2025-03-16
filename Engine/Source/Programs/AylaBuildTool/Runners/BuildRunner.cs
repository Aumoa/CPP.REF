using System.Runtime.Loader;
using Spectre.Console;

namespace AylaEngine;

internal static class BuildRunner
{
    private struct CompileItem
    {
        public ModuleRulesResolver Resolver { get; init; }

        public SourceCodeDescriptor SourceCode { get; init; }
    }

    public static async ValueTask RunAsync(BuildOptions options, CancellationToken cancellationToken)
    {
        string? projectPath = null;
        if (options.ProjectFile != null)
        {
            projectPath = Path.GetDirectoryName(options.ProjectFile);
        }

        var solution = await Solution.ScanProjectsAsync(Global.EngineDirectory, projectPath, cancellationToken);
        List<CompileItem> compileTargets = [];
        foreach (var project in solution.AllProjects)
        {
            if (project is ModuleProject mp)
            {
                var resolver = new ModuleRulesResolver(solution, ModuleRules.New(mp.RuleType, new TargetInfo { Platform = PlatformInfo.Win64 }));

                foreach (var sourceCode in mp.GetSourceCodes())
                {
                    if (sourceCode.Type == SourceCodeType.SourceCode)
                    {
                        compileTargets.Add(new CompileItem
                        {
                            Resolver = resolver,
                            SourceCode = sourceCode
                        });
                    }
                }
            }
        }

        AnsiConsole.WriteLine(string.Join("\n", compileTargets.Select(p => p.SourceCode.FilePath)));
    }
}
