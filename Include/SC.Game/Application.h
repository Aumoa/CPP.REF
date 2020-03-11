#pragma once

namespace SC::Game
{
	/// <summary> 게임 앱에 대한 제어 함수를 제공하며 앱의 기초 처리기에 대한 인터페이스를 제공합니다. </summary>
	class Application : public Object
	{
		AppConfiguration appConfig;
		double dpi;
		bool discardApp = false;
		
		RefPtr<UI::Canvas> frame;

		sc_game_export_object( ComPtr<ID3D12CommandAllocator> ) pCommandAllocators[2];
		uint64 lastPending[2]{ };
		RefPtr<Details::CDeviceContext> deviceContextUI;
		sc_game_export_object( RefPtr<Details::VisibleViewStorage> ) visibleViewStorage;
		RefPtr<Threading::Event> waitingHandle = new Threading::Event();

		sc_game_export_float( D3D12_VIEWPORT, 6 ) viewport;
		sc_game_export_float( D3D12_RECT, 4 ) scissor;

	protected:
		/// <summary> <see cref="Application"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="appConfig"> 앱 설정을 전달합니다. </param>
		Application( AppConfiguration appConfig );

		/// <summary> 앱이 시작될 때 호출됩니다. </summary>
		virtual void OnStart() = 0;

		/// <summary> 앱이 종료될 때 호출됩니다. 앱은 종료 코드를 반환해야 합니다. </summary>
		virtual int OnExit() = 0;

	public:
		~Application() override;

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 현재 개체의 텍스트 표현을 생성하여 반환합니다. </summary>
		String ToString() override;

		/// <summary> 응용 프로그램의 진입점에 의해 앱을 실행합니다. 이 함수는 앱이 종료될 때까지 반환하지 않습니다. </summary>
		/// <param name="app"> 게임 앱 개체를 전달합니다. </param>
		static int Start( RefPtr<Application> app );

		/// <summary> (Visual Studio 전용) 앱의 화면 크기를 가져옵니다. </summary>
		vs_property_get( Drawing::Point<int>, ClientSize );

		/// <summary> 앱의 화면 크기를 가져옵니다. </summary>
		Drawing::Point<int> ClientSize_get();

		/// <summary> (Visual Studio 전용) 앱의 설정을 가져옵니다. </summary>
		vs_property_get( AppConfiguration, AppConfig );

		/// <summary> 앱의 설정을 가져옵니다. </summary>
		AppConfiguration AppConfig_get();

		/// <summary> (Visual Studio 전용) 앱의 메인 UI 프레임을 가져옵니다. </summary>
		vs_property_get( RefPtr<UI::Canvas>, Frame );

		/// <summary> 앱의 메인 UI 프레임을 가져옵니다. </summary>
		RefPtr<UI::Canvas> Frame_get();

		/// <summary> (Visual Studio 전용) 앱의 이름이 사용 가능한 플랫폼에서 앱의 이름을 설정하거나 가져옵니다. </summary>
		vs_property( String, AppName );

		/// <summary> 앱의 이름이 사용 가능한 플랫폼에서 앱의 이름을 가져옵니다. </summary>
		String AppName_get();

		/// <summary> 앱의 이름이 사용 가능한 플랫폼에서 앱의 이름을 설정합니다. </summary>
		void AppName_set( String value );

		/// <summary> 앱에서 처리하지 않은 비동기 완료 처리기 또는 이벤트 처리기에 오류가 있는 경우 발생합니다. </summary>
		static Event<RefPtr<UnhandledErrorDetectedEventArgs>> UnhandledErrorDetected;

	private:
		void InitializeDevice();
		static void* __stdcall WndProc( void* hWnd, uint32 uMsg, void* wParam, void* lParam );
		void ResizeBuffers( uint32 width, uint32 height );

		void IdleProcess();
		void Update();
		void Render();

		void LoadSystemFont();

		static void WaitAllQueues();
		void WaitPrimaryQueue();
	};
}