#pragma once

namespace SC::Game::UI
{
	/// <summary> �ؽ�Ʈ ���� ����� ǥ���մϴ�. </summary>
	enum class TextAlignment
	{
		/// <summary> �ؽ�Ʈ�� �������� �����մϴ�. </summary>
		Left = 0,

		/// <summary> �ؽ�Ʈ�� ����� �����մϴ�. </summary>
		Center = 1,

		/// <summary> �ؽ�Ʈ�� ���������� �����մϴ�. </summary>
		Right = 2,

		/// <summary> �ؽ�Ʈ�� ���� ���� �����մϴ�. </summary>
		Justified = 3
	};
}

inline std::wostream& operator<<( std::wostream& wout, SC::Game::UI::TextAlignment value )
{
	static constexpr const wchar_t* context[] =
	{
		L"Left",
		L"Center",
		L"Right",
		L"Justified",
	};

	if ( ( int )value >= 0 && ( int )value < 4 )
	{
		return wout << context[( int )value];
	}
	else
	{
		return wout << L"SC.Game.UI.TextAlignment(" << ( int )value << L")";
	}
}