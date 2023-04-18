// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Locales.Korean;

internal class Errors : IErrors
{
    public string AssemblyLocationNotFound => "어셈블리 경로를 찾지 못했습니다.";

    public string InvalidEngineDirectory => "올바르지 않은 엔진 디렉토리입니다.";

    public string TargetRuleConstructorNotFound => "`TargetRules` 클래스를 초기화하기 위한 생성자를 `{0}` 클래스에서 찾을 수 없습니다.";

    public string ModuleRuleConstructorNotFound => "`ModuleRules` 클래스를 초기화하기 위한 생성자를 `{0}` 클래스에서 찾을 수 없습니다.";

    public string DependencyModuleNotFound => "`{0}` 모듈에서 필요한 모듈 `{1}`을 찾을 수 없습니다.";

    public string DuplicatedModuleDefinitions => "두 개 이상의 모듈 정의 파일(`*.Module.cs`)이 검색되었습니다.";

    public string InvalidProjectFormat => "올바르지 않은 프로젝트 형식입니다.";

    public string StartCMakeError => "cmake.exe 프로세스를 시작할 수 없습니다.";

    public string CMakeReturnCodeError => "cmake.exe에서 성공이 아닌 코드({0})를 반환했습니다.";

    public string MultipleModuleNotSuportedError => "두 개 이상의 모듈이 허용되지 않는 타깃 유형입니다.";
    
    public string NotSupportedBuildHostPlatform => "지원하지 않는 빌드 호스트 플랫폼입니다.";

    public string CompilerNotSupported => "지원되지 않는 컴파일러 버전입니다.";

    public string InvalidToolChainInstallation => "올바르지 않은 ToolChain 설치입니다.";

    public string TargetNotFoundException => "지정한 빌드 타깃({0})을 찾을 수 없습니다.";

    public string NotSupportedArchitecture => "지원되지 않는 아키텍처 버전({0})입니다.";

    public string NullReferenceInRule => "규칙 파일을 로드하는 중 null 레퍼런스를 발견했습니다. 프로퍼티 이름: {0}, 소유 형식: {1}";

    public string NotSupportedType => "규칙 파일에 지원하지 않는 프로퍼티가 포함되어 있습니다. 프로퍼티 이름: {0}, 소유 형식: {1}";

    public string ModuleRuleDuplicated => "디렉토리 {0}에 모듈 규칙 파일(*.Module.cs)이 디렉토리에 두 개 이상 포함되어 있습니다.";

    public string ModuleRuleNotFound => "디렉토리 {0}에서 필요한 모듈 규칙 파일(*.Module.cs)을 찾을 수 없습니다.";

    public string TargetRuleNotFound => "디렉토리 {0}에서 필요한 타깃 규칙 파일({1})을 찾을 수 없습니다.";

    public string RuleClassDefinitions => "{0} 규칙 정의에 호환 가능한 클래스가 없거나 두 개 이상의 호환 가능한 클래스가 있습니다.";

    public string TargetRuleDuplicated => "일치하는 이름의 타깃 규칙 파일(*.Target.cs)이 두 개 이상 포함되어 있습니다.";

    public string InvalidModuleFormat => "{0} 파일의 의존 관계를 구성하는 중 오류가 발생했습니다. 올바르지 않은 모듈 소스 코드 형식입니다.";
}
