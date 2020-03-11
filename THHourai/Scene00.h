#pragma once

namespace Touhou
{
	class Scene00 : public Scene
	{
		static constexpr const int NumScripts = 3;

		RefPtr<OnlyScriptPage> script[NumScripts];
		int currentScript = 0;
		bool loadCompleted = false;

		RefPtr<CharacterPickPage> pickPage;
		RefPtr<LoadingPage> loadingPage;

		RefPtr<GameObject> plane;
		RefPtr<IAsyncLoad> kaguyaLoader;
		RefPtr<IAsyncLoad> mokouLoader;
		RefPtr<GameObject> pivot;

		RefPtr<GameObject> onPlane;
		int picked = -1;

		RefPtr<IAsyncLoad> nextScene;

		std::vector<RefPtr<SkinnedMeshRenderer>> renderers;

	public:
		Scene00();

		void Start() override;
		void Update() override;

		void Load( RefPtr<IAsyncLoad> asyncLoad ) override;

	private:
		void LoadForUI();

	private:
		void OnScriptEnded( object sender );
		void OnPicked( object sender, int pickId );
		void OnChoice( object sender );
		void OnFinish( object sender );
	};
}