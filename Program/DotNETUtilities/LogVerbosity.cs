// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace DotNETUtilities;

/// <summary>
/// 로그 출력 레벨을 나타냅니다.
/// </summary>
public enum LogVerbosity
{
    /// <summary>
    /// 디버그성 로그를 나타냅니다.
    /// </summary>
    Trace = 0,

    /// <summary>
    /// 프로그램 실행에 대한 정보를 표현하는 로그를 나타냅니다.
    /// </summary>
    Info = 1,

    /// <summary>
    /// 프로그램 실행 중 발생한 경고 사항에 대한 로그를 나타냅니다.
    /// </summary>
    Warning = 2,

    /// <summary>
    /// 프로그램 실행 중 발생한 복구 가능한 오류 사항에 대한 로그를 나타냅니다.
    /// </summary>
    Error = 3,

    /// <summary>
    /// 프로그램 실행 중 발생한 복구 불가능한 오류 사항에 대한 로그를 나타냅니다.
    /// </summary>
    Critical = 4,
}
