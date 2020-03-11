#pragma once

namespace SC::Game
{
	/// <summary> 앱에서 처리하지 않은 비동기 완료 처리기 또는 이벤트 처리기에서 발생한 예외 개체를 제공하는 이벤트 데이터입니다. </summary>
	class UnhandledErrorDetectedEventArgs : public EventArgs
	{
		Exception* e = nullptr;
		bool isCritical = true;

	public:
		/// <summary> <see cref="UnhandledErrorDetectedEventArgs"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="e"> 예외 개체를 전달합니다. </param>
		UnhandledErrorDetectedEventArgs( Exception* e );
		~UnhandledErrorDetectedEventArgs() override;

		/// <summary> (<see cref="EventArgs"/> 클래스에서 상속 됨.) 현재 개체의 텍스트 표현을 생성하여 반환합니다. </summary>
		String ToString() override;

		/// <summary> (Visual Studio 전용) 앱에서 처리되지 않은 예외 개체를 가져옵니다. </summary>
		vs_property_get( SC::Exception*, Exception );

		/// <summary> 앱에서 처리되지 않은 예외 개체를 가져옵니다. </summary>
		SC::Exception* Exception_get();

		/// <summary> (Visual Studio 전용) 이 예외를 심각한 예외로 간주하는 값을 설정하거나 가져옵니다. </summary>
		vs_property( bool, IsCritical );

		/// <summary> 이 예외를 심각한 예외로 간주하는 값을 가져옵니다. </summary>
		bool IsCritical_get();

		/// <summary> 이 예외를 심각한 예외로 간주하는 값을 설정합니다. </summary>
		void IsCritical_set( bool value );
	};
}