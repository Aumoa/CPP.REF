#pragma once

namespace SC::Game
{
	/// <summary> ���� �������� �� â�� ���� �Է� ���¸� �����ɴϴ�. </summary>
	class Input : public ValueType
	{
		friend class Scene;

	private:
		Drawing::Point<int> prevCursorPos;
		Drawing::Point<int> cursorPos;

		Drawing::Point<int> scrollDelta = 0;

		std::vector<bool> prevKeyboardState;
		std::vector<bool> keyboardState;

		static bool locked;
		static Drawing::Point<int> lockedCursor;

	public:
		/// <summary> <see cref="Input"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Input();

		/// <summary> <see cref="Input"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Input( const Input& copy );

		/// <summary> Ű�� ���� �����ӿ��� ���������� �˻��մϴ�. </summary>
		bool IsKeyDown( KeyCode keyCode );

		/// <summary> Ű�� ���� �����ӿ��� �������� �˻��մϴ�. </summary>
		bool IsKeyUp( KeyCode keyCode );

		/// <summary> ���콺�� ��޴ϴ�. ���콺 Ŀ���� ������ ������ Ŀ�� ��ġ�� ����� �����˴ϴ�. </summary>
		static void LockCursor();

		/// <summary> ���콺 ����� �����մϴ�. </summary>
		/// <param name="restore"> ���콺 ��ġ�� ��� ������ �ǵ����ϴ�. </param>
		static void UnlockCursor( bool restore = false );

		/// <summary> (Visual Studio ����) Ű�� ������ �������� �˻��մϴ�. </summary>
		vs_property_get( bool, KeyState )[];

		/// <summary> Ű�� ������ �������� �˻��մϴ�. </summary>
		bool KeyState_get( KeyCode param0 );

		/// <summary> (Visual Studio ����) ���� �����ӿ��� â ��ġ�� �������� �ϴ� Ŀ�� ��ġ�� �����ɴϴ�. </summary>
		vs_property_get( Drawing::Point<int>, MousePosition );

		/// <summary> ���� �����ӿ��� â ��ġ�� �������� �ϴ� Ŀ�� ��ġ�� �����ɴϴ�. </summary>
		Drawing::Point<int> MousePosition_get();

		/// <summary> (Visual Studio ����) ���� �����ӿ��� Ŀ�� �̵����� �����ɴϴ�. </summary>
		vs_property_get( Drawing::Point<int>, DeltaMousePosition );

		/// <summary> ���� �����ӿ��� Ŀ�� �̵����� �����ɴϴ�. </summary>
		Drawing::Point<int> DeltaMousePosition_get();

		/// <summary> ���� �����ӿ��� ��ũ�� �̵����� �����ɴϴ�. </summary>
		vs_property_get( Drawing::Point<double>, DeltaScroll );

		/// <summary> ���� �����ӿ��� ��ũ�� �̵����� �����ɴϴ�. </summary>
		Drawing::Point<double> DeltaScroll_get();

		/// <summary> (Visual Studio ����) Ŀ���� ��� �������� ��Ÿ���� ���� �����ɴϴ�. </summary>
		vs_property_get( bool, CursorLocked );

		/// <summary> Ŀ���� ��� �������� ��Ÿ���� ���� �����ɴϴ�. </summary>
		bool CursorLocked_get();

		Input& operator=( const Input& copy );

	private:
		bool IsKeyChanged( KeyCode keyCode );
		static void SetCursorToCenter();
	};
}