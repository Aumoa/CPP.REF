// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace DotNETUtilities;

/// <summary>
/// 로그 출력 정보를 빌드합니다.
/// </summary>
public struct LogEntry
{
    /// <summary>
    /// 로그 카테고리를 나타냅니다.
    /// </summary>
    public LogCategoryAttribute? Category { get; set; }

    /// <summary>
    /// 로그 수준을 나타냅니다.
    /// </summary>
    public LogVerbosity? Verbosity { get; set; }

    /// <summary>
    /// 로그 출력 메시지를 나타냅니다.
    /// </summary>
    public string? Message { get; set; }

    /// <summary>
    /// 로그 모듈에서 실행할 로그 액션을 나타냅니다.
    /// </summary>
    public Action? LogAction { get; set; }
}
