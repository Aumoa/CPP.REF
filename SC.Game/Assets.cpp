using namespace SC;
using namespace SC::Game;

Assets::Assets( String name ) : Object()
	, name( name )
{

}

String Assets::Name_get()
{
	return name;
}