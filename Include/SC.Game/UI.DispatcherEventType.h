#pragma once

namespace SC::Game::UI
{
	/// <summary> UI 이벤트 처리기에서 처리할 수 있는 이벤트 유형을 표현합니다. </summary>
	enum class DispatcherEventType
	{
		/// <summary> 마우스 이동 관련 이벤트입니다. </summary>
		MouseMove = 0,

		/// <summary> 마우스 클릭 관련 이벤트입니다. </summary>
		MouseClick = 1,

		/// <summary> 키보드 누름, 놓음 관련 이벤트입니다. </summary>
		KeyboardEvent = 2,
	};
}

inline std::wostream& operator<<( std::wostream& wout, SC::Game::UI::DispatcherEventType value )
{
	static constexpr const wchar_t* context[] =
	{
		L"MouseMove",
		L"MouseClick",
		L"KeyboardEvent"
	};

	if ( ( int )value >= 0 && ( int )value < 3 )
	{
		return wout << context[( int )value];
	}
	else
	{
		return wout << L"SC.Game.UI.DispatcherEventType(" << ( int )value << L")";
	}
}