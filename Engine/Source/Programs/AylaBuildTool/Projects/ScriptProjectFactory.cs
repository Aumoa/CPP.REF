using Microsoft.CodeAnalysis;

namespace AylaEngine;

internal sealed class ScriptProjectFactory
{
    private readonly Solution m_Solution;
    private readonly ModuleRulesResolverFactory m_ResolverFactory;
    private readonly Dictionary<ModuleProject, CSProject> m_CachedProjects = [];

    public ScriptProjectFactory(Solution solution, ModuleRulesResolverFactory resolverFactory)
    {
        m_Solution = solution;
        m_ResolverFactory = resolverFactory;
    }

    public CSProject GetScriptProject(ModuleProject project)
    {
        lock (m_CachedProjects)
        {
            if (m_CachedProjects.TryGetValue(project, out var scriptProject))
            {
                return scriptProject;
            }
        }

        var createdProject = CreateScriptProject(project);
        lock (m_CachedProjects)
        {
            if (m_CachedProjects.TryGetValue(project, out var scriptProject))
            {
                return scriptProject;
            }

            m_CachedProjects.Add(project, createdProject);
        }

        return createdProject;
    }

    private CSProject CreateScriptProject(ModuleProject project)
    {
        List<CSPropertyGroup> propertyGroups = [];
        List<CSItemGroup> itemGroups = [];

        propertyGroups.Add(new CSPropertyGroup(
            null,
            OutputKind.DynamicallyLinkedLibrary,
            CSTargetFramework.Net0900,
            true,
            NullableContextOptions.Enable,
            project.ScriptAssemblyName,
            project.Group.IsEngine ? "Ayla" : project.Group.Name,
            true,
            true,
            false,
            false,
            false,
            TargetInfo.GetAllTargets().Select(t => VSUtility.GetConfigName(t)).Distinct().ToArray(),
            TargetInfo.GetAllTargets().Select(t => t.Platform.Name).Distinct().ToArray(),
            null,
            null,
            [],
            null
        ));

        itemGroups.Add(new CSItemGroup(
            null,
            [],
            [new CSUsing("Ayla.Object", "Object"), new CSUsing("Ayla.Debug", "Debug")],
            [new CSRemoveItem("**\\*.meta")]
            ));

        foreach (var targetInfo in TargetInfo.GetAllTargets())
        {
            var outputPath = project.Group.Output(targetInfo, FolderPolicy.PathType.Current);
            var optimized = targetInfo.Config.IsOptimized();
            List<string> defines = [];
            if (targetInfo.Editor)
            {
                defines.Add("WITH_EDITOR");
            }
            if (targetInfo.Config != Configuration.Shipping)
            {
                defines.Add("DO_CHECK");
            }

            var condition = CSCondition.Parse($"$(Configuration)|$(Platform)'=='{VSUtility.GetConfigName(targetInfo)}|{targetInfo.Platform.Name}");

            propertyGroups.Add(new CSPropertyGroup(
                condition,
                null,
                null,
                null,
                null,
                null,
                null,
                null,
                null,
                null,
                null,
                null,
                [],
                [],
                outputPath,
                optimized,
                [.. defines],
                VSUtility.GetArchitectureName(targetInfo.Platform.Architecture)
                ));

            var resolver = m_ResolverFactory.GetResolver(project, targetInfo);
            var referencedProjects = resolver.DependencyModuleNames.Select(p =>
            {
                var dependProject = (ModuleProject)m_Solution.FindProject(p)!;
                if (dependProject.GetRule(targetInfo).Script.Enabled)
                {
                    return new CSProjectReference(dependProject.ScriptProjectFileName, false);
                }
                else
                {
                    return null!;
                }
            }).Where(p => p != null);

            itemGroups.Add(new CSItemGroup(
                condition,
                [.. referencedProjects],
                [],
                []
                ));
        }

        return new CSProject("Microsoft.NET.Sdk", [.. propertyGroups], [.. itemGroups], null);
    }
}
