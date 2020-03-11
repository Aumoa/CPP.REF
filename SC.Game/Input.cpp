using namespace SC;
using namespace SC::Game;
using namespace SC::Drawing;

bool Input::locked;
Point<int> Input::lockedCursor;

Input::Input()
{

}

Input::Input( const Input& copy )
	: prevCursorPos( copy.prevCursorPos )
	, cursorPos( copy.cursorPos )
	, prevKeyboardState( copy.prevKeyboardState )
	, keyboardState( copy.keyboardState )
{

}

bool Input::IsKeyDown( KeyCode keyCode )
{
	auto code = ( int )keyCode;
	if ( IsKeyChanged( keyCode ) && keyboardState[code] )
	{
		return true;
	}
	return false;
}

bool Input::IsKeyUp( KeyCode keyCode )
{
	auto code = ( int )keyCode;
	if ( IsKeyChanged( keyCode ) && !keyboardState[code] )
	{
		return true;
	}
	return false;
}

void Input::LockCursor()
{
	locked = true;
	ShowCursor( FALSE );

	POINT point;
	GetCursorPos( &point );
	ScreenToClient( Details::GlobalVar.hWnd, &point );

	lockedCursor.X = point.x;
	lockedCursor.Y = point.y;
	SetCursorToCenter();
}

void Input::UnlockCursor( bool restore )
{
	locked = false;

	if ( restore )
	{
		POINT point = { lockedCursor.X, lockedCursor.Y };
		ClientToScreen( Details::GlobalVar.hWnd, &point );
		SetCursorPos( point.x, point.y );
	}

	ShowCursor( TRUE );
}

bool Input::KeyState_get( KeyCode param0 )
{
	return keyboardState[( int )param0];
}

Point<int> Input::MousePosition_get()
{
	return cursorPos;
}

Point<int> Input::DeltaMousePosition_get()
{
	return cursorPos - prevCursorPos;
}

Point<double> Input::DeltaScroll_get()
{
	return scrollDelta;
}

bool Input::CursorLocked_get()
{
	return locked;
}

Input& Input::operator=( const Input& copy )
{
	prevCursorPos = copy.prevCursorPos;
	cursorPos = copy.cursorPos;
	prevKeyboardState = copy.prevKeyboardState;
	keyboardState = copy.keyboardState;
	return *this;
}

bool Input::IsKeyChanged( KeyCode keyCode )
{
	auto code = ( int )keyCode;
	return keyboardState[code] != prevKeyboardState[code];
}

void Input::SetCursorToCenter()
{
	POINT point;

	RECT rect;

	GetClientRect( Details::GlobalVar.hWnd, &rect );
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	point.x = width / 2;
	point.y = height / 2;
	ClientToScreen( Details::GlobalVar.hWnd, &point );

	SetCursorPos( point.x, point.y );
}