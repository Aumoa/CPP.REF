using namespace SC;
using namespace SC::Drawing;
using namespace SC::Game::UI;

MouseMoveEventArgs::MouseMoveEventArgs()
{

}

MouseMoveEventArgs::MouseMoveEventArgs( Point<int> absCursorPos, Point<int> deltaCursorPos )
	: AbsCursorPos( absCursorPos )
	, DeltaCursorPos( deltaCursorPos )
{

}

MouseMoveEventArgs::MouseMoveEventArgs( Point<int> deltaScroll )
	: DeltaScroll( deltaScroll )
{

}

MouseMoveEventArgs::MouseMoveEventArgs( const MouseMoveEventArgs& copy )
	: AbsCursorPos( copy.AbsCursorPos )
	, DeltaCursorPos( copy.DeltaCursorPos )
	, DeltaScroll( copy.DeltaScroll )
{

}

String MouseMoveEventArgs::ToString()
{
	return String::Format( L"MouseMoveEvent(Cursor: {0}, CursorDelta: {1}, ScrollDelta: {2})", AbsCursorPos, DeltaCursorPos, DeltaScroll );
}

bool MouseMoveEventArgs::Equals( object obj )
{
	if ( MouseMoveEventArgs e; obj.Is<MouseMoveEventArgs>( &e ) )
	{
		return e.Equals( obj );
	}
	return false;
}

bool MouseMoveEventArgs::Equals( MouseMoveEventArgs right )
{
	return AbsCursorPos.Equals( right.AbsCursorPos )
		&& DeltaCursorPos.Equals( right.DeltaCursorPos )
		&& DeltaScroll.Equals( right.DeltaScroll );
}

MouseMoveEventArgs& MouseMoveEventArgs::operator=( const MouseMoveEventArgs& copy )
{
	AbsCursorPos = copy.AbsCursorPos;
	DeltaCursorPos = copy.DeltaCursorPos;
	DeltaScroll = copy.DeltaScroll;
	return *this;
}