using namespace SC;
using namespace SC::Drawing;
using namespace SC::Game;
using namespace SC::Game::UI;

KeyboardEventArgs::KeyboardEventArgs()
{

}

KeyboardEventArgs::KeyboardEventArgs( KeyCode key, bool isDown )
	: Key( key )
	, IsDown( isDown )
{

}

KeyboardEventArgs::KeyboardEventArgs( const KeyboardEventArgs& copy )
	: Key( copy.Key )
	, IsDown( copy.IsDown )
{

}

String KeyboardEventArgs::ToString()
{
	return String::Format( "{0} is ", Key, IsDown ? "down" : "up" );
}

bool KeyboardEventArgs::Equals( object obj )
{
	if ( MouseClickEventArgs e; obj.Is<MouseClickEventArgs>( &e ) )
	{
		return Equals( e );
	}
	else return false;
}

bool KeyboardEventArgs::Equals( KeyboardEventArgs right )
{
	return Key == right.Key && IsDown == right.IsDown;
}

KeyboardEventArgs& KeyboardEventArgs::operator=( const KeyboardEventArgs& copy )
{
	Key = copy.Key;
	IsDown = copy.IsDown;
	return *this;
}