using namespace SC;
using namespace SC::Game::UI;

DispatcherEventArgs::DispatcherEventArgs( DispatcherEventType type, object args )
	: type( type )
	, args( args )
{

}

DispatcherEventType DispatcherEventArgs::Type_get()
{
	return type;
}

object DispatcherEventArgs::Argument_get()
{
	return args;
}

bool DispatcherEventArgs::IsCompleted_get()
{
	return isCompleted;
}

void DispatcherEventArgs::IsCompleted_set( bool value )
{
	isCompleted = value;
}