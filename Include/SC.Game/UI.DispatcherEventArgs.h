#pragma once

namespace SC::Game::UI
{
	/// <summary> EventDispatcher 클래스로 전송하는 이벤트 메시지에 대한 매개변수를 표현합니다. </summary>
	class DispatcherEventArgs : public EventArgs
	{
		DispatcherEventType type;
		object args;
		bool isCompleted = false;

	public:
		/// <summary> <see cref="DispatcherEventArgs"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="type"> 이벤트 유형을 전달합니다. </param>
		/// <param name="args"> 이벤트 매개변수를 전달합니다. </param>
		DispatcherEventArgs( DispatcherEventType type, object args );

		/// <summary> (Visual Studio 전용) 이벤트 유형을 가져옵니다. </summary>
		vs_property_get( DispatcherEventType, Type );

		/// <summary> 이벤트 유형을 가져옵니다. </summary>
		DispatcherEventType Type_get();

		/// <summary> (Visual Studio 전용) 이벤트 매개변수를 가져옵니다. </summary>
		vs_property_get( object, Argument );

		/// <summary> 이벤트 매개변수를 가져옵니다. </summary>
		object Argument_get();

		/// <summary> (Visual Studio 전용) 이 이벤트가 이미 처리되었는지 나타내는 값을 설정하거나 가져옵니다. </summary>
		vs_property( bool, IsCompleted );

		/// <summary> 이 이벤트가 이미 처리되었는지 나타내는 값을 가져옵니다. </summary>
		bool IsCompleted_get();

		/// <summary> 이 이벤트가 이미 처리되었는지 나타내는 값을 설정합니다. </summary>
		void IsCompleted_set( bool value );
	};
}