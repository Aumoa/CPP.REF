// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace BuildTool;

/// <summary>
/// 모듈의 범주를 나타냅니다.
/// </summary>
public enum ModuleCategory
{
    /// <summary>
    /// 게임 프로젝트를 나타냅니다. 디버그 상태에 가깝습니다.
    /// </summary>
    Game,

    /// <summary>
    /// 엔진 프로젝트를 나타냅니다. 배포 상태에 가깝습니다.
    /// </summary>
    Engine,

    /// <summary>
    /// 서드파티 프로젝트를 나타냅니다. 항상 배포 상태입니다.
    /// </summary>
    ThirdParty
}
