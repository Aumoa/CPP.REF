using System.Data;
using Microsoft.CodeAnalysis;
using YamlDotNet.Serialization;

namespace AylaEngine;

internal class ModuleProject(Solution Solution, string name, GroupDescriptor descriptor, string sourceDirectory, Type ruleType, string ruleFilePath, ModuleProject.ModuleDeclaration declaration) : Project(name, descriptor, declaration)
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

    public bool IsScriptable()
    {
        return TargetInfo.GetAllTargets().Any(t => GetRule(t).Script.Enabled);
    }

    public bool IsExecutable()
    {
        return TargetInfo.GetAllTargets().Any(t => GetRule(t).Type == ModuleType.Application);
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
        List<string> blacklist =
        [
            Path.Combine(SourceDirectory, "Script")
        ];

        foreach (var source in Directory.GetFiles(SourceDirectory, "*", SearchOption.AllDirectories))
        {
            if (blacklist.Any(p => source.StartsWith(p, StringComparison.OrdinalIgnoreCase)))
            {
                continue;
            }

            if (SourceCodeDescriptor.TryGet(Group, Name, source, SourceDirectory, out var descriptor))
            {
                yield return descriptor;
            }
        }
    }

    public string ScriptSourceDirectory => Path.Combine(SourceDirectory, "Script");

    public string ScriptAssemblyName => Name + ".Script";

    public string ScriptProjectFileName => Path.Combine(ScriptSourceDirectory, ScriptAssemblyName + ".csproj");

    private CSProject? m_ScriptProject;

    public CSProject ScriptProject
    {
        get
        {
            if (m_ScriptProject == null)
            {
                List<CSPropertyGroup> propertyGroups = [];
                List<CSItemGroup> itemGroups = [];

                propertyGroups.Add(new CSPropertyGroup(
                    null,
                    OutputKind.DynamicallyLinkedLibrary,
                    CSTargetFramework.Net0900,
                    true,
                    NullableContextOptions.Enable,
                    ScriptAssemblyName,
                    Group.IsEngine ? "Ayla" : Group.Name,
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
                    [new CSUsing("Ayla.Object", "Object")],
                    [new CSRemoveItem("**\\*.meta")]
                    ));

                foreach (var targetInfo in TargetInfo.GetAllTargets())
                {
                    var outputPath = Group.Output(targetInfo, FolderPolicy.PathType.Current);
                    var optimized = targetInfo.Config.IsOptimized();
                    List<string> defines = ["$(DefineConstants)"];
                    if (targetInfo.Editor)
                    {
                        defines.Add("WITH_EDITOR");
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

                    var rule = GetRule(targetInfo);
                    var resolver = new ModuleRulesResolver(targetInfo, Solution, rule, Group);
                    var referencedProjects = resolver.DependencyModuleNames.Select(p =>
                    {
                        var dependProject = (ModuleProject)Solution.FindProject(p)!;
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

                m_ScriptProject = new CSProject("Microsoft.NET.Sdk", [.. propertyGroups], [.. itemGroups], null);
            }

            return m_ScriptProject;
        }
    }
}
