using namespace Touhou;

Bee::Bee( String name ) : GameObject( name )
{
	LoadAssets();
	LoadAnimatorController();
}

void Bee::LoadAssets()
{
	var model = AssetBundle::LoadModelAssetsAsync( "Assets/Model/Bee/bee.glb" );

	IsLocked = true;
	var go = model->GetResults().As<GameObject>();
	go->Transform->Scale = Vector3( 0.03, 0.03, 0.03 );
	go->Transform->Rotation = Quaternion::AngleAxis( 3.14159, Vector3::Up );
	go->Parent = this;
	IsLocked = false;
}

void Bee::LoadAnimatorController()
{

}