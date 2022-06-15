// Copyright 2017 CLOVERGAMES Co., Ltd. All right reserved.

using DotNETUtilities;

using System.Reflection;

namespace BuildTool;

/// <summary>
/// 네이티브 솔루션 단위를 표현합니다.
/// </summary>
public class Solution
{
    private readonly Assembly _assembly;

    /// <summary>
    /// 개체의 새 인스턴스를 초기화합니다.
    /// </summary>
    /// <param name="rule"> Solution.cs 파일의 레퍼런스를 전달합니다. </param>
    public Solution(FileReference rule)
    {
        SolutionDirectoryReference? GenerateDirectory(string? path)
        {
            if (!string.IsNullOrEmpty(path))
            {
                if (Path.IsPathFullyQualified(path))
                {
                    return new SolutionDirectoryReference(path);
                }
                else
                {
                    return new SolutionDirectoryReference(rule.GetParent().Move(path));
                }
            }

            return null;
        }

        Rule = CompileSolutionRule(rule, out _assembly);

        Directory = GenerateDirectory(Rule.GameRoot)!;
        EngineDirectory = GenerateDirectory(Rule.EngineRoot);
        ThirdpartyDirectory = GenerateDirectory(Rule.ThirdpartyRoot);

        if (!string.IsNullOrEmpty(Rule.ProgramsRoot))
        {
            if (Path.IsPathFullyQualified(Rule.ProgramsRoot))
            {
                ProgramsDirectory = new DirectoryReference(Rule.ProgramsRoot);
            }
            else
            {
                ProgramsDirectory = rule.GetParent().Move(Rule.ProgramsRoot);
            }
        }

        Projects = SearchAndCompileProjects(Directory)
            .Concat(SearchAndCompileProjects(EngineDirectory))
            .Concat(SearchAndCompileProjects(ThirdpartyDirectory))
            .ToArray();

        BuildReferences();
    }

    private static IEnumerable<Project> SearchAndCompileProjects(SolutionDirectoryReference? from)
    {
        if (from is null)
        {
            return Enumerable.Empty<Project>();
        }

        if (!from.IsExist || !from.Source.IsExist)
        {
            return Enumerable.Empty<Project>();
        }

        List<FileReference> submodules = new();
        foreach (var file in from.Source.GetAllFiles())
        {
            string moduleName = file.GetParent().Name;
            if (file.FileName.Equals($"{moduleName}.Module.cs", StringComparison.OrdinalIgnoreCase))
            {
                submodules.Add(file);
            }
        }

        Assembly projectsRuleAssembly = DynamicCompilation.CompileAssemblyAsync(
            from.Name,
            submodules,
            new List<FileReference>
            {
                typeof(ModuleRule).AssemblyLocation()
            }
            ).Result;

        return submodules.Select(p => new Project(p, projectsRuleAssembly, from));
    }

    /// <inheritdoc/>
    public override string ToString()
    {
        return Rule.SolutionName;
    }

    private SolutionRule CompileSolutionRule(FileReference rule, out Assembly assembly)
    {
        if (!rule.IsExist)
        {
            throw new FileNotFoundException($"{rule.FullPath} 파일을 찾을 수 없습니다.");
        }

        // Compile assembly.
        assembly = DynamicCompilation.CompileAssemblyAsync<SolutionRule>("SolutionRule", rule).Result;

        // Find class that inherited from SolutionRule class.
        string solutionRuleName = rule.Name.Split(".").First() + "Rule";
        Type ruleClass = _assembly.GetTypes().Where(p => p.IsDerivedFrom<SolutionRule>() && p.Name == solutionRuleName).FirstOrDefault()
            ?? throw new RuleClassException(rule, typeof(SolutionRule));

        // Invoke constructor.
        object[] ctorArgs = Array.Empty<object>(); // Not yet required.
        Type[] ctorTypes = ctorArgs.Select(p => p.GetType()).ToArray();

        ConstructorInfo ctor = ruleClass.GetConstructor(ctorTypes)
            ?? throw new RuleClassException(rule, ruleClass, ctorTypes);
        try
        {
            return (SolutionRule)ctor.Invoke(ctorArgs);
        }
        catch (Exception e)
        {
            throw new RuleClassException(rule, ruleClass, e);
        }
    }

    /// <summary>
    /// 솔루션 규칙 클래스를 가져옵니다.
    /// </summary>
    public SolutionRule Rule { get; }

    /// <summary>
    /// 솔루션 코드가 위치한 디렉토리를 가져옵니다.
    /// </summary>
    public SolutionDirectoryReference Directory { get; }

    /// <summary>
    /// 엔진 코드가 위치한 디렉토리를 가져옵니다.
    /// </summary>
    public SolutionDirectoryReference? EngineDirectory { get; }

    /// <summary>
    /// 서드파티 코드가 위치한 디렉토리를 가져옵니다.
    /// </summary>
    public SolutionDirectoryReference? ThirdpartyDirectory { get; }

    /// <summary>
    /// 도구 프로그램의 루트 디렉토리를 가져옵니다.
    /// </summary>
    public DirectoryReference? ProgramsDirectory { get; }

    /// <summary>
    /// 이 솔루션에 포함되는 프로젝트 목록을 가져옵니다.
    /// </summary>
    public Project[] Projects { get; private set; }

    /// <summary>
    /// 생성된 솔루션과 포함 프로젝트 목록을 전부 빌드합니다.
    /// </summary>
    public void BuildReferences()
    {
        List<Project> priorityList = new();
        HashSet<Project> projectsIncludeSet = new();
        Dictionary<Project, Project[]> dependenciesMap = new();
        foreach (var project in Projects)
        {
            ComputeDependencyRecursive(project, projectsIncludeSet, priorityList, dependenciesMap);
        }

        foreach (var project in priorityList)
        {
            project.BuildReferences(this, dependenciesMap[project]);
        }

        // Exchange as priority sorted array.
        Projects = priorityList.ToArray();
    }

    private void ComputeDependencyRecursive(Project projectTarget, HashSet<Project> projectsIncludeSet, List<Project> priorityList, Dictionary<Project, Project[]> dependenciesMap)
    {
        if (projectsIncludeSet.Contains(projectTarget))
        {
            return;
        }

        List<string> dependencies = new();
        dependencies.AddRange(projectTarget.Rule.PublicDependencyModuleNames);
        dependencies.AddRange(projectTarget.Rule.PrivateDependencyModuleNames);

        List<Project> dependProjects = new();

        foreach (var depend in dependencies)
        {
            int indexOf = Projects.IndexOf(p => p.Rule.ProjectName == depend);
            if (indexOf == -1)
            {
                throw new KeyNotFoundException($"프로젝트 {projectTarget.Rule.ProjectName}의 의존 모듈 {depend}을 찾는 중 오류가 발생했습니다. 대상 모듈을 찾을 수 없습니다.");
            }

            Project dependProject = Projects[indexOf];
            if (!projectsIncludeSet.Contains(dependProject))
            {
                ComputeDependencyRecursive(dependProject, projectsIncludeSet, priorityList, dependenciesMap);
            }

            dependProjects.AddRange(dependenciesMap[dependProject]);
        }

        dependProjects.AddRange(dependencies.Select(p =>
        {
            int indexOf = Projects.IndexOf(n => n.Rule.ProjectName == p);
            return Projects[indexOf];
        }));

        dependenciesMap.Add(projectTarget, dependProjects.Distinct().ToArray());
        projectsIncludeSet.Add(projectTarget);
        priorityList.Add(projectTarget);
    }
}
