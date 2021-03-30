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

    /// <summary>
    /// 이 모듈에서 컴파일 대상으로 포함 시킬 코드 파일의 확장자를 설정합니다.
    /// </summary>
    public List<string> ExtensionNamesForCompile = new()
    {
        ".cpp",
        ".c"
    };

    /// <summary>
    /// 이 모듈에서 FX 컴파일 대상으로 포함 시킬 코드 파일의 확장자를 설정합니다.
    /// FX 파일은 파일 이름.형식.확장자로 구성되며, 형식은 VertexShader, PixelShader, ShaderLibrary 등입니다.
    /// </summary>
    public List<string> ExtensionNamesForFxCompile = new()
    {
        ".hlsl"
    };

    /// <summary>
    /// 소스 코드 빌드를 사용하지 않고 바이너리를 즉시 링크합니다.
    /// </summary>
    public bool bDirectLink = false;

    /// <summary>
    /// 이 프로젝트는 프로그램을 실행하는 데 사용됩니다.
    /// </summary>
    public bool bLaunch = false;

    /// <summary>
    /// 빌드 규칙이 로드된 후 호출되는 함수입니다.
    /// </summary>
    public void PostProcess()
    {
        for (int i = 0; i < ExtensionNamesForCompile.Count; ++i)
        {
            // 모든 확장자를 소문자로 만듭니다.
            ExtensionNamesForCompile[i] = ExtensionNamesForCompile[i].ToLower();

            // .이 붙어있지 않을 경우 .을 붙입니다.
            if (ExtensionNamesForCompile[i][0] is not '.')
            {
                ExtensionNamesForCompile[i] = $".{ExtensionNamesForCompile[i]}";
            }
        }
    }
}