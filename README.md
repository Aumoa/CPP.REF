# 프로젝트 빌드 요구 사항
- Visual Studio 2019 (C++ 포함) (C++20 기능을 사용합니다.)
- Python (명령 프롬프트에 Python을 입력하면 쉽게 설치 가능)
- cmake

# ThirdParty 빌드
- generate_projects.bat을 실행합니다. Python 및 cmake가 준비되어야 합니다.
- 서드파티 프로젝트가 준비되기 전 솔루션을 실행하면 서드파티 프로젝트가 비활성화 상태가 됩니다. 솔루션을 닫은 다음 프로젝트를 준비하고 다시 시도하세요.
- 솔루션을 사용 중인 채로 프로젝트를 준비하여 모든 프로젝트를 다시 로드할 경우 종속성이 제대로 로드되지 않을 수 있습니다. 그 경우 위 단계를 다시 시도하세요.

# 프로젝트 보드
- https://aumoa.visualstudio.com/CPP.REF/

# 코드 작성 규칙
- 기본적으로, 특수한 기능을 수행하지 않는 모든 일반적인 클래스는 Object 클래스를 상속 받습니다. 다음 규칙을 갖습니다.
1. Object 클래스를 상속받을 때는 반드시 virtual public으로 상속받는다.
2. 인터페이스 역시 동일하게 이 규칙을 적용한다.
3. Object가 아닌 클래스는 virtual 상속으로 받지 않으며, 오직 하나의 클래스만 상속받을 수 있다.
4. 인터페이스를 상속 받을 땐 virtual public으로 상속받는다.
- 헤더 클래스 정의에는 가능한한 전방 선언만을 사용합니다. 꼭 헤더 인클루드가 필요할 경우에만 추가 종속성을 인클루드합니다. 컴파일 속도를 향상시키기 위함.
  - 일부 도구(TRefPtr 등)에서는 소멸자에서 클래스의 유효성을 검사하기 때문에 헤더가 필요할 수 있습니다. 이 경우 클래스에 소멸자를 선언하고, cpp 구현에서 빈 소멸자로 구현하세요.