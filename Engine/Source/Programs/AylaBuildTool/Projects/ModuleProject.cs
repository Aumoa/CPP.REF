using YamlDotNet.Serialization;

namespace AylaEngine;

internal class ModuleProject(Solution solution, string name, GroupDescriptor descriptor, string sourceDirectory, Type ruleType, string ruleFilePath, ModuleProject.ModuleDeclaration declaration) : Project(name, descriptor, declaration)
{
    public record ModuleDeclaration : Project.Declaration
    {
        public Guid ScriptGuid { get; init; } = default;
        public Guid BindingGuid { get; init; } = default;

        [YamlIgnore]
        public override bool IsValid => base.IsValid && ScriptGuid != default && BindingGuid != default;

        public static new ModuleDeclaration New() => new ModuleDeclaration
        {
            Guid = Guid.NewGuid(),
            ScriptGuid = Guid.NewGuid(),
            BindingGuid = Guid.NewGuid()
        };
    }

    public readonly Solution Solution = solution;
    public readonly Type RuleType = ruleType;
    public readonly string RuleFilePath = ruleFilePath;
    public readonly string SourceDirectory = sourceDirectory;
    public readonly new ModuleDeclaration Decl = declaration;

    private readonly Dictionary<ITargetInfo, ModuleRules> m_CachedRules = new();
    private readonly Dictionary<ITargetInfo, ModuleRulesResolver> m_CachedResolvers = new();

    public ModuleRules GetRule(ITargetInfo targetInfo)
    {
        ModuleRules? rules;

        lock (m_CachedRules)
        {
            if (m_CachedRules.TryGetValue(targetInfo, out rules))
            {
                return rules;
            }
        }

        rules = ModuleRules.New(RuleType, targetInfo);
        lock (m_CachedRules)
        {
            if (m_CachedRules.TryGetValue(targetInfo, out var @int))
            {
                return @int;
            }

            m_CachedRules.Add(targetInfo, rules);
        }

        return rules;
    }

    public ModuleRulesResolver GetResolver(ITargetInfo targetInfo)
    {
        ModuleRulesResolver? resolver;
        lock (m_CachedResolvers)
        {
            if (m_CachedResolvers.TryGetValue(targetInfo, out resolver))
            {
                return resolver;
            }
        }

        resolver = new ModuleRulesResolver(targetInfo, Solution, GetRule(targetInfo), Group);
        lock (m_CachedResolvers)
        {
            if (m_CachedResolvers.TryGetValue(targetInfo, out var @int))
            {
                return @int;
            }
            m_CachedResolvers.Add(targetInfo, resolver);
        }

        return resolver;
    }

    public IEnumerable<SourceCodeDescriptor> GetSourceCodes()
    {
        foreach (var source in Directory.GetFiles(SourceDirectory, "*", SearchOption.AllDirectories))
        {
            if (SourceCodeDescriptor.TryGet(Group, Name, source, SourceDirectory, out var descriptor))
            {
                yield return descriptor;
            }
        }
    }

    public string GetScriptProjectName()
    {
        return Path.Combine(SourceDirectory, "Script", Name + ".Script.csproj");
    }
}
