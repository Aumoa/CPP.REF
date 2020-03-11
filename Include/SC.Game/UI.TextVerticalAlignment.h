#pragma once

namespace SC::Game::UI
{
	/// <summary> �ؽ�Ʈ ���� ���� ����� ǥ���մϴ�. </summary>
	enum class TextVerticalAlignment
	{
		/// <summary> �ؽ�Ʈ�� �������� �����մϴ�. </summary>
		Top = 0,

		/// <summary> �ؽ�Ʈ�� ����� �����մϴ�. </summary>
		Center = 1,

		/// <summary> �ؽ�Ʈ�� �Ʒ������� �����մϴ�. </summary>
		Bottom = 2
	};
}

inline std::wostream& operator<<( std::wostream& wout, SC::Game::UI::TextVerticalAlignment value )
{
	static constexpr const wchar_t* context[] =
	{
		L"Top",
		L"Center",
		L"Bottom",
	};

	if ( ( int )value >= 0 && ( int )value < 3 )
	{
		return wout << context[( int )value];
	}
	else
	{
		return wout << L"SC.Game.UI.TextVerticalAlignment(" << ( int )value << L")";
	}
}