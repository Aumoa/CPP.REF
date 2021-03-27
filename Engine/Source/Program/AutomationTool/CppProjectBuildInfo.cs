// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using System.Collections.Generic;

/// <summary>
/// C++ 프로젝트 빌드 정보를 나타냅니다.
/// </summary>
class CppProjectBuildInfo : ProjectBuildInfo
{
    /// <summary>
    /// 소스코드의 루트 디렉토리를 나타냅니다.
    /// </summary>
    public override DirectoryReference SourceRoot { get; set; }

    /// <summary>
    /// 프로젝트의 빌드 규칙입니다.
    /// </summary>
    public BuildRules Rules;

    /// <summary>
    /// 프로젝트의 심볼을 나타냅니다.
    /// </summary>
    public override string Symbol { get; set; }

    /// <summary>
    /// 의존 관계 구성이 완료된 비공개 의존 관계입니다.
    /// </summary>
    public List<string> ResolvedPrivateDependency;

    /// <summary>
    /// 의존 관계 구성이 완료된 공개 의존 관계입니다.
    /// </summary>
    public List<string> ResolvedPublicDependency;

    /// <summary>
    /// 프로젝트 파일 레퍼런스를 나타냅니다.
    /// </summary>
    public override FileReference ProjectFile { get; set; }

    /// <summary>
    /// 프로젝트 GUID를 나타냅니다. 
    /// </summary>
    public override Guid ProjectGuid { get; set; }

    /// <summary>
    /// 이미 생성되었는지 나타내는 값입니다.
    /// </summary>
    public bool IsGenerated
    {
        get
        {
            return ProjectFile is not null;
        }
    }

    /// <summary>
    /// 개체를 생성합니다.
    /// </summary>
    public CppProjectBuildInfo()
    {

    }
}