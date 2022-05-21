// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace BuildTool;

/// <summary>
/// CMake 프로젝트를 생성합니다.
/// </summary>
public class CMakeGenerator : ISolutionGenerator
{
    /// <summary>
    /// 개체의 새 인스턴스를 초기화합니다.
    /// </summary>
    public CMakeGenerator(Solution solution)
    {
    }

    /// <inheritdoc/>
    public void Generate()
    {
    }

    /// <inheritdoc/>
    public string GenerateAPI(ModuleRule rule, bool isExport)
    {
        return "";
    }
}
