#pragma once

namespace SC::Game::UI
{
	/// <summary> ���콺 �̵� ���� �̺�Ʈ�� �Ű������Դϴ�. </summary>
	struct MouseMoveEventArgs : public ValueType, virtual public IEquatable<MouseMoveEventArgs>
	{
		/// <summary> ������ ��ġ�� �����մϴ�. </summary>
		Drawing::Point<int> AbsCursorPos;

		/// <summary> ����� Ŀ�� ��ġ�� �����մϴ�. </summary>
		Drawing::Point<int> DeltaCursorPos;

		/// <summary> ����� ��ũ�� ��ġ �������� �����մϴ�. </summary>
		Drawing::Point<int> DeltaScroll;

		/// <summary> <see cref="MouseMoveEventArgs"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		MouseMoveEventArgs();

		/// <summary> <see cref="MouseMoveEventArgs"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		MouseMoveEventArgs( Drawing::Point<int> absCursorPos, Drawing::Point<int> deltaCursorPos );

		/// <summary> <see cref="MouseMoveEventArgs"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		MouseMoveEventArgs( Drawing::Point<int> deltaScroll );

		/// <summary> <see cref="MouseMoveEventArgs"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		MouseMoveEventArgs( const MouseMoveEventArgs& copy );

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ��ü�� �ؽ�Ʈ ǥ���� �����ɴϴ�. </summary>
		String ToString() override;

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ������ ��ü�� ���� ��ü�� ������ Ȯ���մϴ�. </summary>
		/// <param name="obj"> ���� ��ü�� �����մϴ�. </param>
		bool Equals( object obj ) override;

		/// <summary> (<see cref="IEquatable"/> �������̽����� ���� ��.) ���� ��ü�� ������ ������ �ٸ� ��ü�� �������� ���մϴ�. </summary>
		/// <param name="right"> ������ ������ ��ü�� �����մϴ�. </param>
		bool Equals( MouseMoveEventArgs right ) override;

		MouseMoveEventArgs& operator=( const MouseMoveEventArgs& copy );
	};
}