using namespace Touhou;

using namespace SC::Threading;

DemoScene::DemoScene() : Scene()
{

}

void DemoScene::Start()
{
	App::Navigate( demoPage );

#if defined( _DEBUG )
	GameVar::pickedCharacterType = 0;
	nextScene = SceneManager::LoadSceneAsync<Scene01>();
#else
	nextScene = SceneManager::LoadSceneAsync<Scene00>();
#endif

	Scene::Start();
}

void DemoScene::Update()
{
	if ( nextScene->Status == AsyncStatus::Error )
	{
		RefPtr<Exception> exception = new Exception( "\"Scene00\" ����� �ҷ����� ���̾����ϴ�.", nextScene->Error.Get() );
		demoPage->HelpText = String::Format( "���� �������� ���� ����� �ε��ϴ� �� ������ �߻��Ͽ����ϴ�.\n���� �޽���:\n{0}", exception );
	}
	else
	{
		if ( demoPage->DemoEnd )
		{
			if ( nextScene->Status == AsyncStatus::Completed )
			{
				SceneManager::StartScene( nextScene );
			}
			else
			{
				App::Navigate( loadingPage );
				loadingPage->AsyncLoad = nextScene;
			}
		}
	}

	Scene::Update();
}

void DemoScene::Load( RefPtr<IAsyncLoad> asyncLoad )
{
	demoPage = new DemoPage();
	loadingPage = new LoadingPage();
}