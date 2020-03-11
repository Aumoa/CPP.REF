#pragma once

namespace SC::Game::UI
{
	/// <summary> Ű���� ���� �̺�Ʈ�� �Ű������Դϴ�. </summary>
	struct KeyboardEventArgs : public ValueType, virtual public IEquatable<KeyboardEventArgs>
	{
		/// <summary> ���� Ű�� Ű�ڵ带 �����մϴ�. </summary>
		KeyCode Key;

		/// <summary> ���콺 ��ư�� ���� �������� Ȯ���ϴ� ���� �����մϴ�. </summary>
		bool IsDown;

		/// <summary> <see cref="MouseClickEventArgs"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		KeyboardEventArgs();

		/// <summary> <see cref="MouseClickEventArgs"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		KeyboardEventArgs( KeyCode key, bool isDown );

		/// <summary> <see cref="MouseClickEventArgs"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		KeyboardEventArgs( const KeyboardEventArgs& copy );

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ��ü�� �ؽ�Ʈ ǥ���� �����ɴϴ�. </summary>
		String ToString() override;

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ������ ��ü�� ���� ��ü�� ������ Ȯ���մϴ�. </summary>
		/// <param name="obj"> ���� ��ü�� �����մϴ�. </param>
		bool Equals( object obj ) override;

		/// <summary> (<see cref="IEquatable"/> �������̽����� ���� ��.) ���� ��ü�� ������ ������ �ٸ� ��ü�� �������� ���մϴ�. </summary>
		/// <param name="right"> ������ ������ ��ü�� �����մϴ�. </param>
		bool Equals( KeyboardEventArgs right ) override;

		KeyboardEventArgs& operator=( const KeyboardEventArgs& copy );
	};
}