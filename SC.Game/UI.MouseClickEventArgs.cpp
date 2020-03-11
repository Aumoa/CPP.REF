using namespace SC;
using namespace SC::Drawing;
using namespace SC::Game::UI;

MouseClickEventArgs::MouseClickEventArgs()
{

}

MouseClickEventArgs::MouseClickEventArgs( MouseButtonType type, bool isDown, Point<int> cursorPos )
	: Button( type )
	, IsDown( isDown )
	, CursorPos( cursorPos )
{

}

MouseClickEventArgs::MouseClickEventArgs( const MouseClickEventArgs& copy )
	: Button( copy.Button )
	, IsDown( copy.IsDown )
	, CursorPos( copy.CursorPos )
{

}

String MouseClickEventArgs::ToString()
{
	return String::Format( "{0} is {1} at {2}", Button, IsDown ? "down" : "up", CursorPos );
}

bool MouseClickEventArgs::Equals( object obj )
{
	if ( MouseClickEventArgs e; obj.Is<MouseClickEventArgs>( &e ) )
	{
		return Equals( e );
	}
	else return false;
}

bool MouseClickEventArgs::Equals( MouseClickEventArgs right )
{
	return Button == right.Button && IsDown == right.IsDown && CursorPos == right.CursorPos;
}

MouseClickEventArgs& MouseClickEventArgs::operator=( const MouseClickEventArgs& copy )
{
	Button = copy.Button;
	IsDown = copy.IsDown;
	CursorPos = copy.CursorPos;
	return *this;
}