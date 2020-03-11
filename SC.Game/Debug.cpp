using namespace SC;
using namespace SC::Game;

void Debug::Log( object logText )
{
	OutputDebugStringW( logText->ToString().Chars );
	OutputDebugStringW( L"\n" );
}