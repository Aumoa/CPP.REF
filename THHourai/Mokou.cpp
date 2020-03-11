using namespace Touhou;

Mokou::Mokou( String name ) : PlayableCharacterBase( name, "Mokou.mdl" )
{
	LoadAssets();
	LoadAnimatorController();

	{
		auto movement = AddComponent<Movement>();
		movement->Velocity = 3.0;
	}
}

void Mokou::LoadAssets()
{
	ayaOriginal = AssetBundle::LoadModelAssetsAsync( "Assets/Model/Mokou/Mokou.mdl" )->GetResults().As<GameObject>();
	ayaOriginal->RemoveComponent<Animator>();
	
	IsLocked = true;
	ayaOriginal->Transform->Scale = Vector3( 0.01, 0.01, 0.01 );
	ayaOriginal->Parent = this;
	IsLocked = false;
}