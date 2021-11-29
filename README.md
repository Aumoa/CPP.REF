# 프로젝트 빌드 요구 사항
- Visual Studio 2019 16.10 이상 (C++ 포함) (C++20 기능을 사용합니다.)

# 코드 작성 규칙
## 기본적으로, 특수한 기능을 수행하지 않는 모든 일반적인 클래스는 Object 클래스를 상속 받습니다. 다음 규칙을 갖습니다.
1. Object 클래스를 상속받을 때는 반드시 implements 키워드를 사용하여 상속받는다(virtual public으로 정의 됨.)
2. 인터페이스 역시 동일하게 이 규칙을 적용한다.
3. Object가 아닌 클래스는 virtual 상속으로 받지 않으며, Object 인터페이스를 공유하는 상위 클래스는 하나만 존재할 수 있다.
4. Object를 상속받는 인터페이스가 아닌 모든 클래스는 S 접두사로 시작한다.
5. Object 구현이 포함되는 모든 인터페이스는 I 접두사로 시작한다.

# 프로젝트 생성
## Visual Studio용 프로젝트 생성은 Engine/Binaries/VisualStudioProjects.dll 모듈을 통해 진행됩니다. 터미널을 통해 다음 과정을 따릅니다.
### 엔진 프로젝트 생성
1. 엔진 프로젝트 루트로 이동
2. GenerateProjects.bat 실행
### 게임 프로젝트 생성
1. 게임 프로젝트 워크스페이스로 이동.
2. 프로젝트 정의 솔루션 XML 파일 생성.
3. 모듈 프로젝트 XML 파일 생성 (Source/%ModuleName%/%ModuleName%.xml)
4. 터미널 명령 "%EngineRoot%/Binaries/WindowsConsole.exe --ConsoleDll "VisualStudioProjects.dll" --Solution "%SolutionXml%"
### 게임 솔루션 XML 파일 예시
```xml
<SolutionInfo Name="MyGame">
  <EngineRoot Path="../CPP.REF/Engine" />
  <ThirdpartyRoot Path="../CPP.REF/ThirdParty" />
  <FirstProject Name="MyGame" />
</SolutionInfo>
```
### 게임 모듈 프로젝트 XML 파일 예시
```xml
<ProjectInfo Name="MyGameModule" Path="MyGameModule" Type="Game" >
	<IncludePaths>
		<Item RelativePath="Public" Access="Public" />
		<Item RelativePath="Private" />
	</IncludePaths>
	<ReferencedProjects>
		<Item Name="Core" />
		<Item Name="Game" />
	</ReferencedProjects>
</ProjectInfo>
```

# 개요
- 이 프로젝트는 영리를 목적으로 사용되지 않습니다. 순수하게 학습의 목적입니다.
- 언리얼 엔진의 구성을 많이 참고하였습니다.