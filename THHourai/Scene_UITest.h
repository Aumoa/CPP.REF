#pragma once

namespace Touhou
{
	class Scene_UITest : public Scene
	{
		RefPtr<UITestPage> page;

	public:
		Scene_UITest();

		void Start() override;
		void Load( RefPtr<IAsyncLoad> asyncLoad ) override;

	private:
		void LoadForUI();
	};
}