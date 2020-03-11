#pragma once

namespace Touhou
{
	class DemoScene : public Scene
	{
		RefPtr<DemoPage> demoPage;
		RefPtr<LoadingPage> loadingPage;

		RefPtr<IAsyncLoad> nextScene;

	public:
		DemoScene();

		void Start() override;
		void Update() override;
		void Load( RefPtr<IAsyncLoad> asyncLoad ) override;
	};
}