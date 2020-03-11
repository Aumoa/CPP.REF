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
		RefPtr<Exception> exception = new Exception( "\"Scene00\" 장면을 불러오는 중이었습니다.", nextScene->Error.Get() );
		demoPage->HelpText = String::Format( "게임 엔진에서 시작 장면을 로드하는 중 오류가 발생하였습니다.\n예외 메시지:\n{0}", exception );
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