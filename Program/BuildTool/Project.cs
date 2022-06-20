// Copyright 2020-2022 Aumoa.lib. All right reserved.

using DotNETUtilities;

using System.Reflection;

namespace BuildTool;

/// <summary>
/// 네이티브 프로젝트 단위를 표현합니다.
/// </summary>
public class Project
{
    private readonly Assembly _assembly;

    /// <summary>
    /// 개체의 새 인스턴스를 초기화합니다.
    /// </summary>
    /// <param name="rule"> Solution.cs 파일의 레퍼런스를 전달합니다. </param>
    /// <param name="assembly"> 미리 컴파일 된 어셈블리를 전달합니다. </param>
    /// <param name="solutionDirectory"> 모듈이 포함 된 솔루션 디렉토리 레퍼런스를 전달합니다. </param>
    public Project(FileReference rule, Assembly? assembly, SolutionDirectoryReference solutionDirectory)
    {
        if (!rule.IsExist)
        {
            throw new FileNotFoundException($"{rule.FullPath} 파일을 찾을 수 없습니다.");
        }

        if (assembly == null)
        {
            // Compile assembly.
            _assembly = DynamicCompilation.CompileAssemblyAsync<ModuleRule>("SolutionRule", rule).Result;
        }
        else
        {
            _assembly = assembly;
        }

        // Find class that inherited from ModuleRule class.
        string projectRuleName = rule.Name.Split(".").First() + "Rule";
        Type ruleClass = _assembly.GetTypes().Where(p => p.IsDerivedFrom<ModuleRule>() && p.Name == projectRuleName).FirstOrDefault()
            ?? throw new RuleClassException(rule, typeof(ModuleRule));

        // Invoke constructor.
        object[] ctorArgs = Array.Empty<object>(); // Not yet required.
        Type[] ctorTypes = ctorArgs.Select(p => p.GetType()).ToArray();

        ConstructorInfo ctor = ruleClass.GetConstructor(ctorTypes)
            ?? throw new RuleClassException(rule, ruleClass, ctorTypes);
        try
        {
            Rule = (ModuleRule)ctor.Invoke(ctorArgs);
        }
        catch (Exception e)
        {
            throw new RuleClassException(rule, ruleClass, e);
        }

        Directory = rule.GetParent();
        SolutionDirectory = solutionDirectory;
    }

    /// <inheritdoc/>
    public override string ToString()
    {
        return Rule.ProjectName;
    }

    /// <summary>
    /// 모듈 규칙 클래스를 가져옵니다.
    /// </summary>
    public ModuleRule Rule { get; }

    /// <summary>
    /// 모듈 코드가 위치한 디렉토리를 가져옵니다.
    /// </summary>
    public DirectoryReference Directory { get; }

    /// <summary>
    /// 모듈의 솔루션 디렉토리를 가져옵니다.
    /// </summary>
    public SolutionDirectoryReference SolutionDirectory { get; }

    /// <summary>
    /// 프로젝트를 빌드합니다.
    /// </summary>
    /// <param name="solution"> 솔루션 정보를 전달합니다. </param>
    /// <param name="dependencyModules"> 의존 관계의 프로젝트 목록을 전달합니다. </param>
    public void BuildReferences(Solution solution, Project[] dependencyModules)
    {
        Solution = solution;
        DependencyModules = dependencyModules;

        List<string> includePaths = new();
        List<string> additionalMacros = new();
        List<int> disableWarnings = new();
        List<string> additionalLibraries = new();

        IEnumerable<string> ProcessIncludePaths(Project project, IEnumerable<string> paths)
        {
            return paths.Select(p => project.Directory.Move(p).FullPath);
        }

        foreach (var depend in dependencyModules)
        {
            includePaths.AddRange(ProcessIncludePaths(depend, depend.Rule.PublicIncludePaths));
            includePaths.Add(depend.IntermediateIncludePath);
            additionalMacros.AddRange(depend.Rule.PublicAdditionalMacros);
            disableWarnings.AddRange(depend.Rule.PublicDisableWarnings);
            additionalLibraries.AddRange(depend.Rule.PublicAdditionalLibraries);
        }

        IntermediateIncludePath = SolutionDirectory.Intermediate.Move("Include").Move(Rule.ProjectName).FullPath;
        includePaths.Add(IntermediateIncludePath);

        includePaths.AddRange(ProcessIncludePaths(this, Rule.PublicIncludePaths));
        includePaths.AddRange(ProcessIncludePaths(this, Rule.PrivateIncludePaths));
        IncludePaths = includePaths.Distinct().ToArray();

        additionalMacros.AddRange(Rule.PublicAdditionalMacros);
        additionalMacros.AddRange(Rule.PrivateAdditionalMacros);
        AdditionalMacros = additionalMacros.Distinct().ToArray();

        disableWarnings.AddRange(Rule.PublicDisableWarnings);
        disableWarnings.AddRange(Rule.PrivateDisableWarnings);
        DisableWarnings = disableWarnings.Distinct().ToArray();

        additionalLibraries.AddRange(Rule.PublicAdditionalLibraries);
        additionalLibraries.AddRange(Rule.PrivateAdditionalLibraries);
        AdditionalLibraries = additionalLibraries.Distinct().ToArray();
    }

    /// <summary>
    /// 이 프로젝트가 참조하는 솔루션을 가져옵니다.
    /// </summary>
    public Solution Solution { get; private set; } = null!;

    /// <summary>
    /// 이 프로젝트가 의존하는 모든 모듈의 목록을 가져옵니다.
    /// </summary>
    public Project[] DependencyModules { get; private set; } = null!;

    /// <summary>
    /// 이 프로젝트가 포함해야 하는 소스 코드의 위치 목록을 가져옵니다.
    /// </summary>
    public string[] IncludePaths { get; private set; } = null!;

    /// <summary>
    /// 이 프로젝트가 추가로 정의해야 하는 매크로 목록을 가져옵니다.
    /// </summary>
    public string[] AdditionalMacros { get; private set; } = null!;

    /// <summary>
    /// 이 프로젝트가 비활성화해야 하는 경고 번호 목록을 가져옵니다.
    /// </summary>
    public int[] DisableWarnings { get; private set; } = null!;

    /// <summary>
    /// 이 프로젝트가 추가로 포함해야 하는 라이브러리 목록을 가져옵니다.
    /// </summary>
    public string[] AdditionalLibraries { get; private set; } = null!;

    /// <summary>
    /// 이 프로젝트의 임시 포함 파일 위치를 가져옵니다.
    /// </summary>
    public string IntermediateIncludePath { get; private set; } = null!;

    /// <summary>
    /// API 매크로 목록을 생성합니다.
    /// </summary>
    /// <param name="generator"> 플랫폼 프로젝트 생성 인터페이스를 전달합니다. </param>
    /// <returns> 매크로 리스트가 반환됩니다. </returns>
    public List<string> GenerateAPIs(ISolutionGenerator generator)
    {
        List<string> macros = DependencyModules.Select(p => p.Rule).Select(p => generator.GenerateAPI(p, false)).ToList();
        macros.Add(generator.GenerateAPI(Rule, true));
        return macros;
    }
}
