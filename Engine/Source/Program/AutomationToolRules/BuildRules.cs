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

    /// <summary>
    /// 추가로 지정할 전처리기 지정자입니다.
    /// </summary>
    public List<string> AdditionalPreprocessorDefines = new();

    /// <summary>
    /// 추가로 지정할 동적 라이브러리 링크 목록입니다.
    /// </summary>
    public List<string> AdditionalDependencies = new();

    /// <summary>
    /// PCH 헤더 파일 여부를 설정합니다. 확장자를 제외한 파일 이름을 결정합니다.
    /// </summary>
    public string PrivatePCHHeaderFile = null;

    /// <summary>
    /// C++ 컴파일 버전을 선택합니다.
    /// </summary>
    public CppVersion CppStandardVersion = CppVersion.Cpp14;
}