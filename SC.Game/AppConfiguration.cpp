using namespace SC;
using namespace SC::Game;

AppConfiguration::AppConfiguration()
	: appName()
{

}

AppConfiguration::AppConfiguration( const AppConfiguration& copy )
	: appName( copy.appName )
	, fullscreenMode( copy.fullscreenMode )
	, scaleFactor( copy.scaleFactor )
	, physicsFPS( copy.physicsFPS )
	, disableUI( copy.disableUI )
	, deviceName( copy.deviceName )
	, verticalSync( copy.verticalSync )
{

}

String AppConfiguration::ToString()
{
	return "SC.Game.AppConfiguration";
}

String AppConfiguration::AppName_get()
{
	return appName;
}

void AppConfiguration::AppName_set( String value )
{
	appName = value;
}

bool AppConfiguration::FullscreenMode_get()
{
	return fullscreenMode;
}

void AppConfiguration::FullscreenMode_set( bool value )
{
	fullscreenMode = value;
}

double AppConfiguration::ScaleFactor_get()
{
	return scaleFactor;
}

void AppConfiguration::ScaleFactor_set( double value )
{
	scaleFactor = std::clamp( value, 1.0, 5.0 );
}

int AppConfiguration::PhysicsUpdatePerSeconds_get()
{
	return physicsFPS;
}

void AppConfiguration::PhysicsUpdatePerSeconds_set( int value )
{
	physicsFPS = value;
}

bool AppConfiguration::DisableUI_get()
{
	return disableUI;
}

void AppConfiguration::DisableUI_set( bool value )
{
	disableUI = value;
}

String AppConfiguration::AdapterName_get()
{
	return deviceName;
}

bool AppConfiguration::VSync_get()
{
	return verticalSync;
}

void AppConfiguration::VSync_set( bool value )
{
	verticalSync = value;
}

AppConfiguration& AppConfiguration::operator=( const AppConfiguration& copy )
{
	appName = copy.appName;
	fullscreenMode = copy.fullscreenMode;
	scaleFactor = copy.scaleFactor;
	physicsFPS = copy.physicsFPS;
	disableUI = copy.disableUI;
	deviceName = copy.deviceName;
	verticalSync = copy.verticalSync;
	return *this;
}