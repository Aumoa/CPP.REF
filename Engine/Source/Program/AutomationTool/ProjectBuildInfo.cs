// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;

/// <summary>
/// 프로젝트 빌드 정보를 나타냅니다.
/// </summary>
abstract class ProjectBuildInfo
{
    /// <summary>
    /// 소스코드의 루트 디렉토리를 나타냅니다.
    /// </summary>
    public abstract DirectoryReference SourceRoot { get; set; }

    /// <summary>
    /// 프로젝트의 심볼을 나타냅니다.
    /// </summary>
    public abstract string Symbol { get; set; }

    /// <summary>
    /// 프로젝트 파일 레퍼런스를 나타냅니다.
    /// </summary>
    public abstract FileReference ProjectFile { get; set; }

    /// <summary>
    /// 프로젝트 GUID를 나타냅니다. 
    /// </summary>
    public abstract Guid ProjectGuid { get; set; }
}