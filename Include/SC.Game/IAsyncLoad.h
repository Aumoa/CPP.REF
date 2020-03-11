#pragma once

namespace SC::Game
{
	/// <summary> 게임 데이터 자산 불러오기 상태를 표현하는 인터페이스 개체입니다. </summary>
	struct IAsyncLoad : virtual public Object, virtual public Threading::IAsyncOperation<object>
	{
		/// <summary> (Visual Studio 전용) 작업의 진행 상태를 0에서 1사이의 실수로 나타내는 값을 설정하거나 가져옵니다. 값의 설정은 Load 함수의 매개변수에서만 사용할 수 있습니다. </summary>
		vs_property( double, Progress );

		/// <summary> 작업의 진행 상태를 0에서 1사이의 실수로 나타내는 값을 가져옵니다. </summary>
		virtual double Progress_get() = 0;

		/// <summary> 작업의 진행 상태를 0에서 1사이의 실수로 나타내는 값을 설정합니다. Load 함수의 매개변수에서만 사용할 수 있습니다. </summary>
		virtual void Progress_set( double value ) = 0;
	};
}