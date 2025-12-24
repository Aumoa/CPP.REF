# CPP.REF

**Modern Cross-Platform Game Engine** - Unreal Engine과 Unity의 장점을 결합한 학습 및 연구용 게임 엔진

[![Language](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![.NET](https://img.shields.io/badge/.NET-9-purple.svg)](https://dotnet.microsoft.com/)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-green.svg)](#supported-platforms)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

## 🎯 프로젝트 목표

CPP.REF는 Unreal Engine과 Unity Engine의 설계 철학을 연구하고, 두 엔진의 장점을 결합하여 모던 C++와 C# 스크립팅을 통합한 새로운 형태의 게임 엔진입니다.

### 핵심 특징

- 🚀 **C# TPL 스타일의 Task System** - `Task<T>`, `async/await`, `TaskScheduler` 등 완전한 비동기 프로그래밍 지원
- ⚡ **Full Raytracing Rendering** - 차세대 레이트레이싱 기반 렌더링 파이프라인
- 🔗 **CoreCLR 통합** - C++와 C# 간의 원활한 Marshal과 상호 운용성
- 🌐 **Cross-Platform** - Windows, Linux, macOS 지원 (Android/iOS 계획 중)
- 🎨 **Modern C++20** - Coroutines, Concepts, Ranges 등 최신 C++ 기능 활용
- 🔧 **Modular Architecture** - Unreal Engine 스타일의 모듈 시스템

## 🏗️ 아키텍처

```
CPP.REF/
├── Engine/                      # 엔진 코어
│   ├── Source/
│   │   ├── Runtime/            # 런타임 모듈
│   │   │   ├── Core/          # 핵심 시스템 (Task, Memory, Threading)
│   │   │   ├── Engine/        # 게임 엔진 로직
│   │   │   ├── RenderCore/    # 렌더링 추상화
│   │   │   ├── Direct3D12/    # D3D12 백엔드
│   │   │   ├── VulkanAPI/     # Vulkan 백엔드
│   │   │   └── */Script/      # C# 바인딩
│   │   ├── Programs/
│   │   │   └── AylaBuildTool/ # 커스텀 빌드 시스템
│   │   └── Editor/             # 에디터 도구
│   └── Binaries/               # 빌드 출력
│
└── YourGame/                    # 게임 프로젝트 (분리된 디렉토리)
    └── Source/
        └── Runtime/
            └── GameAssembly/   # 게임 코드
```

## 🚀 빠른 시작

### 필수 요구사항

| 항목 | 버전 | 링크 |
|------|------|------|
| **C++ 컴파일러** | MSVC 2022+ / GCC 11+ / Clang 14+ | C++20 지원 필수 |
| **.NET SDK** | 9.0+ | [다운로드](https://dotnet.microsoft.com/download) |
| **CMake** | 3.20+ | [다운로드](https://cmake.org/download/) (Third-party 빌드용) |
| **Vulkan SDK** | 1.3+ | [다운로드](https://vulkan.lunarg.com/) (선택사항) |

### Windows에서 빌드

```batch
# 1. 저장소 클론
git clone https://github.com/Aumoa/CPP.REF.git
cd CPP.REF

# 2. 빌드 도구 설정
Setup.bat

# 3. Visual Studio 솔루션 생성
GenerateSolution.bat

# 4. Visual Studio에서 빌드
# CPP.REF.sln 열기 → F7 (빌드) → F5 (디버그)
```

### Linux/macOS에서 빌드

```bash
# 1. 저장소 클론
git clone https://github.com/Aumoa/CPP.REF.git
cd CPP.REF

# 2. AylaBuildTool 빌드
dotnet build -c Shipping Engine/Source/Programs/AylaBuildTool

# 3. 게임 빌드
dotnet Engine/Binaries/DotNET/AylaBuildTool.dll Build \
    -Target Launch \
    -Config Debug \
    -Platform Linux

# 4. 실행
./Engine/Binaries/Linux/Launch
```

## 📚 핵심 기술

### 1. Modern Task Parallel Library

C# TPL과 동일한 비동기 프로그래밍 모델을 C++로 구현:

```cpp
// C++20 Coroutines + Task System
Task<int> CalculateAsync()
{
    // 백그라운드 스레드에서 실행
    auto result = co_await Task<>::Run([]() {
        return ExpensiveComputation();
    });
    
    // 여러 작업 병렬 실행
    auto tasks = {
        FetchDataAsync(),
        ProcessImageAsync(),
        LoadAssetsAsync()
    };
    co_await Task<>::WhenAll(tasks);
    
    co_return result;
}
```

**지원 기능:**
- ✅ `Task<T>` / `Task<>` (void)
- ✅ `co_await` / `async` 패턴
- ✅ `TaskScheduler` / `SynchronizationContext`
- ✅ `Task::Run`, `Task::WhenAll`, `Task::WhenAny`
- ✅ `Task::Delay`, `Task::Unwrap`
- ✅ `CancellationToken` (via `std::stop_token`)
- ✅ `TaskCreationOptions::LongRunning`

### 2. CoreCLR 스크립팅

C++와 C# 간의 원활한 통합:

```csharp
// C# 게임 로직
public class GameController : MonoBehaviour
{
    private async Task LoadLevelAsync()
    {
        // C++ 엔진 API 호출
        var level = await AssetDatabase.LoadAsync<Level>("MainLevel");
        
        // UI 업데이트 (메인 스레드)
        await Task.Yield(); // SynchronizationContext 활용
        UpdateLoadingBar(100);
    }
}
```

### 3. Raytracing Rendering

완전한 레이트레이싱 기반 렌더링:
- **Direct3D 12 DXR** 지원
- **Vulkan Ray Tracing** 지원
- Real-time Global Illumination
- Path Tracing

## 🛠️ AylaBuildTool

Unreal Engine의 UnrealBuildTool과 유사한 커스텀 빌드 시스템:

```bash
# 기본 사용법
dotnet AylaBuildTool.dll <Command> [Options]

# 예시
dotnet AylaBuildTool.dll Build -Target Launch -Config Debug -Platform Win64
dotnet AylaBuildTool.dll Generate -IDE VisualStudio2022
dotnet AylaBuildTool.dll Clean
```

### 지원 플랫폼

| 플랫폼 | 상태 | 컴파일러 | 렌더링 API |
|--------|------|----------|-----------|
| **Windows** | ✅ 지원 | MSVC 2022+ | D3D12, Vulkan |
| **Linux** | ✅ 지원 | GCC 11+, Clang 14+ | Vulkan |
| **macOS** | 🚧 진행중 | Clang 14+ | Vulkan (MoltenVK) |
| **Android** | 📋 계획 | NDK r25+ | Vulkan |
| **iOS** | 📋 계획 | Xcode 14+ | Metal (via Vulkan) |

## 📖 문서

- [📘 아키텍처 가이드](docs/Architecture.md)
- [🔧 빌드 시스템](docs/BuildSystem.md)
- [⚡ Task System 가이드](docs/TaskSystem.md)
- [🎨 렌더링 파이프라인](docs/Rendering.md)
- [🔗 C# 스크립팅 API](docs/Scripting.md)

## 🎓 참고 자료

이 프로젝트는 다음 엔진들의 설계를 연구하여 제작되었습니다:

- [**Unreal Engine**](https://github.com/Aumoa/UnrealEngine) - 모듈 시스템, 빌드 도구 (Unreal 공식 저장소 fork)
- [**Unity Engine**](https://github.com/Aumoa/SC.Game) - 컴포넌트 시스템, 스크립팅 (Unity 기준으로 마음대로 구현한 프로젝트)
- **.NET TPL** - 비동기 프로그래밍 모델

## 🔧 개발 환경 설정

### Visual Studio (권장)

```batch
# 솔루션 생성
GenerateSolution.bat

# Launch를 시작 프로젝트로 설정
# 디버깅 설정: $(OutDir)Launch.exe
```

### Visual Studio Code

```bash
# VSCode 워크스페이스 생성
dotnet AylaBuildTool.dll generate -p "ProjectName" -g "VisualStudioCode"

# .vscode/launch.json, tasks.json 자동 생성됨
code .
```

## 🐛 알려진 제한사항

- ⚠️ Visual Studio 프로젝트 생성이 일부 환경에서 불완전할 수 있음
- ⚠️ IntelliSense가 실제 빌드 설정과 다를 수 있음
- ⚠️ macOS 지원은 현재 개발 중

## 🤝 기여

이 프로젝트는 주로 개인 학습 목적으로 진행되고 있지만, 기여는 환영합니다!

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📄 라이선스

이 프로젝트는 [MIT License](LICENSE) 하에 배포됩니다.

## 📞 연락처

- **GitHub**: [@Aumoa](https://github.com/Aumoa)
- **Issues**: [GitHub Issues](https://github.com/Aumoa/CPP.REF/issues)

---

<p align="center">
  <i>Made with ❤️ for learning and research</i>
</p>
