// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace BuildTool;

/// <summary>
/// <see cref="Solution"/> 개체에서 생성된 정보를 기반으로 각 플랫폼에 맞는 프로젝트를 생성합니다.
/// </summary>
public interface ISolutionGenerator
{
    /// <summary>
    /// 프로젝트 및 솔루션 파일을 생성합니다.
    /// </summary>
    void Generate();

    /// <summary>
    /// API 매크로를 생성합니다.
    /// </summary>
    /// <param name="rule"> 모듈 규칙을 전달합니다. </param>
    /// <param name="isExport"> 이 모듈이 export 대상인지 나타내는 값을 전달합니다. </param>
    /// <returns> 생성된 매크로가 반환됩니다. </returns>
    string GenerateAPI(ModuleRule rule, bool isExport);
}
