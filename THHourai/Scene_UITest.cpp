using namespace Touhou;

using namespace SC::Drawing;

Scene_UITest::Scene_UITest() : Scene()
{

}

void Scene_UITest::Start()
{
	App::Navigate( page );

	Scene::Start();
}

void Scene_UITest::Load( RefPtr<IAsyncLoad> asyncLoad )
{
	LoadForUI();
}

void Scene_UITest::LoadForUI()
{
	page = new UITestPage();
}