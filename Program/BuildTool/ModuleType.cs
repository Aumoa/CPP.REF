// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace BuildTool;

/// <summary>
/// 모듈의 형식을 나타냅니다.
/// </summary>
public enum ModuleType
{
    /// <summary>
    /// 빌드 대상에 포함되지 않는 모듈 형식을 나타냅니다.
    /// </summary>
    None,

    /// <summary>
    /// 실행 가능한 모듈 형식을 나타냅니다.
    /// </summary>
    Application,

    /// <summary>
    /// 콘솔 창과 함께 실행되는 모듈 형식을 나타냅니다.
    /// </summary>
    ConsoleApplication,

    /// <summary>
    /// 다른 모듈에 필요한 기능을 제공합니다.
    /// </summary>
    Library,
}
