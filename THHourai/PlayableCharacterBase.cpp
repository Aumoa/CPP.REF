using namespace Touhou;

PlayableCharacterBase::PlayableCharacterBase( String name, String filename ) : GameObject( name )
	, filename( filename )
{
	AddComponent<MouseEventDispatcher>();
	AddComponent<Animator>();
	{
		var combat = AddComponent<CombatBase>();
		combat->MaxHealthPoint = 500;
		combat->MaxMagicPoint = 10;
		combat->CurrentHealthPoint = 500;
		combat->MaxMagicPoint = 10;
	}
}

void PlayableCharacterBase::LoadAnimatorController()
{
	var controller = new AnimatorController( "AnimatorController" );

	var standClip = AssetBundle::GetItemAs<AnimationClip>( String::Format( "{0}:Stand", filename ) );
	var walkClip = AssetBundle::GetItemAs<AnimationClip>( String::Format( "{0}:Walk", filename ) );

	var standState = new AnimationState( "Stand" );
	standState->Clip = standClip;

	var walkState = new AnimationState( "Walk" );
	walkState->Clip = walkClip;

	controller->AddState( standState, true );
	controller->AddState( walkState );

	AnimationTransitionCondition to_walk( L"Walk", L"Velocity", 0.3, []( AnimVar var ) -> bool
		{
			if ( var.Type == 1 )
			{
				if ( var.Double >= 0.1 )
				{
					return true;
				}
			}
			return false;
		}
	);
	controller->AddTransition( L"Stand", to_walk );

	AnimationTransitionCondition to_stand( L"Stand", L"Velocity", 0.3, []( AnimVar var ) -> bool
		{
			if ( var.Type == 1 )
			{
				if ( var.Double <= 0.9 )
				{
					return true;
				}
			}
			return false;
		}
	);
	controller->AddTransition( L"Walk", to_stand );

	GetComponent<Animator>()->Controller = controller;
}