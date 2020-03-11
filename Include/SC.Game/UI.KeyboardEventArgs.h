#pragma once

namespace SC::Game::UI
{
	/// <summary> 키보드 관련 이벤트의 매개변수입니다. </summary>
	struct KeyboardEventArgs : public ValueType, virtual public IEquatable<KeyboardEventArgs>
	{
		/// <summary> 누른 키의 키코드를 설정합니다. </summary>
		KeyCode Key;

		/// <summary> 마우스 버튼이 누름 상태인지 확인하는 값을 설정합니다. </summary>
		bool IsDown;

		/// <summary> <see cref="MouseClickEventArgs"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		KeyboardEventArgs();

		/// <summary> <see cref="MouseClickEventArgs"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		KeyboardEventArgs( KeyCode key, bool isDown );

		/// <summary> <see cref="MouseClickEventArgs"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		KeyboardEventArgs( const KeyboardEventArgs& copy );

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 개체의 텍스트 표현을 가져옵니다. </summary>
		String ToString() override;

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 지정한 개체가 현재 개체와 같은지 확인합니다. </summary>
		/// <param name="obj"> 비교할 개체를 전달합니다. </param>
		bool Equals( object obj ) override;

		/// <summary> (<see cref="IEquatable"/> 인터페이스에서 구현 됨.) 현재 개체가 동일한 종류의 다른 개체와 동일한지 비교합니다. </summary>
		/// <param name="right"> 동일한 종류의 개체를 전달합니다. </param>
		bool Equals( KeyboardEventArgs right ) override;

		KeyboardEventArgs& operator=( const KeyboardEventArgs& copy );
	};
}