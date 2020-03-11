#pragma once

namespace Touhou
{
	class Scene01 : public Scene
	{
		RefPtr<ComplexPage> complexPage;

	public:
		Scene01();

		void Start() override;
		void Update() override;

		void Load( RefPtr<IAsyncLoad> asyncLoad ) override;

	private:
		void LoadForUI();
	};
}