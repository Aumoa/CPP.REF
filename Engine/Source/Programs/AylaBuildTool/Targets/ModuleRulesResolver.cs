using Spectre.Console;

namespace AylaEngine;

internal class ModuleRulesResolver
{
    public ModuleRulesResolver(Solution solution, ModuleRules rules)
    {
        Name = rules.Name;
        PrivateDependencyModuleNames = rules.PrivateDependencyModuleNames.Distinct().ToArray();
        PrivateIncludePaths = rules.PrivateIncludePaths.Distinct().ToArray();
        PrivateAdditionalMacros = rules.PrivateAdditionalMacros.Distinct().ToArray();

        HashSet<string> route = new();
        List<string> dependencyModuleNames = [];
        List<string> includePaths = [];
        List<MacroSet> additionalMacros = [];
        Resolve(solution, rules, route, true, dependencyModuleNames, includePaths, additionalMacros);
        PublicDependencyModuleNames = dependencyModuleNames.Distinct().ToArray();
        PublicIncludePaths = includePaths.Distinct().ToArray();
        PublicAdditionalMacros = additionalMacros.Distinct().ToArray();
    }

    private void Resolve(Solution solution, ModuleRules rules, HashSet<string> route, bool isPrimay, List<string> dependencyModuleNames, List<string> includePaths, List<MacroSet> additionalMacros)
    {
        if (route.Add(rules.Name) == false)
        {
            return;
        }

        var targetProject = (ModuleProject)solution.FindProject(rules.Name)!;

        dependencyModuleNames.AddRange(rules.PublicDependencyModuleNames);
        includePaths.AddRange(rules.PublicIncludePaths.Select(AbsoluteIncludePath));
        additionalMacros.AddRange(rules.PublicAdditionalMacros);

        IEnumerable<string> deps = rules.PublicDependencyModuleNames;
        if (isPrimay)
        {
            deps = deps.Concat(rules.PrivateDependencyModuleNames);
            additionalMacros.Add(rules.Name.ToUpper() + "_API=PLATFORM_SHARED_EXPORT");
        }
        else
        {
            additionalMacros.Add(rules.Name.ToUpper() + "_API=PLATFORM_SHARED_IMPORT");
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
            Resolve(solution, dependTargetRule, route, false, dependencyModuleNames, includePaths, additionalMacros);
        }

        return;

        string AbsoluteIncludePath(string relativeIncludePath)
        {
            return Path.Combine(targetProject.SourceDirectory, relativeIncludePath);
        }
    }

    public readonly string Name;

    private readonly string[] PrivateDependencyModuleNames;
    private readonly string[] PublicDependencyModuleNames;

    public IEnumerable<string> DependencyModuleNames => PrivateDependencyModuleNames.Concat(PublicDependencyModuleNames).Distinct();

    private readonly string[] PrivateIncludePaths;
    private readonly string[] PublicIncludePaths;

    public IEnumerable<string> IncludePaths => PrivateIncludePaths.Concat(PublicIncludePaths).Distinct();

    private readonly MacroSet[] PrivateAdditionalMacros;
    private readonly MacroSet[] PublicAdditionalMacros;

    public IEnumerable<MacroSet> AdditionalMacros => PrivateAdditionalMacros.Concat(PublicAdditionalMacros).Distinct();
}
