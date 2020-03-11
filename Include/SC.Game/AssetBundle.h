#pragma once

namespace SC::Game
{
	/// <summary> 게임 데이터 자산 스토리지에 대한 접근 함수를 제공합니다. </summary>
	class AssetBundle abstract : virtual public Object
	{
		static std::multimap<String, RefPtr<Assets>> assets;

	public:
		/// <summary> 모든 자산 데이터를 단순 반복하는 열거자 개체를 가져옵니다. </summary>
		static RefPtr<IEnumerator<RefPtr<Assets>>> GetEnumerator();

		/// <summary> 새 자산 데이터를 추가합니다. </summary>
		static void AddItem( RefPtr<Assets> value );

		/// <summary> 포함된 자산 데이터를 가져옵니다. </summary>
		/// <param name="name"> 자산 데이터의 이름을 제공합니다. </param>
		static RefPtr<Assets> GetItem( String name );

		/// <summary> 포함된 자산 데이터에서 같은 이름을 가진 모든 항목을 가져옵니다. </summary>
		/// <param name="name"> 자산 데이터의 이름을 제공합니다. </param>
		static RefPtr<IEnumerator<RefPtr<Assets>>> GetItems( String name );

		/// <summary> 자산 데이터가 포함되어 있는지 검사합니다. </summary>
		/// <param name="name"> 자산 데이터의 이름을 제공합니다. </param>
		static bool Contains( String name );

		/// <summary> 포함된 자산 데이터에서 형식으로 변환 가능한 개체를 가져옵니다. </summary>
		template< class T >
		static RefPtr<T> GetItemAs( String name )
		{
			auto allCollect = GetItems( name );

			for ( auto i : allCollect )
			{
				if ( auto as = i.TryAs<T>(); as )
				{
					return as;
				}
			}

			return nullptr;
		}

		/// <summary> 포함된 자산 데이터에서 형식으로 변환 가능한 개체를 가져옵니다. </summary>
		template< class T >
		static RefPtr<T> GetItemsAs( String name )
		{
			auto allCollect = GetItems( name );
			std::list<RefPtr<T>> collection;

			for ( auto i : allCollect )
			{
				if ( auto as = i.TryAs<T>(); as )
				{
					collection.push_back( as );
				}
			}

			return new BasicEnumerator<RefPtr<T>>( collection.begin(), collection.end(), std::move( collection ) );
		}

		/// <summary> 모델 파일을 불러옵니다. </summary>
		/// <param name="filepath"> 파일 경로를 전달합니다. </param>
		/// <returns> 파일을 불러오는 작업에 대한 비동기 작업 개체를 반환합니다. 이 개체를 통해 사용자는 작업의 진행 상황을 확인하고 반환 값을 가져옵니다. </returns>
		static RefPtr<IAsyncLoad> LoadModelAssetsAsync( const std::filesystem::path& filepath );

	private:
		static RefPtr<GameObject> CreateGameObjectFromModel( const std::filesystem::path& filepath, RefPtr<IAsyncLoad> asyncLoad = nullptr );
		static RefPtr<GameObject> CreateGameObjectFromM3DModel( const std::filesystem::path& filepath, RefPtr<IAsyncLoad> asyncLoad = nullptr );
		static RefPtr<GameObject> CreateGameObjectFromMDLModel( const std::filesystem::path& filepath, RefPtr<IAsyncLoad> asyncLoad = nullptr );
		static RefPtr<GameObject> CreateGameObjectFromAssimpModel( const std::filesystem::path& filepath, RefPtr<IAsyncLoad> asyncLoad = nullptr );
	};
}