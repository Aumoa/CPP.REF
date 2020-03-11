#pragma once

namespace SC::Game::UI
{
	/// <summary> 마우스 이동 관련 이벤트의 매개변수입니다. </summary>
	struct MouseMoveEventArgs : public ValueType, virtual public IEquatable<MouseMoveEventArgs>
	{
		/// <summary> 절대적 위치를 설정합니다. </summary>
		Drawing::Point<int> AbsCursorPos;

		/// <summary> 상대적 커서 위치를 설정합니다. </summary>
		Drawing::Point<int> DeltaCursorPos;

		/// <summary> 상대적 스크롤 위치 변경점을 설정합니다. </summary>
		Drawing::Point<int> DeltaScroll;

		/// <summary> <see cref="MouseMoveEventArgs"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		MouseMoveEventArgs();

		/// <summary> <see cref="MouseMoveEventArgs"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		MouseMoveEventArgs( Drawing::Point<int> absCursorPos, Drawing::Point<int> deltaCursorPos );

		/// <summary> <see cref="MouseMoveEventArgs"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		MouseMoveEventArgs( Drawing::Point<int> deltaScroll );

		/// <summary> <see cref="MouseMoveEventArgs"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		MouseMoveEventArgs( const MouseMoveEventArgs& copy );

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 개체의 텍스트 표현을 가져옵니다. </summary>
		String ToString() override;

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 지정한 개체가 현재 개체와 같은지 확인합니다. </summary>
		/// <param name="obj"> 비교할 개체를 전달합니다. </param>
		bool Equals( object obj ) override;

		/// <summary> (<see cref="IEquatable"/> 인터페이스에서 구현 됨.) 현재 개체가 동일한 종류의 다른 개체와 동일한지 비교합니다. </summary>
		/// <param name="right"> 동일한 종류의 개체를 전달합니다. </param>
		bool Equals( MouseMoveEventArgs right ) override;

		MouseMoveEventArgs& operator=( const MouseMoveEventArgs& copy );
	};
}