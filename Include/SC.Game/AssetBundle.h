#pragma once

namespace SC::Game
{
	/// <summary> ���� ������ �ڻ� ���丮���� ���� ���� �Լ��� �����մϴ�. </summary>
	class AssetBundle abstract : virtual public Object
	{
		static std::multimap<String, RefPtr<Assets>> assets;

	public:
		/// <summary> ��� �ڻ� �����͸� �ܼ� �ݺ��ϴ� ������ ��ü�� �����ɴϴ�. </summary>
		static RefPtr<IEnumerator<RefPtr<Assets>>> GetEnumerator();

		/// <summary> �� �ڻ� �����͸� �߰��մϴ�. </summary>
		static void AddItem( RefPtr<Assets> value );

		/// <summary> ���Ե� �ڻ� �����͸� �����ɴϴ�. </summary>
		/// <param name="name"> �ڻ� �������� �̸��� �����մϴ�. </param>
		static RefPtr<Assets> GetItem( String name );

		/// <summary> ���Ե� �ڻ� �����Ϳ��� ���� �̸��� ���� ��� �׸��� �����ɴϴ�. </summary>
		/// <param name="name"> �ڻ� �������� �̸��� �����մϴ�. </param>
		static RefPtr<IEnumerator<RefPtr<Assets>>> GetItems( String name );

		/// <summary> �ڻ� �����Ͱ� ���ԵǾ� �ִ��� �˻��մϴ�. </summary>
		/// <param name="name"> �ڻ� �������� �̸��� �����մϴ�. </param>
		static bool Contains( String name );

		/// <summary> ���Ե� �ڻ� �����Ϳ��� �������� ��ȯ ������ ��ü�� �����ɴϴ�. </summary>
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

		/// <summary> ���Ե� �ڻ� �����Ϳ��� �������� ��ȯ ������ ��ü�� �����ɴϴ�. </summary>
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

		/// <summary> �� ������ �ҷ��ɴϴ�. </summary>
		/// <param name="filepath"> ���� ��θ� �����մϴ�. </param>
		/// <returns> ������ �ҷ����� �۾��� ���� �񵿱� �۾� ��ü�� ��ȯ�մϴ�. �� ��ü�� ���� ����ڴ� �۾��� ���� ��Ȳ�� Ȯ���ϰ� ��ȯ ���� �����ɴϴ�. </returns>
		static RefPtr<IAsyncLoad> LoadModelAssetsAsync( const std::filesystem::path& filepath );

	private:
		static RefPtr<GameObject> CreateGameObjectFromModel( const std::filesystem::path& filepath, RefPtr<IAsyncLoad> asyncLoad = nullptr );
		static RefPtr<GameObject> CreateGameObjectFromM3DModel( const std::filesystem::path& filepath, RefPtr<IAsyncLoad> asyncLoad = nullptr );
		static RefPtr<GameObject> CreateGameObjectFromMDLModel( const std::filesystem::path& filepath, RefPtr<IAsyncLoad> asyncLoad = nullptr );
		static RefPtr<GameObject> CreateGameObjectFromAssimpModel( const std::filesystem::path& filepath, RefPtr<IAsyncLoad> asyncLoad = nullptr );
	};
}