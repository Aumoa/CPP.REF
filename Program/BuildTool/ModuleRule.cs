// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace BuildTool;

/// <summary>
/// 모듈의 규칙을 표현합니다.
/// </summary>
public class ModuleRule
{
    /// <summary>
    /// 개체의 새 인스턴스를 초기화합니다.
    /// </summary>
    public ModuleRule()
    {
        ProjectName = GetType().Name;
        if (ProjectName.EndsWith("Rule"))
        {
            ProjectName = ProjectName[..^4];
        }
    }

    /// <summary>
    /// 모듈 프로젝트 이름을 가져옵니다.
    /// </summary>
    public string ProjectName { get; protected set; }

    /// <summary>
    /// C++ Syntax 요소가 제거된 이름을 가져옵니다.
    /// </summary>
    public string SafeProjectName { get => ProjectName.Replace(".", "_"); }

    /// <summary>
    /// 프로젝트 타깃 이름을 가져옵니다.
    /// </summary>
    public string? TargetName { get; protected set; }

    /// <summary>
    /// 프로젝트의 필터 경로를 가져옵니다.
    /// </summary>
    public string? FilterPath { get; protected set; }

    /// <summary>
    /// 모듈의 범주를 지정합니다.
    /// </summary>
    public ModuleCategory Category { get; protected set; } = ModuleCategory.Game;

    /// <summary>
    /// 모듈의 형식을 지정합니다.
    /// </summary>
    public ModuleType ModuleType { get; protected set; } = ModuleType.Library;

    /// <summary>
    /// 소스 코드를 나타내는 확장자 목록을 지정합니다.
    /// </summary>
    public List<string> SourceCodeExtensions { get; protected set; } = new() { ".cpp", ".cxx", ".c", ".ixx" };

    /// <summary>
    /// 헤더를 나타내는 확장자 목록을 지정합니다.
    /// </summary>
    public List<string> HeaderExtensions { get; protected set; } = new() { ".h" };

    /// <summary>
    /// 빌드에 포함되지 않는 확장자 목록을 지정합니다.
    /// </summary>
    public List<string> NonBuildExtensions { get; protected set; } = new() { ".inl", ".cs" };

    /// <summary>
    /// Vcpkg 지원 여부를 지정합니다.
    /// </summary>
    public bool SupportsVcpkg { get; protected set; } = true;

    /// <summary>
    /// UnityBuild를 사용하지 않도록 지정합니다.
    /// </summary>
    public bool NonUnityBuild { get; protected set; } = true;

    /// <summary>
    /// 프로젝트가 사용할 PCH 헤더 파일의 이름을 지정합니다.
    /// </summary>
    public string? PrivatePCHHeader { get; protected set; }

    /// <summary>
    /// 공용 의존 모듈 이름 목록을 지정합니다.
    /// </summary>
    public List<string> PublicDependencyModuleNames { get; protected set; } = new();

    /// <summary>
    /// 개인 의존 모듈 이름 목록을 지정합니다.
    /// </summary>
    public List<string> PrivateDependencyModuleNames { get; protected set; } = new();

    /// <summary>
    /// 공용 포함 패스 목록을 지정합니다. 이 모듈을 의존하는 모든 프로젝트에 적용됩니다.
    /// </summary>
    public List<string> PublicIncludePaths { get; protected set; } = new();

    /// <summary>
    /// 개인 포함 패스 목록을 지정합니다. 이 프로젝트에만 적용됩니다.
    /// </summary>
    public List<string> PrivateIncludePaths { get; protected set; } = new();

    /// <summary>
    /// 공용 추가 정의 매크로 목록을 지정합니다. 이 모듈을 의존하는 모든 프로젝트에 적용됩니다.
    /// </summary>
    public List<string> PublicAdditionalMacros { get; protected set; } = new();

    /// <summary>
    /// 개인 추가 정의 매크로 목록을 지정합니다. 이 프로젝트에만 적용됩니다.
    /// </summary>
    public List<string> PrivateAdditionalMacros { get; protected set; } = new();

    /// <summary>
    /// 공용 비활성 경고 번호 목록을 지정합니다. 이 모듈을 의존하는 모든 프로젝트에 적용됩니다.
    /// </summary>
    public List<int> PublicDisableWarnings { get; protected set; } = new();

    /// <summary>
    /// 개인 비활성 경고 번호 목록을 지정합니다. 이 프로젝트에만 적용됩니다.
    /// </summary>
    public List<int> PrivateDisableWarnings { get; protected set; } = new();

    /// <summary>
    /// 공용 추가 라이브러리 목록을 지정합니다. 이 모듈을 의존하는 모든 프로젝트에 적용됩니다.
    /// </summary>
    public List<string> PublicAdditionalLibraries { get; protected set; } = new();

    /// <summary>
    /// 개인 추가 라이브러리 목록을 지정합니다. 이 프로젝트에만 적용됩니다.
    /// </summary>
    public List<string> PrivateAdditionalLibraries { get; protected set; } = new();
}
