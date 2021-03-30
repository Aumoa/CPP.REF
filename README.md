# C++.REF 프레임워크를 이용한 간단한 게임 엔진 만들기
- C++.REF는 별거 없고... C#과 비슷하게 프로그래밍 해보자!라는 취지로 만들어진 프레임워크입니다.
- 게임 엔진의 형태는 거의 언리얼 엔진의 것을 띄고 있습니다. 아무래도 가장 효율적으로 만들 수 있는 방식인 것 같아서요.
- 이 프로젝트는 상업을 목적으로 이용되지 않으며, 그 누구도 상업적으로 이용할 수 없습니다.

# 프로젝트 빌드 요구 사항
- Visual Studio 2019 (C++ 포함) (C++20 기능을 사용합니다.)

# 프로젝트 파일 생성/빌드
- 엔진 코드를 받습니다.
- 게임 프로젝트가 위치할 적당한 위치로 이동합니다. 엔진 코드는 옮기지 않아도 됩니다.
- 프로젝트폴더/Source/모듈이름/모듈이름.Build.cs 파일을 다음과 같이 작성합니다.
```csharp
public class SampleGameBuild : BuildRules
{
    public SampleGameBuild()
    {
        CppStandardVersion = CppVersion.Cpp20;
        bLaunch = true;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "Game",
        });
    }
}
```
- 프로젝트 폴더로 돌아가, 배치 파일을 작성합니다.
```
@echo off

echo -
echo ---------- Generate Game Projects ----------
echo -

SET EngineRoot=여기에 엔진 디렉토리를 입력하세요.
SET ProjectName=여기에 프로젝트 이름을 입력하세요.

%EngineRoot%\Binaries\AutomationTool\AutomationTool.exe GenerateGameSolution %EngineRoot% . %ProjectName%

pause
```
- 배치 파일을 실행합니다.
- 프로젝트 폴더에 생성된 %ProjectName%.sln 파일을 실행하여 빌드합니다.

# 코드 작성 규칙
- 기본적으로, 특수한 기능을 수행하지 않는 모든 일반적인 클래스는 Object 클래스를 상속 받습니다. 다음 규칙을 갖습니다.
1. Object 클래스를 상속받을 때는 반드시 virtual public으로 상속받는다.
2. 인터페이스 역시 동일하게 이 규칙을 적용한다.
3. Object가 아닌 클래스는 virtual 상속으로 받지 않으며, 오직 하나의 클래스만 상속받을 수 있다.
4. 인터페이스를 상속 받을 땐 virtual public으로 상속받는다.
- 헤더 클래스 정의에는 가능한한 전방 선언만을 사용합니다. 꼭 헤더 인클루드가 필요할 경우에만 추가 종속성을 인클루드합니다. 컴파일 속도를 향상시키기 위함.
  - 일부 도구(TRefPtr 등)에서는 소멸자에서 클래스의 유효성을 검사하기 때문에 헤더가 필요할 수 있습니다. 이 경우 클래스에 소멸자를 선언하고, cpp 구현에서 빈 소멸자로 구현하세요.
