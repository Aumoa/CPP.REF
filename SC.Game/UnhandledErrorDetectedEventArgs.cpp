using namespace SC;
using namespace SC::Game;

UnhandledErrorDetectedEventArgs::UnhandledErrorDetectedEventArgs( SC::Exception* e )
{
	this->e = e;
}

UnhandledErrorDetectedEventArgs::~UnhandledErrorDetectedEventArgs()
{
	if ( e )
	{
		e = nullptr;
	}
}

String UnhandledErrorDetectedEventArgs::ToString()
{
	return "SC.Game.Core.UnhandledErrorDetectedEventArgs";
}

Exception* UnhandledErrorDetectedEventArgs::Exception_get()
{
	return e;
}

bool UnhandledErrorDetectedEventArgs::IsCritical_get()
{
	return isCritical;
}

void UnhandledErrorDetectedEventArgs::IsCritical_set( bool value )
{
	isCritical = value;
}