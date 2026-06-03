namespace AylaEngine;

internal sealed class ModuleRulesResolverFactory
{
    private readonly Solution m_Solution;
    private readonly Dictionary<(ModuleProject Project, ITargetInfo TargetInfo), ModuleRulesResolver> m_CachedResolvers = [];

    public ModuleRulesResolverFactory(Solution solution)
    {
        m_Solution = solution;
    }

    public ModuleRulesResolver GetResolver(ModuleProject project, ITargetInfo targetInfo)
    {
        var key = (project, targetInfo);
        lock (m_CachedResolvers)
        {
            if (m_CachedResolvers.TryGetValue(key, out var resolver))
            {
                return resolver;
            }
        }

        var createdResolver = new ModuleRulesResolver(targetInfo, m_Solution, project, project.GetRule(targetInfo));
        lock (m_CachedResolvers)
        {
            if (m_CachedResolvers.TryGetValue(key, out var resolver))
            {
                return resolver;
            }

            m_CachedResolvers.Add(key, createdResolver);
        }

        return createdResolver;
    }
}
