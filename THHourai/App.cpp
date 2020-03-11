using namespace Touhou;

using namespace SC::Game;
using namespace SC::Game::UI;
using namespace SC::Game::Core;

namespace
{
	RefPtr<Canvas> frame;
	App* inst;
}

App::App()
	: Application( GetConfig() )
{
	::inst = this;
}

void App::OnStart()
{
	::frame = Frame;

	var textureTemp = new Texture2D( "templeBase01_diffuse", "Assets/Texture/templeBase01_diffuse.jpg" );

	SceneManager::LoadScene<DemoScene>();
}

int App::OnExit()
{
	return 0;
}

void App::Main()
{
	App::Start( new App() );
}

void App::Navigate( RefPtr<Page> page )
{
	::frame->Navigate( page );
}

App* App::Instance()
{
	return inst;
}

AppConfiguration App::GetConfig()
{
	AppConfiguration config;
	config.AppName = "Touhou Hourai";
	config.PhysicsUpdatePerSeconds = 60;
	config.ScaleFactor = 1.0;
	config.VSync = true;
	return config;
}