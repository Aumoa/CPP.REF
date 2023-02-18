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
}
