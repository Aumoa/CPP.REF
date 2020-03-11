using namespace SC;
using namespace SC::Diagnostics;

StepTimerCallbackDelegate::StepTimerCallbackDelegate( std::function<void( RefPtr<StepTimer> sender )> callback )
	: callback( callback )
{

}

StepTimerCallbackDelegate::StepTimerCallbackDelegate( const StepTimerCallbackDelegate& copy )
	: callback( copy.callback )
{

}

String StepTimerCallbackDelegate::ToString()
{
	return "SC.Diagnostics.StepTimerCallbackDelegate";
}

bool StepTimerCallbackDelegate::IsValid_get()
{
	if ( callback ) return true;
	else return false;
}

void StepTimerCallbackDelegate::Invoke( RefPtr<StepTimer> sender )
{
	callback( sender );
}

void StepTimerCallbackDelegate::operator()( RefPtr<StepTimer> sender )
{
	callback( sender );
}