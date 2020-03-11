#pragma once

namespace SC::Game
{
	/// <summary> 게임 입력 시스템에서 사용하는 가상 키 코드를 표현합니다. </summary>
	enum class KeyCode
	{
		/// <summary> 키가 눌리지 않은 상태를 나타냅니다. </summary>
		None,
		Mouse0 = 0x01, Mouse1, Cancel, Mouse2, Mouse3, Mouse4, Mouse5 /* Reserved Mouse5 in Windows */,
		Backspace = 0x08, Tab,
		Clear = 0x0C, Return,
		Menu = 0x12, Pause, CapsLock,
		Escape = 0x1B,
		Space = 0x20, PageUp, PageDown, End, Home, LeftArrow, UpArrow, RightArrow, DownArrow,
		Print = 0x2C, Insert, Delete, Help,
		Alpha0 = 0x30, Alpha1, Alpha2, Alpha3, Alpha4, Alpha5, Alpha6, Alpha7, Alpha8, Alpha9,
		A = 0x41, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		Keypad0 = 0x60, Keypad1, Keypad2, Keypad3, Keypad4, Keypad5, Keypad6, Keypad7, Keypad8, Keypad9,
		KeypadMultiply = 0x6A, KeypadPlus,
		KeypadMinus = 0x6D, KeypadPeriod, KeypadDivide,
		F1 = 0x70, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15,
		NumLock = 0x90, ScrollLock,
		LeftShift = 0xA0, RightShift, LeftControl, RightControl,
		Semicolon = 0xBA, Plus, Comma, Minus, Period, Slash, GraveAccent,
		LeftBracket = 0xDB, Backslash, RightBracket, Quote
	};
}

inline std::wostream& operator<<( std::wostream& wout, SC::Game::KeyCode value )
{
	static constexpr const wchar_t* context[] =
	{
		L"None",
		L"Mouse0", L"Mouse1", L"Cancel", L"Mouse2", L"Mouse3", L"Mouse4", L"Mouse5",
		L"Backspace", L"Tab", nullptr, nullptr,
		L"Clear", L"Return", nullptr, nullptr, nullptr, nullptr,
		L"Menu", L"Pause", L"CapsLock", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		L"Escape", nullptr, nullptr, nullptr, nullptr,
		L"Space", L"PageUp", L"PageDown", L"End", L"Home", L"LeftArrow", L"UpArrow", L"RightArrow", L"DownArrow", nullptr, nullptr, nullptr,
		L"Print", L"Insert", L"Delete", L"Help",
		L"Alpha0", L"Alpha1", L"Alpha2", L"Alpha3", L"Alpha4", L"Alpha5", L"Alpha6", L"Alpha7", L"Alpha8", L"Alpha9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I", L"J", L"K", L"L", L"M", L"N", L"O", L"P", L"Q", L"R", L"S", L"T", L"U", L"V", L"W", L"X", L"Y", L"Z"
	};

	if ( ( int )value >= 0 && ( int )value < sizeof( context ) / sizeof( context[0] ) )
	{
		return wout << context[( int )value];
	}
	else
	{
		return wout << L"SC.Game.KeyCode(" << ( int )value << L")";
	}
}