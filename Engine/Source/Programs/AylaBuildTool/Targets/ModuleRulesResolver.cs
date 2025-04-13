using Spectre.Console;

namespace AylaEngine;

internal class ModuleRulesResolver
{
    private readonly ITargetInfo m_TargetInfo;

    public ModuleRulesResolver(ITargetInfo targetInfo, Solution solution, ModuleRules rules, GroupDescriptor group)
    {
        var targetProject = (ModuleProject)solution.FindProject(rules.Name)!;
        m_TargetInfo = targetInfo;
        Rules = rules;
        RuleFilePath = targetProject.RuleFilePath;
        Name = rules.Name;
        Group = group;
        EngineGroup = solution.EngineProjects.First().Group;
        PrimaryGroup = solution.PrimaryGroup;

        PrivateDependencyModuleNames = WithBuiltInDependencyModule(rules.PrivateDependencyModuleNames).Distinct().ToArray();
        PrivateIncludePaths = rules.PrivateIncludePaths.Distinct().Select(p => AbsoluteIncludePath(targetProject, p)).ToArray();
        PrivateAdditionalMacros = WithAdditionalMacros(rules.PrivateAdditionalMacros).Distinct().ToArray();
        PrivateDisableWarnings = rules.PrivateDisableWarnings.Distinct().ToArray();
        PrivateAdditionalLibraries = rules.PrivateAdditionalLibraries.Distinct().ToArray();

        HashSet<string> route = new();
        List<string> dependRuleFilePaths = [];
        List<string> dependencyModuleNames = [];
        List<string> includePaths = [];
        List<MacroSet> additionalMacros = [];
        List<int> disableWarnings = [];
        List<string> additionalLibraries = [];
        Resolve(solution, targetProject, rules, route, true, dependRuleFilePaths, dependencyModuleNames, includePaths, additionalMacros, disableWarnings, additionalLibraries);
        DependRuleFilePaths = dependRuleFilePaths.Distinct().ToArray();
        PublicDependencyModuleNames = dependencyModuleNames.Distinct().ToArray();
        PublicIncludePaths = includePaths.Distinct().ToArray();
        PublicAdditionalMacros = additionalMacros.Distinct().ToArray();
        PublicDisableWarnings = disableWarnings.Distinct().ToArray();
        PublicAdditionalLibraries = additionalLibraries.Distinct().ToArray();

        return;
    }

    private IEnumerable<MacroSet> WithAdditionalMacros(IEnumerable<MacroSet> source)
    {
        return source
            .Append($"PLATFORM_STRING=TEXT(\"{m_TargetInfo.Platform}\")")
            .Append($"CONFIG_STRING=TEXT(\"{m_TargetInfo.Config}\")")
            .Append($"WITH_EDITOR={(m_TargetInfo.Editor ? "1" : "0")}");
    }

    private IEnumerable<string> WithBuiltInDependencyModule(IEnumerable<string> source)
    {
        if (Rules.Type == ModuleType.Game)
        {
            switch (m_TargetInfo.Platform.Group)
            {
                case PlatformGroup.Windows:
                    source = source.Append("WindowsLaunch");
                    break;
                default:
                    AnsiConsole.MarkupLine("[red]Target platform not supported yet.[/]");
                    throw TerminateException.Internal();
            }
        }

        return source;
    }

    private void Resolve(Solution solution, ModuleProject targetProject, ModuleRules rules, HashSet<string> route, bool isPrimary, List<string> dependRuleFilePaths, List<string> dependencyModuleNames, List<string> includePaths, List<MacroSet> additionalMacros, List<int> disableWarnings, List<string> additionalLibraries)
    {
        if (route.Add(rules.Name) == false)
        {
            return;
        }

        var intDir = targetProject.Group.Intermediate(targetProject.Name, m_TargetInfo, FolderPolicy.PathType.Current);
        dependencyModuleNames.AddRange(rules.PublicDependencyModuleNames);
        includePaths.AddRange(rules.PublicIncludePaths.Select(p => AbsoluteIncludePath(targetProject, p)).Append(intDir));
        additionalMacros.AddRange(rules.PublicAdditionalMacros);
        disableWarnings.AddRange(rules.PublicDisableWarnings);
        additionalLibraries.AddRange(rules.PublicAdditionalLibraries);

        IEnumerable<string> deps = rules.PublicDependencyModuleNames;
        if (isPrimary)
        {
            deps = deps.Concat(WithBuiltInDependencyModule(rules.PrivateDependencyModuleNames));
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
                AnsiConsole.MarkupLine("[red]Project [b]{0}[/], referenced by Project [b]{1}[/], is not included in this solution.[/]", dep, targetProject.Name);
                throw TerminateException.User();
            }

            if (dependTargetProject is not ModuleProject mp)
            {
                AnsiConsole.MarkupLine("[red]Project [b]{0}[/], referenced by Project [b]{1}[/], is not a valid C++ project.[/]", dep, targetProject.Name);
                throw TerminateException.User();
            }

            var dependTargetRule = mp.GetRule(rules.TargetInfo);
            dependRuleFilePaths.Add(mp.RuleFilePath);

            Resolve(solution, mp, dependTargetRule, route, false, dependRuleFilePaths, dependencyModuleNames, includePaths, additionalMacros, disableWarnings, additionalLibraries);
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
    public readonly GroupDescriptor EngineGroup;
    public readonly GroupDescriptor PrimaryGroup;
    public readonly string[] DependRuleFilePaths;

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
