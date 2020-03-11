#pragma once

namespace Touhou
{
	class App : public Game::Application
	{
	public:
		App();

		void OnStart() override;
		int OnExit() override;

		static void Main();
		static void Navigate( RefPtr<Game::UI::Page> page );

		static App* Instance();

	private:
		static Game::AppConfiguration GetConfig();
	};
}