// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace BuildTool;

/// <summary>
/// BuildTool이 지원하는 플랫폼 목록에 대한 바인딩을 제공합니다.
/// </summary>
public static class PlatformStatics
{
    /// <summary>
    /// 지원 플랫폼 목록입니다.
    /// </summary>
    public static readonly string[] Platforms = new string[] { "Windows", "Linux" };

    /// <summary>
    /// 지원 플랫폼 목록의 매크로 이름입니다.
    /// </summary>
    public static readonly string[] Platforms_Macro = Platforms.Select(p => "PLATFORM_" + p.ToUpper()).ToArray();
}
