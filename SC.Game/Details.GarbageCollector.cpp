using namespace SC;
using namespace SC::Game::Details;

using namespace std;

void GarbageCollector::Add( const object& garbage )
{
	lock_guard<mutex> locker( lock );
	if ( !AppShutdown )
		garbages[GlobalVar.frameIndex].push( garbage );
}

void GarbageCollector::Collect()
{
	lock_guard<mutex> locker( lock );
	while ( garbages[GlobalVar.frameIndex].size() )
	{
		garbages[GlobalVar.frameIndex].pop();
	}
}

void GarbageCollector::CollectAll()
{
	lock_guard<mutex> locker( lock );
	while ( garbages[0].size() )
	{
		garbages[0].pop();
	}

	while ( garbages[1].size() )
	{
		garbages[1].pop();
	}
}