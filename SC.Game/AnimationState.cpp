using namespace SC;
using namespace SC::Game;

using namespace std;

AnimationState::AnimationState( String name ) : Assets( name )
{

}

RefPtr<AnimationClip> AnimationState::Clip_get()
{
	return clip;
}

void AnimationState::Clip_set( RefPtr<AnimationClip> value )
{
	clip = value;
}