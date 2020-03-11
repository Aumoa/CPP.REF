using namespace SC;
using namespace SC::Game;

using namespace std;

map<string, Keyframes::KeyframePair, less<>> AnimationClip::MakeInterpolateMap()
{
	map<string, Keyframes::KeyframePair, less<>> map;

	for ( auto i : keyframes )
	{
		map.insert( { i.first, Keyframes::KeyframePair::Empty } );
	}

	return move( map );
}

void AnimationClip::Interpolate( double timePos, map<string, Keyframes::KeyframePair, less<>>& interpolated )
{
	auto i = keyframes.begin();
	auto j = interpolated.begin();

	if ( isLoop )
	{
		timePos = fmod( timePos, duration );
	}

	for ( i = keyframes.begin(); i != keyframes.end(); ++i )
	{
		j++->second = i->second.Interpolate( timePos );
	}
}

AnimationClip::AnimationClip( String name ) : Assets( name )
{

}

void AnimationClip::RemoveKeyframes( const string_view& boneName )
{
	auto it = keyframes.find( boneName );
	if ( it != keyframes.end() ) keyframes.erase( it );

	ComputeDur();
}

Keyframes AnimationClip::Keyframes_get( const string_view& param0 )
{
	auto it = keyframes.find( param0 );
	if ( it == keyframes.end() ) return Game::Keyframes();
	else return it->second;
}

void AnimationClip::Keyframes_set( const string_view& param0, const Game::Keyframes& value )
{
	auto it = keyframes.find( param0 );
	if ( it != keyframes.end() ) it->second = value;
	else keyframes.insert( { string( param0 ), value } );

	ComputeDur();
}

void AnimationClip::Keyframes_set( const string_view& param0, Game::Keyframes&& value )
{
	auto it = keyframes.find( param0 );
	if ( it != keyframes.end() )
	{
		it->second = move( value );
	}
	else keyframes.insert( { string( param0 ), move( value ) } );

	ComputeDur();
}

double AnimationClip::Duration_get()
{
	return duration;
}

bool AnimationClip::IsLoop_get()
{
	return isLoop;
}

void AnimationClip::IsLoop_set( bool value )
{
	isLoop = value;
}

void AnimationClip::ComputeDur()
{
	duration = 0;

	for ( auto i : keyframes )
	{
		duration = max( duration, i.second.Duration );
	}
}