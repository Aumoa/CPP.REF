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
- 변경 사항이 완료되면 다시 작성
