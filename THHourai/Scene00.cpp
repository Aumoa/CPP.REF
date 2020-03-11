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
			script[i]->HelpText = "�ƹ� Ű�� ���� ��� ����";
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
	script[0]->Script[0] = L"�� ������ ���丮�� <color=Orange><u>��ũ����Ʈ3</u></color> ���� <color=Lime><u>\"��������̾߱�\"</u></color>���� �����Խ��ϴ�. ���丮�� �ణ�� ������ �ֽ��ϴ�.";
	script[0]->Script[1] = L"���� ĳ���� <color=DeepPink>\"���ڿ�\"</color>, <color=DeepPink>\"ī����\"</color>�� �������� ����˴ϴ�.";
	script[0]->ScriptEnded += EventHandler<>( this, &Scene00::OnScriptEnded );
	script[0]->HelpText = "�����͸� �ҷ����� ��...";

	script[1] = new OnlyScriptPage( 6 );
	script[1]->Script[0] = L".......";
	script[1]->Script[1] = L"ȯ����, �⹦�� �ɷ��� ���� �ҳ���� �����ϴ� ��. ������ ¢���� �̺��� ����Ű�⵵ ������, �׷����� <color=Orange>��ȭ</color>�� ������ ȯ������ �ֵ� ���ɻ��Դϴ�.";
	script[1]->Script[2] = L"�����, �� ���� �߻��ϱ� ��������.......";
	script[1]->Script[3] = L"<color=Red>����� ��</color>. ��ġ ȯ���� ��ü�� �ĸ��� �����ϵ�, ���� �ð� ������ ��Ȱ�� ������ �ҳ���� ��κ� �ĸ꿡 ���� �η����� �ذ� �־����ϴ�.";
	script[1]->Script[4] = L"��� �ӿ��� ������ ������ �װ�, ���� �ҷκһ翴�� �� �� ����<color=Red>ȣ���̻� ī����</color>�� ��, <color=Red>�����Ͷ�� ���ڿ�</color>�������� ��Ƴ��ҽ��ϴ�.";
	script[1]->Script[5] = L"\"��Ƴ��ƶ�, �׸��� ������ �ҳ���� ã�Ƴ��� �� ������ȯ�������� ���ƿ��� ������!\", �װ��� ���� <color=Red>���</color>�Դϴ�.";
	script[1]->ScriptEnded += EventHandler<>( this, &Scene00::OnScriptEnded );
	script[1]->HelpText = "�����͸� �ҷ����� ��...";

	script[2] = new OnlyScriptPage( 2 );
	script[2]->Script[0] = L".......";
	script[2]->Script[1] = L"����.......";
	script[2]->ScriptEnded += EventHandler<>( this, &Scene00::OnScriptEnded );
	script[2]->HelpText = "�����͸� �ҷ����� ��...";

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