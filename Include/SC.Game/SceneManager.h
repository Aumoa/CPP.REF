#pragma once

namespace SC::Game
{
	/// <summary> 장면을 관리하는 클래스입니다. </summary>
	class SceneManager abstract final : virtual public Object
	{
		friend class Details::GameLogic;

		static RefPtr<Scene> currentScene;
		
	public:
		/// <summary> 정의된 장면 클래스를 불러옵니다. </summary>
		template< class SceneT, class... Args >
		static void LoadScene( Args&&... args )
		{
			RefPtr scene = new SceneT( args... );
			LoadScene( scene );
		}

		/// <summary> 정의된 장면 클래스를 비동기 상태로 불러옵니다. </summary>
		template< class SceneT, class... Args >
		static RefPtr<IAsyncLoad> LoadSceneAsync( Args&&... args )
		{
			RefPtr scene = new SceneT( args... );
			return LoadSceneAsync( scene );
		}

		/// <summary> 비동기 로드된 장면 개체로 전환합니다. </summary>
		static void StartScene( RefPtr<IAsyncLoad> sceneAsyncLoad );

	private:
		static void LoadScene( RefPtr<Scene> scene );
		static RefPtr<IAsyncLoad> LoadSceneAsync( RefPtr<Scene> scene );
		static void SwitchScene( RefPtr<Scene> scene );
	};
}