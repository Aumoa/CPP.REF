#pragma once

namespace SC::Diagnostics
{
	/// <summary> <see cref="StepTimer"/> 클래스에서 역참조하는 대리자 형식을 정의합니다. </summary>
	struct StepTimerCallbackDelegate : public Delegate
	{
	private:
		std::function<void( RefPtr<StepTimer> sender )> callback;

	public:
		/// <summary> <see cref="StepTimerCallbackDelegate"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		StepTimerCallbackDelegate() = default;

		/// <summary> <see cref="StepTimerCallbackDelegate"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="callback"> 함수를 전달합니다. </param>
		StepTimerCallbackDelegate( std::function<void( RefPtr<StepTimer> sender )> callback );

		/// <summary> <see cref="StepTimerCallbackDelegate"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="caller"> 함수를 호출하는 소유자 개체를 전달합니다. </param>
		/// <param name="callback"> 함수를 전달합니다. </param>
		template< class T >
		inline StepTimerCallbackDelegate( T* caller, void( T::*callback )( RefPtr<StepTimer> sender ) );

		/// <summary> <see cref="StepTimerCallbackDelegate"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="caller"> 함수를 호출하는 소유자 개체를 전달합니다. </param>
		/// <param name="callback"> 함수를 전달합니다. </param>
		template< class T >
		inline StepTimerCallbackDelegate( RefPtr<T> caller, void( T::*callback )( RefPtr<StepTimer> sender ) );

		/// <summary> <see cref="StepTimerCallbackDelegate"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		StepTimerCallbackDelegate( const StepTimerCallbackDelegate& copy );

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 현재 개체의 텍스트 표현을 생성하여 반환합니다. </summary>
		String ToString() override;

		/// <summary> (Visual Studio 전용) 연결된 대리자 개체가 유효한지 검사합니다. </summary>
		vs_property( bool, IsValid );

		/// <summary> 연결된 대리자 개체가 유효한지 검사합니다. </summary>
		bool IsValid_get();

		/// <summary> 함수를 호출합니다. </summary>
		/// <param name="sender"> 호출자 개체를 전달합니다. </param>
		void Invoke( RefPtr<StepTimer> sender );

		void operator()( RefPtr<StepTimer> sender );
	};
}