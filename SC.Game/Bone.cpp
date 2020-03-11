using namespace SC;
using namespace SC::Game;

using namespace std;

Bone::Bone() : Component()
{

}

object Bone::Clone()
{
	RefPtr ptr = new Bone();
	ptr->name = name;
	ptr->index = index;
	ptr->restore = restore;
	return ptr;
}

void Bone::Update( Time& time, Input& input )
{

}

string Bone::Name_get()
{
	return name;
}

void Bone::Name_set( const string_view& value )
{
	name = value;
}

int Bone::Index_get()
{
	return index;
}

void Bone::Index_set( int value )
{
	index = value;
}

bool Bone::Restore_get()
{
	return restore;
}

void Bone::Restore_set( bool value )
{
	restore = value;
}