#pragma once

namespace SC::Game
{
	/// <summary> 게임 앱 환경 설정을 표현합니다. </summary>
	class AppConfiguration : public ValueType
	{
		friend class Application;

	private:
		String appName;
		bool fullscreenMode = false;
		double scaleFactor = 1.0;
		int physicsFPS = 60;
		bool disableUI = false;
		String deviceName;
		bool verticalSync = false;

	public:
		/// <summary> <see cref="AppConfiguration"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		AppConfiguration();

		/// <summary> <see cref="AppConfiguration"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		AppConfiguration( const AppConfiguration& copy );

		/// <summary> (<see cref="SC::Object"/> 클래스에서 상속 됨.) 현재 개체의 텍스트 표현을 생성하여 반환합니다. </summary>
		String ToString() override;

		/// <summary> (Visual Studio 전용) 앱의 이름을 설정하거나 가져옵니다. </summary>
		vs_property( String, AppName );

		/// <summary> 앱의 이름을 가져옵니다. </summary>
		String AppName_get();

		/// <summary> 앱의 이름을 설정합니다. </summary>
		/// <param name="value"> 값을 전달합니다. </param>
		void AppName_set( String value );

		/// <summary> (Visual Studio 전용) 앱이 전체 화면 모드를 사용하는지 나타내는 값을 설정하거나 가져옵니다. 플랫폼이 전체 화면을 지원하지 않는 경우 항상 true를 반환하며, 값은 무시됩니다. </summary>
		vs_property( bool, FullscreenMode );

		/// <summary> 앱이 전체 화면 모드를 사용하는지 나타내는 값을 가져옵니다. 플랫폼이 전체 화면을 지원하지 않는 경우 항상 true를 반환합니다. </summary>
		bool FullscreenMode_get();

		/// <summary> 앱이 전체 화면 모드를 사용하는지 나타내는 값을 설정합니다. 플랫폼이 전체 화면을 지원하지 않는 경우 값은 무시됩니다. </summary>
		/// <param name="value"> 값을 전달합니다. </param>
		void FullscreenMode_set( bool value );

		/// <summary> (Visual Studio 전용) UI 확대 비율을 설정하거나 가져옵니다. 1 ~ 5 사이의 값을 지정합니다. </summary>
		vs_property( double, ScaleFactor );

		/// <summary> UI 확대 비율을 가져옵니다. </summary>
		double ScaleFactor_get();

		/// <summary> UI 확대 비율을 설정합니다. 1 ~ 5 사이의 값을 지정합니다. </summary>
		void ScaleFactor_set( double value );

		/// <summary> (Visual Studio 전용) 초당 물리 연산 횟수를 설정하거나 가져옵니다. </summary>
		vs_property( int, PhysicsUpdatePerSeconds );

		/// <summary> 초당 물리 연산 횟수를 가져옵니다. </summary>
		int PhysicsUpdatePerSeconds_get();

		/// <summary> 초당 물리 연산 횟수를 설정합니다. </summary>
		void PhysicsUpdatePerSeconds_set( int value );

		/// <summary> (Visual Studio 전용) 앱의 UI 렌더링 기능 활성화 상태를 나타내는 값을 설정하거나 가져옵니다. </summary>
		vs_property( bool, DisableUI );

		/// <summary> 앱의 UI 렌더링 기능 활성화 상태를 나타내는 값을 가져옵니다. </summary>
		bool DisableUI_get();

		/// <summary> 앱의 UI 렌더링 기능 활성화 상태를 나타내는 값을 설정합니다. </summary>
		void DisableUI_set( bool value );

		/// <summary> (Visual Studio 전용) 앱의 그래픽 장치가 사용하는 물리 어댑터 이름을 가져옵니다. </summary>
		vs_property_get( String, AdapterName );

		/// <summary> 앱의 그래픽 장치가 사용하는 물리 어댑터 이름을 가져옵니다. </summary>
		String AdapterName_get();

		/// <summary> (Visual Studio 전용) 앱의 수직 동기화 기능 활성화 상태를 나타내는 값을 설정하거나 가져옵니다. </summary>
		vs_property( bool, VSync );

		/// <summary> 앱의 수직 동기화 기능 활성화 상태를 나타내는 값을 가져옵니다. </summary>
		bool VSync_get();

		/// <summary> 앱의 수직 동기화 기능 활성화 상태를 나타내는 값을 설정합니다. </summary>
		void VSync_set( bool value );

		AppConfiguration& operator=( const AppConfiguration& copy );
	};
}