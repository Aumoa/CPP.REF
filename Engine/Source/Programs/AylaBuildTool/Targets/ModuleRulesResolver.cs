using Spectre.Console;

namespace AylaEngine;

internal class ModuleRulesResolver
{
    public ModuleRulesResolver(Solution solution, ModuleRules rules)
    {
        Name = rules.Name;
        PrivateDependencyModuleNames = rules.PrivateDependencyModuleNames.Distinct().ToArray();
        PrivateIncludePaths = rules.PrivateIncludePaths.Distinct().ToArray();

        HashSet<string> route = new();
        List<string> dependencyModuleNames = [];
        List<string> includePaths = [];
        Resolve(solution, rules, route, true, dependencyModuleNames, includePaths);
        PublicDependencyModuleNames = dependencyModuleNames.Distinct().ToArray();
        PublicIncludePaths = includePaths.Distinct().ToArray();
    }

    private void Resolve(Solution solution, ModuleRules rules, HashSet<string> route, bool addPrivate, List<string> dependencyModuleNames, List<string> includePaths)
    {
        if (route.Add(rules.Name) == false)
        {
            return;
        }

        var targetProject = (ModuleProject)solution.FindProject(rules.Name)!;

        dependencyModuleNames.AddRange(rules.PublicDependencyModuleNames);
        includePaths.AddRange(rules.PublicIncludePaths.Select(AbsoluteIncludePath));

        IEnumerable<string> deps = rules.PublicDependencyModuleNames;
        if (addPrivate)
        {
            deps = deps.Concat(rules.PrivateDependencyModuleNames);
        }

        foreach (var dep in deps)
        {
            var dependTargetProject = solution.FindProject(dep);
            if (dependTargetProject == null)
            {
                AnsiConsole.MarkupLine("[red]Depend module [b]{0}[/] is not included in current solution.[/]");
                throw TerminateException.User();
            }

            if (dependTargetProject is not ModuleProject mp)
            {
                AnsiConsole.MarkupLine("[red]Depend module [b]{0}[/] is not Module project.[/]");
                throw TerminateException.User();
            }

            var dependTargetRule = mp.GetRule(rules.TargetInfo);
            Resolve(solution, dependTargetRule, route, false, dependencyModuleNames, includePaths);
        }

        return;

        string AbsoluteIncludePath(string relativeIncludePath)
        {
            return Path.Combine(targetProject.SourceDirectory, relativeIncludePath);
        }
    }

    public readonly string Name;

    public readonly string[] PrivateDependencyModuleNames;
    public readonly string[] PublicDependencyModuleNames;

    public readonly string[] PrivateIncludePaths;
    public readonly string[] PublicIncludePaths;
}
