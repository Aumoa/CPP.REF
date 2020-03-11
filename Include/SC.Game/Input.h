#pragma once

namespace SC::Game
{
	/// <summary> 게임 엔진에서 주 창에 대한 입력 상태를 가져옵니다. </summary>
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
		/// <summary> <see cref="Input"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		Input();

		/// <summary> <see cref="Input"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		Input( const Input& copy );

		/// <summary> 키가 현재 프레임에서 눌러졌는지 검사합니다. </summary>
		bool IsKeyDown( KeyCode keyCode );

		/// <summary> 키가 현재 프레임에서 놓였는지 검사합니다. </summary>
		bool IsKeyUp( KeyCode keyCode );

		/// <summary> 마우스를 잠급니다. 마우스 커서는 보이지 않으며 커서 위치는 가운데로 고정됩니다. </summary>
		static void LockCursor();

		/// <summary> 마우스 잠금을 해제합니다. </summary>
		/// <param name="restore"> 마우스 위치를 잠금 전으로 되돌립니다. </param>
		static void UnlockCursor( bool restore = false );

		/// <summary> (Visual Studio 전용) 키가 눌러짐 상태인지 검사합니다. </summary>
		vs_property_get( bool, KeyState )[];

		/// <summary> 키가 눌러짐 상태인지 검사합니다. </summary>
		bool KeyState_get( KeyCode param0 );

		/// <summary> (Visual Studio 전용) 현재 프레임에서 창 위치를 기준으로 하는 커서 위치를 가져옵니다. </summary>
		vs_property_get( Drawing::Point<int>, MousePosition );

		/// <summary> 현재 프레임에서 창 위치를 기준으로 하는 커서 위치를 가져옵니다. </summary>
		Drawing::Point<int> MousePosition_get();

		/// <summary> (Visual Studio 전용) 현재 프레임에서 커서 이동량을 가져옵니다. </summary>
		vs_property_get( Drawing::Point<int>, DeltaMousePosition );

		/// <summary> 현재 프레임에서 커서 이동량을 가져옵니다. </summary>
		Drawing::Point<int> DeltaMousePosition_get();

		/// <summary> 현재 프레임에서 스크롤 이동량을 가져옵니다. </summary>
		vs_property_get( Drawing::Point<double>, DeltaScroll );

		/// <summary> 현재 프레임에서 스크롤 이동량을 가져옵니다. </summary>
		Drawing::Point<double> DeltaScroll_get();

		/// <summary> (Visual Studio 전용) 커서가 잠김 상태인지 나타내는 값을 가져옵니다. </summary>
		vs_property_get( bool, CursorLocked );

		/// <summary> 커서가 잠김 상태인지 나타내는 값을 가져옵니다. </summary>
		bool CursorLocked_get();

		Input& operator=( const Input& copy );

	private:
		bool IsKeyChanged( KeyCode keyCode );
		static void SetCursorToCenter();
	};
}