#pragma once

namespace SC::Game::UI
{
	/// <summary> UI �̺�Ʈ ó���⿡�� ó���� �� �ִ� �̺�Ʈ ������ ǥ���մϴ�. </summary>
	enum class DispatcherEventType
	{
		/// <summary> ���콺 �̵� ���� �̺�Ʈ�Դϴ�. </summary>
		MouseMove = 0,

		/// <summary> ���콺 Ŭ�� ���� �̺�Ʈ�Դϴ�. </summary>
		MouseClick = 1,

		/// <summary> Ű���� ����, ���� ���� �̺�Ʈ�Դϴ�. </summary>
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