#pragma once

namespace SC::Game
{
	/// <summary> ����� �����ϴ� Ŭ�����Դϴ�. </summary>
	class SceneManager abstract final : virtual public Object
	{
		friend class Details::GameLogic;

		static RefPtr<Scene> currentScene;
		
	public:
		/// <summary> ���ǵ� ��� Ŭ������ �ҷ��ɴϴ�. </summary>
		template< class SceneT, class... Args >
		static void LoadScene( Args&&... args )
		{
			RefPtr scene = new SceneT( args... );
			LoadScene( scene );
		}

		/// <summary> ���ǵ� ��� Ŭ������ �񵿱� ���·� �ҷ��ɴϴ�. </summary>
		template< class SceneT, class... Args >
		static RefPtr<IAsyncLoad> LoadSceneAsync( Args&&... args )
		{
			RefPtr scene = new SceneT( args... );
			return LoadSceneAsync( scene );
		}

		/// <summary> �񵿱� �ε�� ��� ��ü�� ��ȯ�մϴ�. </summary>
		static void StartScene( RefPtr<IAsyncLoad> sceneAsyncLoad );

	private:
		static void LoadScene( RefPtr<Scene> scene );
		static RefPtr<IAsyncLoad> LoadSceneAsync( RefPtr<Scene> scene );
		static void SwitchScene( RefPtr<Scene> scene );
	};
}