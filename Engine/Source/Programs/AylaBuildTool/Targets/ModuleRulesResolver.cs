using Spectre.Console;

namespace AylaEngine;

internal class ModuleRulesResolver
{
    public ModuleRulesResolver(TargetInfo targetInfo, Solution solution, ModuleRules rules, GroupDescriptor group)
    {
        var targetProject = (ModuleProject)solution.FindProject(rules.Name)!;
        Rules = rules;
        RuleFilePath = targetProject.RuleFilePath;
        Name = rules.Name;
        Group = group;

        PrivateDependencyModuleNames = rules.PrivateDependencyModuleNames.Distinct().ToArray();
        PrivateIncludePaths = rules.PrivateIncludePaths.Distinct().Select(p => AbsoluteIncludePath(targetProject, p)).ToArray();
        PrivateAdditionalMacros = rules.PrivateAdditionalMacros.Append($"PLATFORM_STRING=TEXT(\"{targetInfo.Platform}\")").Append($"CONFIG_STRING=TEXT(\"{targetInfo.Config}\")").Distinct().ToArray();
        PrivateDisableWarnings = rules.PrivateDisableWarnings.Distinct().ToArray();
        PrivateAdditionalLibraries = rules.PrivateAdditionalLibraries.Distinct().ToArray();

        HashSet<string> route = new();
        List<string> dependencyModuleNames = [];
        List<string> includePaths = [];
        List<MacroSet> additionalMacros = [];
        List<int> disableWarnings = [];
        List<string> additionalLibraries = [];
        Resolve(solution, targetProject, rules, route, true, dependencyModuleNames, includePaths, additionalMacros, disableWarnings, additionalLibraries);
        PublicDependencyModuleNames = dependencyModuleNames.Distinct().ToArray();
        PublicIncludePaths = includePaths.Distinct().ToArray();
        PublicAdditionalMacros = additionalMacros.Distinct().ToArray();
        PublicDisableWarnings = disableWarnings.Distinct().ToArray();
        PublicAdditionalLibraries = additionalLibraries.Distinct().ToArray();
    }

    private void Resolve(Solution solution, ModuleProject targetProject, ModuleRules rules, HashSet<string> route, bool isPrimary, List<string> dependencyModuleNames, List<string> includePaths, List<MacroSet> additionalMacros, List<int> disableWarnings, List<string> additionalLibraries)
    {
        if (route.Add(rules.Name) == false)
        {
            return;
        }

        dependencyModuleNames.AddRange(rules.PublicDependencyModuleNames);
        includePaths.AddRange(rules.PublicIncludePaths.Select(p => AbsoluteIncludePath(targetProject, p)));
        additionalMacros.AddRange(rules.PublicAdditionalMacros);
        disableWarnings.AddRange(rules.PublicDisableWarnings);
        additionalLibraries.AddRange(rules.PublicAdditionalLibraries);

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
            Resolve(solution, mp, dependTargetRule, route, false, dependencyModuleNames, includePaths, additionalMacros, disableWarnings, additionalLibraries);
        }

        return;
    }

    private static string AbsoluteIncludePath(ModuleProject targetProject, string relativeIncludePath)
    {
        return Path.Combine(targetProject.SourceDirectory, relativeIncludePath);
    }

    public readonly ModuleRules Rules;
    public readonly string RuleFilePath;
    public readonly string Name;
    public readonly GroupDescriptor Group;

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

    private readonly string[] PublicAdditionalLibraries;
    private readonly string[] PrivateAdditionalLibraries;

    public IEnumerable<string> AdditionalLibraries => PrivateAdditionalLibraries.Concat(PublicAdditionalLibraries).Distinct();
}
