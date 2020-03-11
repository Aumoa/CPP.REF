using namespace Touhou;

Kaguya::Kaguya( String name ) : PlayableCharacterBase( name, "Kaguya.mdl" )
{
	LoadAssets();
	LoadAnimatorController();

	{
		auto movement = AddComponent<Movement>();
		movement->Velocity = 3.0;
	}
}

void Kaguya::LoadAssets()
{
	ayaOriginal = AssetBundle::LoadModelAssetsAsync( "Assets/Model/Kaguya/Kaguya.mdl" )->GetResults().As<GameObject>();
	ayaOriginal->RemoveComponent<Animator>();

	IsLocked = true;
	ayaOriginal->Transform->Scale = Vector3( 0.01, 0.01, 0.01 );
	ayaOriginal->Parent = this;
	IsLocked = false;
}