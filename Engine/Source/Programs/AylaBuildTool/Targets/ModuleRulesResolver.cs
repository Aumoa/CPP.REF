namespace AylaEngine;

internal class ModuleRulesResolver
{
    private readonly ITargetInfo m_TargetInfo;

    public ModuleRulesResolver(ITargetInfo targetInfo, Solution solution, ModuleProject targetProject, ModuleRules rules)
    {
        m_TargetInfo = targetInfo;
        Project = targetProject;
        Rules = rules;
        BuildProfile = BuildProfileResolver.Resolve(targetProject, targetInfo);
        EngineGroup = solution.EngineGroup;
        PrimaryGroup = solution.PrimaryGroup;
        m_PchUsage = rules.PchUsage;
        m_PrivatePchHeaderFile = rules.PrivatePchHeaderFile;

        PrivateDependencyModuleNames = WithBuiltInDependencyModule(rules.PrivateDependencyModuleNames).Distinct().ToArray();
        PrivateIncludePaths = rules.PrivateIncludePaths.Distinct().Select(p => AbsoluteIncludePath(targetProject, p)).ToArray();
        PrivateAdditionalMacros = rules.PrivateAdditionalMacros.Concat(BuiltinMacros).Distinct().ToArray();
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

    private IEnumerable<MacroSet> BuiltinMacros
    {
        get
        {
            yield return $"PLATFORM_STRING=TEXT(\"{m_TargetInfo.Platform}\")";
            yield return $"CONFIG_STRING=TEXT(\"{m_TargetInfo.Config}\")";
            yield return $"WITH_EDITOR={(m_TargetInfo.Editor ? "1" : "0")}";

            if (m_TargetInfo.Platform.Group == PlatformGroup.Windows)
            {
                yield return "PLATFORM_WINDOWS=1";
            }
            else if (m_TargetInfo.Platform.Group == PlatformGroup.Linux)
            {
                yield return "PLATFORM_LINUX=1";
            }
            else if (m_TargetInfo.Platform.Group == PlatformGroup.OSX)
            {
                yield return "PLATFORM_OSX=1";
            }
            else
            {
                throw new PlatformNotSupportedException();
            }

            yield return "_UNICODE";
            yield return "UNICODE";

            if (BuildProfile.EnablesAssertions)
            {
                yield return "DO_CHECK=1";
            }
        }
    }

    private IEnumerable<string> WithBuiltInDependencyModule(IEnumerable<string> source)
    {
        if (Rules.Type == ModuleType.Game)
        {
            switch (m_TargetInfo.Platform.Group)
            {
                case PlatformGroup.Windows:
                    source = source.Append("WindowsAPI");
                    break;
                case PlatformGroup.Linux:
                    source = source.Append("LinuxAPI");
                    break;
                case PlatformGroup.OSX:
                    source = source.Append("OSXAPI");
                    break;
                default:
                    Console.Error.WriteLine("Target platform not supported yet.");
                    throw TerminateException.Internal();
            }
        }
        else if (Rules.Type == ModuleType.Console)
        {
            // Console applications only need platform APIs on desktop platforms
            // No ApplicationCore dependency required
            switch (m_TargetInfo.Platform.Group)
            {
                case PlatformGroup.Windows:
                    source = source.Append("WindowsAPI");
                    break;
                case PlatformGroup.Linux:
                    source = source.Append("LinuxAPI");
                    break;
                case PlatformGroup.OSX:
                    source = source.Append("OSXAPI");
                    break;
                default:
                    Console.Error.WriteLine("Console applications are not supported on this platform.");
                    throw TerminateException.User();
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

        var buildProfile = BuildProfileResolver.Resolve(targetProject, m_TargetInfo);
        var intDir = targetProject.Group.Intermediate(targetProject.Name, m_TargetInfo, buildProfile, FolderPolicy.PathType.Current);
        dependencyModuleNames.AddRange(rules.PublicDependencyModuleNames);
        includePaths.AddRange(rules.PublicIncludePaths.Select(p => AbsoluteIncludePath(targetProject, p)).Append(intDir));
        additionalMacros.AddRange(rules.PublicAdditionalMacros);
        disableWarnings.AddRange(rules.PublicDisableWarnings);
        additionalLibraries.AddRange(rules.PublicAdditionalLibraries);

        IEnumerable<string> deps = rules.PublicDependencyModuleNames;
        if (isPrimary)
        {
            deps = deps.Concat(WithBuiltInDependencyModule(rules.PrivateDependencyModuleNames));
            additionalMacros.Add(rules.SafeName.ToUpper() + "_API=PLATFORM_SHARED_EXPORT");
        }
        else
        {
            additionalMacros.Add(rules.SafeName.ToUpper() + "_API=PLATFORM_SHARED_IMPORT");
        }

        foreach (var dep in deps)
        {
            var dependTargetProject = solution.FindProject(dep);
            if (dependTargetProject == null)
            {
                Console.Error.WriteLine("Project {0}, referenced by Project {1}, is not included in this solution.", dep, targetProject.Name);
                throw TerminateException.User();
            }

            if (dependTargetProject is not ModuleProject mp)
            {
                Console.Error.WriteLine("Project {0}, referenced by Project {1}, is not a valid C++ project.", dep, targetProject.Name);
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

    public readonly ModuleProject Project;
    public readonly ModuleRules Rules;
    public readonly BuildConfigurationProfile BuildProfile;
    public string RuleFilePath => Project.RuleFilePath;
    public string Name => Project.Name;
    private readonly PchUsageMode m_PchUsage;
    private readonly string? m_PrivatePchHeaderFile;
    public PchUsageMode PchUsage => m_PchUsage;
    public string? PrivatePchHeaderFile => m_PrivatePchHeaderFile;
    public GroupDescriptor Group => Project.Group;
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
