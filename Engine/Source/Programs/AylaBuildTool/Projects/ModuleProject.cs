namespace AylaEngine;

internal class ModuleProject(string name, GroupDescriptor descriptor, string sourceDirectory, Type ruleType, string ruleFilePath, Project.Declaration declaration) : Project(name, descriptor, declaration)
{
    public readonly Type RuleType = ruleType;
    public readonly string RuleFilePath = ruleFilePath;
    public readonly string SourceDirectory = sourceDirectory;

    private readonly Dictionary<ITargetInfo, ModuleRules> m_CachedRules = new();

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

    public IEnumerable<SourceCodeDescriptor> GetSourceCodes()
    {
        foreach (var source in Directory.GetFiles(SourceDirectory, "*", SearchOption.AllDirectories))
        {
            if (SourceCodeDescriptor.TryGet(source, SourceDirectory, out var descriptor))
            {
                yield return descriptor;
            }
        }
    }
}
