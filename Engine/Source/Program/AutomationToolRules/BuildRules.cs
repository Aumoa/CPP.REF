// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System.Collections.Generic;

/// <summary>
/// 프로젝트의 빌드 규칙입니다.
/// </summary>
public class BuildRules
{
    /// <summary>
    /// 파생된 프로젝트에게 전파되는 공개 의존 프로젝트 목록입니다.
    /// </summary>
    public List<string> PublicDependencyModuleNames = new();

    /// <summary>
    /// 내부에서만 사용되는 비공개 의존 프로젝트 목록입니다.
    /// </summary>
    public List<string> PrivateDependencyModuleNames = new();
}