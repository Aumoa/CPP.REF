#pragma once

namespace SC::Game::UI
{
	/// <summary> ���콺 Ŭ�� ���� �̺�Ʈ�� �Ű������Դϴ�. </summary>
	struct MouseClickEventArgs : public ValueType, virtual public IEquatable<MouseClickEventArgs>
	{
		/// <summary> ���콺 ��ư ������ �����մϴ�. </summary>
		MouseButtonType Button;

		/// <summary> ���콺 ��ư�� ���� �������� Ȯ���ϴ� ���� �����մϴ�. </summary>
		bool IsDown;

		/// <summary> ���콺 ��ư�� ��ġ�� �����մϴ�. </summary>
		Drawing::Point<int> CursorPos;

		/// <summary> <see cref="MouseClickEventArgs"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		MouseClickEventArgs();

		/// <summary> <see cref="MouseClickEventArgs"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		MouseClickEventArgs( MouseButtonType type, bool isDown, Drawing::Point<int> cursorPos );

		/// <summary> <see cref="MouseClickEventArgs"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		MouseClickEventArgs( const MouseClickEventArgs& copy );

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ��ü�� �ؽ�Ʈ ǥ���� �����ɴϴ�. </summary>
		String ToString() override;

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ������ ��ü�� ���� ��ü�� ������ Ȯ���մϴ�. </summary>
		/// <param name="obj"> ���� ��ü�� �����մϴ�. </param>
		bool Equals( object obj ) override;

		/// <summary> (<see cref="IEquatable"/> �������̽����� ���� ��.) ���� ��ü�� ������ ������ �ٸ� ��ü�� �������� ���մϴ�. </summary>
		/// <param name="right"> ������ ������ ��ü�� �����մϴ�. </param>
		bool Equals( MouseClickEventArgs right ) override;

		MouseClickEventArgs& operator=( const MouseClickEventArgs& copy );
	};
}