using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;
using namespace SC::Threading;

using namespace std;

RefPtr<Scene> SceneManager::currentScene;

void SceneManager::StartScene( RefPtr<IAsyncLoad> sceneAsyncLoad )
{
	if ( sceneAsyncLoad->Status == AsyncStatus::Completed )
	{
		SwitchScene( sceneAsyncLoad->GetResults().As<Scene>() );
	}
}

void SceneManager::LoadScene( RefPtr<Scene> scene )
{
	scene->Load( nullptr );

	SwitchScene( scene );
}

RefPtr<IAsyncLoad> SceneManager::LoadSceneAsync( RefPtr<Scene> scene )
{
	RefPtr asyncLoad = new AsyncLoad();
	asyncLoad->IsLocked = true;
	AsyncLoad* ptr = asyncLoad.Get();
	asyncLoad->Bind( ThreadPool::CreateTask( [ptr]( object arg ) -> object
		{
			RefPtr asyncLoad_ = ptr;
			asyncLoad_->IsLocked = false;
			auto sc = arg.As<Scene>();
			sc->Load( asyncLoad_ );
			return sc;
		}, scene
	) );
	return asyncLoad;
}

void SceneManager::SwitchScene( RefPtr<Scene> scene )
{
	if ( currentScene )
	{
		currentScene->Unload();
	}

	currentScene = scene;
}