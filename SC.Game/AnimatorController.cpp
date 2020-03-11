using namespace SC;
using namespace SC::Game;

using namespace std;

auto AnimatorController::MakeParameters() -> decltype( parameters )
{
	return parameters;
}

AnimatorController::AnimatorController( String name ) : Assets( name )
{

}

void AnimatorController::AddState( RefPtr<AnimationState> state, bool isDefault )
{
	AddState( state->Name.Chars, state, isDefault );
}

void AnimatorController::AddState( const wstring_view& stateNameOverride, RefPtr<AnimationState> state, bool isDefault )
{
	auto it = animationStates.find( stateNameOverride );
	if ( it != animationStates.end() ) it->second = state;
	else
	{
		auto ret = animationStates.insert( { wstring( stateNameOverride ), state } );
		if ( isDefault )
		{
			defaultState = ret.first->first;
		}
	}
}

void AnimatorController::RemoveState( const wstring_view& stateName )
{
	auto it = animationStates.find( stateName );
	if ( it != animationStates.end() ) animationStates.erase( it );
}

void AnimatorController::AddVar( const wstring_view& name, const AnimVar& var )
{
	auto it = parameters.find( name );
	if ( it != parameters.end() ) it->second = var;
	else
	{
		auto ret = parameters.insert( { wstring( name ), var } );
	}
}

void AnimatorController::RemoveVar( const wstring_view& name )
{
	auto it = parameters.find( name );
	if ( it != parameters.end() ) parameters.erase( it );
}

void AnimatorController::AddTransition( const wstring_view& from, const AnimationTransitionCondition& value )
{
	if ( auto it = transConds.find( from ); it == transConds.end() )
	{
		transConds.insert( { wstring( from ), vector{ value } } );
	}
	else
	{
		it->second.push_back( value );
	}
}

void AnimatorController::RemoveTransition( const wstring_view& from, const AnimationTransitionCondition& value )
{
	if ( auto it = transConds.find( from ); it != transConds.end() )
	{
		auto& vec = it->second;

		for ( int i = 0, count = vec.size(); i < count; ++i )
		{
			auto& item = vec[i];
			bool check = true;

			if ( !value.To.empty() )
			{
				if ( item.To == value.To )
				{
					check = check && true;
				}
				else
				{
					check = false;
				}
			}

			if ( !value.VarName.empty() )
			{
				if ( item.VarName == value.VarName )
				{
					check = check && true;
				}
				else
				{
					check = false;
				}
			}
		}
	}
}

RefPtr<AnimationState> AnimatorController::States_get( const std::wstring_view& param0 )
{
	auto it = animationStates.find( param0 );
	if ( it != animationStates.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

AnimVar AnimatorController::Vars_get( const wstring_view& param0 )
{
	auto it = parameters.find( param0 );
	if ( it != parameters.end() )
	{
		return it->second;
	}
	else
	{
		return AnimVar( 0 );
	}
}

wstring_view AnimatorController::DefaultState_get()
{
	return defaultState;
}

vector<AnimationTransitionCondition> AnimatorController::Transition_get( const std::wstring_view& param0 )
{
	if ( auto it = transConds.find( param0 ); it != transConds.end() )
	{
		return it->second;
	}
	else
	{
		auto ret = transConds.insert( { wstring( param0 ), vector<AnimationTransitionCondition>{ } } );
		return ret.first->second;
	}
}