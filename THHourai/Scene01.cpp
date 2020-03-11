using namespace Touhou;

using namespace SC::Threading;

using namespace std;

Scene01::Scene01() : Scene()
{

}

void Scene01::Start()
{
	App::Navigate( complexPage );

	var quaterViewCam = MainCamera::GetInstance();
	{
		auto trp = quaterViewCam->Transform;
		trp->Position = Vector3( 0, 8, -7 );
		trp->LookAt( Vector3::Zero );
	}
	quaterViewCam->AddComponent<KeyboardMovement>();

	Scene::Start();
}

void Scene01::Update()
{
	Scene::Update();
}

void Scene01::Load( RefPtr<IAsyncLoad> asyncLoad )
{
	LoadForUI();

	/* 쿼터뷰 카메라 */
	Add( MainCamera::GetInstance() );

	/* 기본 조명 */
	declare_element( GameObject, directionalLight );
	{
		auto trp = directionalLight->Transform;
		trp->Position = Vector3( -8, 16, 8 );
		trp->LookAt( Vector3::Zero );
	}
	{
		auto dl = directionalLight->AddComponent<DirectionalLight>();
		dl->Ambient = 0.2;
		dl->Diffuse = 0.8;
		dl->Specular = 1.5;
		dl->IsShadowCast = true;
		dl->Width = 30;
		dl->Height = 30;
		dl->MaxDepth = 100;
	}
	Add( directionalLight );

	/* 게임 캐릭터 로드 */
	if ( !GameVar::playableCharacter )
	{
		switch ( GameVar::pickedCharacterType )
		{
		case 0:
			GameVar::playableCharacter = new Kaguya( "playableCharacter" );
			break;
		case 1:
			GameVar::playableCharacter = new Mokou( "playableCharacter" );
			break;
		}
	}
	Add( GameVar::playableCharacter );

	/* 지형 로드 */
	Add( new Scene0_Terrain( "scene0_Terrain" ) );
}

void Scene01::LoadForUI()
{
	complexPage = new ComplexPage();
}