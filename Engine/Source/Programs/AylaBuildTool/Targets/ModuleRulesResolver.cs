using Spectre.Console;

namespace AylaEngine;

internal class ModuleRulesResolver
{
    public ModuleRulesResolver(TargetInfo targetInfo, Solution solution, ModuleRules rules)
    {
        Name = rules.Name;

        var targetProject = (ModuleProject)solution.FindProject(rules.Name)!;
        PrivateDependencyModuleNames = rules.PrivateDependencyModuleNames.Distinct().ToArray();
        PrivateIncludePaths = rules.PrivateIncludePaths.Distinct().Select(p => AbsoluteIncludePath(targetProject, p)).ToArray();
        PrivateAdditionalMacros = rules.PrivateAdditionalMacros.Append($"PLATFORM_STRING=TEXT(\"{targetInfo.Platform}\")").Append($"CONFIG_STRING=TEXT(\"{targetInfo.Config}\")").Distinct().ToArray();
        PrivateDisableWarnings = rules.PrivateDisableWarnings.Distinct().ToArray();

        HashSet<string> route = new();
        List<string> dependencyModuleNames = [];
        List<string> includePaths = [];
        List<MacroSet> additionalMacros = [];
        List<int> disableWarnings = [];
        Resolve(solution, targetProject, rules, route, true, dependencyModuleNames, includePaths, additionalMacros, disableWarnings);
        PublicDependencyModuleNames = dependencyModuleNames.Distinct().ToArray();
        PublicIncludePaths = includePaths.Distinct().ToArray();
        PublicAdditionalMacros = additionalMacros.Distinct().ToArray();
        PublicDisableWarnings = disableWarnings.Distinct().ToArray();
    }

    private void Resolve(Solution solution, ModuleProject targetProject, ModuleRules rules, HashSet<string> route, bool isPrimary, List<string> dependencyModuleNames, List<string> includePaths, List<MacroSet> additionalMacros, List<int> disableWarnings)
    {
        if (route.Add(rules.Name) == false)
        {
            return;
        }

        dependencyModuleNames.AddRange(rules.PublicDependencyModuleNames);
        includePaths.AddRange(rules.PublicIncludePaths.Select(p => AbsoluteIncludePath(targetProject, p)));
        additionalMacros.AddRange(rules.PublicAdditionalMacros);
        disableWarnings.AddRange(rules.PublicDisableWarnings);

        IEnumerable<string> deps = rules.PublicDependencyModuleNames;
        if (isPrimary)
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
            Resolve(solution, mp, dependTargetRule, route, false, dependencyModuleNames, includePaths, additionalMacros, disableWarnings);
        }

        return;
    }

    private static string AbsoluteIncludePath(ModuleProject targetProject, string relativeIncludePath)
    {
        return Path.Combine(targetProject.SourceDirectory, relativeIncludePath);
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

    private readonly int[] PrivateDisableWarnings;
    private readonly int[] PublicDisableWarnings;

    public IEnumerable<int> DisableWarnings => PrivateDisableWarnings.Concat(PublicDisableWarnings).Distinct();
}
