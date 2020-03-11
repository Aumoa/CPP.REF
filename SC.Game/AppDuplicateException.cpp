using namespace SC;
using namespace SC::Game;

AppDuplicateException::AppDuplicateException( Exception* innerException )
	: Exception( "App has been duplicated.", innerException, "SC.Game.Core.AppDuplicateException" )
{

}