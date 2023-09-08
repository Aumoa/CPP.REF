# CPP.REF

학습용 Cross-Platform 엔진 샘플

언리얼 엔진과 유니티 엔진의 코드 기반을 참고로 하여 학습용 엔진 구조를 공부하는것에 목적을 둡니다.

## 참고 자료:

[Unity-like Framework](https://github.com/Aumoa/SC.Game)

[Unreal Engine](https://github.com/Aumoa/UnrealEngine) (Unreal Engine 공식 Repos의 fork로, Unreal Engine fork 권한을 참고해주세요.)

## 빌드:

Unreal Engine과 유사한 빌드 시스템을 사용합니다.

Core Framework는 C++ 언어로 작성되며, 언어 레벨은 C++20입니다. 현재 GCC 및 MSVC에서 지원하는 스펙이 검증에 통과하였습니다.

## Windows 환경에서

엔진 코드에서 Setup.bat 배치 파일 실행, 이후 GenerateSolution.bat 배치 파일을 실행합니다. 차례로 DotNET을 이용한 AylaBuildTool 빌드, AylaBuildTool을 이용한 Visual Studio 프로젝트 파일 생성을 지원합니다.
- DotNET 빌드 도구가 필요합니다. 자세한 내용은 Microsoft 공식 [DotNET 빌드 도구](https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-build)(Visual Studio)를 참조하세요.
- 프로젝트 파일은 Editor 빌드를 기준으로 생성됩니다. 컴파일 진행 시 모든 빌드는 플랫폼 및 환경에 맞게 진행되며, 현재 버전에서는 IntelliSense 환경이 정확히 빌드 상황과 일치하지 않을 수 있습니다.

프로젝트 파일이 생성되면, 일반적인 Visual Studio의 빌드 방식을 사용하여 프로젝트를 빌드 및 디버그할 수 있습니다. 자세한 내용은 [Visual Studio의 프로젝트 디버그](https://learn.microsoft.com/ko-kr/visualstudio/debugger/getting-started-with-the-debugger-cpp?view=vs-2022)를 참조하세요.
- 아직 Visual Studio 프로젝트 생성이 완벽하지 않습니다. 일부 환경(주로 Editor)에서 디버그 실행이 원할하지 않을 수 있습니다.
- 이 경우 실행 파일(`Launch.exe`)의 위치를 디버그 설정에서 명확히 지정하여 실행해주세요.

## Linux 환경에서

Linux 환경은 프로젝트 파일 생성을 지원하지 않습니다. (피처) 따라서, AylaBuildTool을 이용한 수동 빌드만 지원됩니다.
- AylaBuildTool을 빌드하기 위해 (YourEngineDirectory)에서 `dotnet build -c Shipping Engine/Source/Programs/AylaBuildTool`을 실행합니다. Setup.bat 배치 스크립트와 동일하지만 Linux에서 실행하는 것만 다릅니다.
- 전 과정으로 AylaBuildTool.dll이 생성된다면, \[모든 환경에서] 섹션을 참고하여 Standalone 빌드 방법<sup>[1](#StandaloneBuild)</sup>을 참고하세요.


## 모든 환경에서

Standalone 빌드 실행 시<sup>[1](#StandaloneBuild)</sup>, `dotnet (YourEngineDirectory)\Engine\Binaries\DotNET\AylaBuildTool.dll Build -Target (YourTargetName) [-Config (YourConfigName)] [-Platform (YourPlatformName)]` 명령을 실행합니다.
- 현재 지원 플랫폼은 Win64, Linux이며 Slate를 사용하는 GUI 환경은 Win64만 지원합니다.
- Android 및 기타 플랫폼은 대상 플랫폼에서 C++20을 지원할 때 포함할 예정입니다. (고려 사항)


<hr>
# 각주

1. <a name="Standalone">모든 환경에서> Standalone 빌드 실행 시: 참고</a>
