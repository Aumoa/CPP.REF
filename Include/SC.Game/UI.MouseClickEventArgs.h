#pragma once

namespace SC::Game::UI
{
	/// <summary> 마우스 클릭 관련 이벤트의 매개변수입니다. </summary>
	struct MouseClickEventArgs : public ValueType, virtual public IEquatable<MouseClickEventArgs>
	{
		/// <summary> 마우스 버튼 유형을 설정합니다. </summary>
		MouseButtonType Button;

		/// <summary> 마우스 버튼이 누름 상태인지 확인하는 값을 설정합니다. </summary>
		bool IsDown;

		/// <summary> 마우스 버튼의 위치를 설정합니다. </summary>
		Drawing::Point<int> CursorPos;

		/// <summary> <see cref="MouseClickEventArgs"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		MouseClickEventArgs();

		/// <summary> <see cref="MouseClickEventArgs"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		MouseClickEventArgs( MouseButtonType type, bool isDown, Drawing::Point<int> cursorPos );

		/// <summary> <see cref="MouseClickEventArgs"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		MouseClickEventArgs( const MouseClickEventArgs& copy );

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 개체의 텍스트 표현을 가져옵니다. </summary>
		String ToString() override;

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 지정한 개체가 현재 개체와 같은지 확인합니다. </summary>
		/// <param name="obj"> 비교할 개체를 전달합니다. </param>
		bool Equals( object obj ) override;

		/// <summary> (<see cref="IEquatable"/> 인터페이스에서 구현 됨.) 현재 개체가 동일한 종류의 다른 개체와 동일한지 비교합니다. </summary>
		/// <param name="right"> 동일한 종류의 개체를 전달합니다. </param>
		bool Equals( MouseClickEventArgs right ) override;

		MouseClickEventArgs& operator=( const MouseClickEventArgs& copy );
	};
}