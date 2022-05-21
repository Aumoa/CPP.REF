// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace BuildTool;

/// <summary>
/// 솔루션 정의 규칙을 표현합니다.
/// </summary>
public class SolutionRule
{
    /// <summary>
    /// 개체의 새 인스턴스를 초기화합니다.
    /// </summary>
    public SolutionRule()
    {
        SolutionName = GetType().Name;
        if (SolutionName.EndsWith("Rule"))
        {
            SolutionName = SolutionName[..^4];
        }
        GameRoot = ".";
    }

    /// <summary>
    /// 솔루션의 이름을 나타냅니다. 일반적으로 큰 의미를 가지지 않습니다.
    /// </summary>
    public string SolutionName { get; protected set; }

    /// <summary>
    /// 이 솔루션의 모든 모듈이 Static 라이브러리로 링크됩니다.
    /// </summary>
    public bool StaticLibraries { get; protected set; }

    /// <summary>
    /// 게임 루트 디렉토리를 지정합니다.
    /// </summary>
    public string GameRoot { get; protected set; }

    /// <summary>
    /// 엔진 루트 디렉토리를 지정합니다.
    /// </summary>
    public string? EngineRoot { get; protected set; }

    /// <summary>
    /// 서드파티 루트 디렉토리를 지정합니다.
    /// </summary>
    public string? ThirdpartyRoot { get; protected set; }

    /// <summary>
    /// 도구 프로그램의 루트 디렉토리를 지정합니다.
    /// </summary>
    public string? ProgramsRoot { get; protected set; }

    /// <summary>
    /// 표준 엔진 경로를 사용해 <see cref="EngineRoot"/>, <see cref="ThirdpartyRoot"/> 및 <see cref="ProgramsRoot"/> 프로퍼티를 채웁니다.
    /// </summary>
    /// <param name="path"> 경로를 전달합니다. </param>
    protected void GenerateEngine(string path)
    {
        EngineRoot = Path.Combine(path, "Engine");
        ThirdpartyRoot = Path.Combine(path, "Thirdparty");
        ProgramsRoot = Path.Combine(path, "Program");
    }
}
