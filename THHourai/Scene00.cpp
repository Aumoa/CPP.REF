using namespace Touhou;

using namespace SC::Threading;
using namespace SC::Drawing;

using namespace std;

Scene00::Scene00() : Scene()
{

}

void Scene00::Start()
{
	App::Navigate( script[0] );

	Scene::Start();
}

void Scene00::Update()
{
	if ( onPlane && renderers.empty() ) renderers = onPlane->GetComponentsInChildren<SkinnedMeshRenderer>();

	if ( loadingPage->AsyncLoad )
	{
		if ( nextScene->Status == AsyncStatus::Completed )
		{
			SceneManager::StartScene( nextScene );
		}
	}

	if ( !loadCompleted && kaguyaLoader->Status == AsyncStatus::Completed && mokouLoader->Status == AsyncStatus::Completed )
	{
		for ( int i = currentScript; i < NumScripts; ++i )
		{
			script[i]->HelpText = "아무 키나 눌러 계속 진행";
		}
		loadCompleted = true;
	}

	Scene::Update();
}

void Scene00::Load( RefPtr<IAsyncLoad> asyncLoad )
{
	LoadForUI();

	kaguyaLoader = AssetBundle::LoadModelAssetsAsync( "Assets/Model/Kaguya/Kaguya.mdl" );
	mokouLoader = AssetBundle::LoadModelAssetsAsync( "Assets/Model/Mokou/Mokou.mdl" );

	auto camera = MainCamera::GetInstance();
	camera->Transform->Position = Vector3( -6, 3, 6 );
	camera->Transform->LookAt( Vector3( -4, 0, 4 ) );
	Add( camera );

	declare_element( GameObject, dlight );
	auto dl = dlight->AddComponent<DirectionalLight>();
	dl->IsShadowCast = true;
	dl->Width = 15;
	dl->Height = 15;
	dl->MaxDepth = 50;
	dl->Ambient = 0.2;
	dl->Diffuse = 0.4;
	dlight->Transform->Position = Vector3( 2, 10, 2 );
	dlight->Transform->LookAt( Vector3::Zero );
	Add( dlight );

	declare_element( Material, ground );
	ground->DiffuseMap = new Texture2D( "plane_texture_diffuse", "Assets/Texture/templeBase01_diffuse.jpg" );
	ground->NormalMap = new Texture2D( "plane_texture_normal", "Assets/Texture/templeBase01_normal.jpg" );

	declare_element( GameObject, plane );
	plane->AddComponent<MeshFilter>()->Mesh = Mesh::CreatePlane( "plane000", 5.0f, 5.0f );
	plane->AddComponent<MeshRenderer>()->Material = ground;
	plane->Transform->Scale = Vector3( 10, 1, 10 );
	var base = new GameObject( "plane" );
	plane->Parent = base;
	this->plane = base;
	var pivot = new GameObject( "pivot" );
	pivot->Transform->Position = Vector3( -4.5, 0, 4.5 );
	pivot->Parent = base;
	this->pivot = pivot;
}

void Scene00::LoadForUI()
{
	script[0] = new OnlyScriptPage( 2 );
	script[0]->Script[0] = L"이 게임의 스토리는 <color=Orange><u>워크래프트3</u></color> 맵인 <color=Lime><u>\"동방봉래이야기\"</u></color>에서 가져왔습니다. 스토리에 약간의 변경이 있습니다.";
	script[0]->Script[1] = L"게임 캐릭터 <color=DeepPink>\"모코우\"</color>, <color=DeepPink>\"카구야\"</color>의 시점으로 진행됩니다.";
	script[0]->ScriptEnded += EventHandler<>( this, &Scene00::OnScriptEnded );
	script[0]->HelpText = "데이터를 불러오는 중...";

	script[1] = new OnlyScriptPage( 6 );
	script[1]->Script[0] = L".......";
	script[1]->Script[1] = L"환상향, 기묘한 능력을 지닌 소녀들이 거주하는 땅. 때때로 짖궂은 이변을 일으키기도 하지만, 그럼에도 <color=Orange>평화</color>는 언제나 환상향의 주된 관심사입니다.";
	script[1]->Script[2] = L"어느날, 그 일이 발생하기 전까지는.......";
	script[1]->Script[3] = L"<color=Red>용신의 힘</color>. 마치 환상향 자체가 파멸을 갈망하듯, 오랜 시간 낙원의 생활을 지내던 소녀들은 대부분 파멸에 대한 두려움을 잊고 있었습니다.";
	script[1]->Script[4] = L"재앙 속에서 수많은 생명이 죽고, 오직 불로불사였던 두 명 ――<color=Red>호라이산 카구야</color>와 나, <color=Red>후지와라노 모코우</color>――만이 살아남았습니다.";
	script[1]->Script[5] = L"\"살아남아라, 그리고 전생한 소녀들을 찾아내서 이 곳――환상향으로 돌아오게 만들자!\", 그것이 나의 <color=Red>사명</color>입니다.";
	script[1]->ScriptEnded += EventHandler<>( this, &Scene00::OnScriptEnded );
	script[1]->HelpText = "데이터를 불러오는 중...";

	script[2] = new OnlyScriptPage( 2 );
	script[2]->Script[0] = L".......";
	script[2]->Script[1] = L"나는.......";
	script[2]->ScriptEnded += EventHandler<>( this, &Scene00::OnScriptEnded );
	script[2]->HelpText = "데이터를 불러오는 중...";

	pickPage = new CharacterPickPage();
	pickPage->Picked += EventHandler<int>( this, &Scene00::OnPicked );
	pickPage->Choice += EventHandler<>( this, &Scene00::OnChoice );
	pickPage->Finish += EventHandler<>( this, &Scene00::OnFinish );

	loadingPage = new LoadingPage();
}

void Scene00::OnScriptEnded( object sender )
{
	if ( currentScript == NumScripts - 1 )
	{
		if ( loadCompleted )
		{
			currentScript = NumScripts;

			App::Navigate( pickPage );
			Add( plane );
		}
	}
	else
	{
		App::Navigate( script[++currentScript] );
	}
}

void Scene00::OnPicked( object sender, int pickId )
{
	if ( pickId != picked )
	{
		if ( onPlane ) onPlane->Parent = nullptr;

		if ( pickId == 0 )
		{
			onPlane = kaguyaLoader->GetResults().As<GameObject>()->Clone().As<GameObject>();
			onPlane->Transform->Scale = Vector3( 0.01, 0.01, 0.01 );
			picked = pickId;
		}
		else if ( pickId == 1 )
		{
			onPlane = mokouLoader->GetResults().As<GameObject>()->Clone().As<GameObject>();
			onPlane->Transform->Scale = Vector3( 0.01, 0.01, 0.01 );
			picked = pickId;
		}
		else
		{
			onPlane = new GameObject( "Dummy" );
			picked = -1;
		}
		onPlane->Parent = pivot;
	}
}

void Scene00::OnChoice( object sender )
{
	nextScene = SceneManager::LoadSceneAsync<Scene01>();
	GameVar::pickedCharacterType = picked;
}

void Scene00::OnFinish( object sender )
{
	if ( nextScene->Status == AsyncStatus::Completed )
	{
		SceneManager::StartScene( nextScene );
	}
	else
	{
		loadingPage->AsyncLoad = nextScene;
	}
}