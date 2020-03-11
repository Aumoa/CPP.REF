#pragma once

namespace SC::Game
{
	/// <summary> 게임 엔진에서 모든 게임 항목에 대한 기본 클래스를 나타냅니다. </summary>
	class GameObject : public Assets, virtual public IEnumerable<RefPtr<GameObject>>, virtual public ICloneable
	{
		friend class Scene;

		using ComponentPair = std::pair<std::size_t, RefPtr<Component>>;
		std::vector<ComponentPair> components;
		RefPtr<Transform> transform;

		std::vector<RefPtr<GameObject>> gameObjects;
		WeakPtr parent;

	public:
		/// <summary> <see cref="GameObject"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 개체 이름을 전달합니다. </param>
		GameObject( String name = "SC.Game.GameObject" );

		/// <summary> (<see cref="IEnumerable"/> 인터페이스에서 구현 됨.) 자식 개체 전체를 반복하는 열거자를 반환합니다. </summary>
		RefPtr<IEnumerator<RefPtr<GameObject>>> GetEnumerator() override;

		/// <summary> (<see cref="ICloneable"/>) 현재 인스턴스의 복사본인 새 개체를 생성합니다. </summary>
		object Clone() override;

		/// <summary> 장면이 시작될 때 장면에 포함된 모든 게임 개체에 실행됩니다. </summary>
		virtual void Start();

		/// <summary> 게임 개체에 대한 논리 갱신을 수행합니다. </summary>
		/// <param name="time"> 게임 시간을 전달합니다. </param>
		/// <param name="input"> 현재 프레임에서 입력 상태를 전달합니다. </param>
		virtual void Update( Time& time, Input& input );

		/// <summary> 게임 개체에 대한 고정 프레임 논리 갱신을 수행합니다. </summary>
		/// <param name="time"> 게임 시간을 전달합니다. </param>
		virtual void FixedUpdate( Time& time );

		/// <summary> 게임 개체에 대한 늦은 논리 갱신을 수행합니다. </summary>
		/// <param name="time"> 게임 시간을 전달합니다. </param>
		/// <param name="input"> 현재 프레임에서 입력 상태를 전달합니다. </param>
		virtual void LateUpdate( Time& time, Input& input );

		/// <summary> 개체에 확장 컴포넌트를 추가합니다. </summary>
		template< class T, class... CtorArgs, std::enable_if_t<where<Component, T>>* = nullptr >
		RefPtr<T> AddComponent( CtorArgs&&... ctorArgs )
		{
			RefPtr value = new T( ctorArgs... );
			AddComponent( typeid( T ).hash_code(), value );
			return value;
		}

		/// <summary> 개체에 추가된 확장 컴포넌트를 가져옵니다. </summary>
		template< class T >
		RefPtr<T> GetComponent()
		{
			return GetComponent( typeid( T ).hash_code(), []( Component* ptr ) -> auto
				{
					T* dynamic = dynamic_cast< T* >( ptr );
					return dynamic != nullptr;
				}
			).TryAs<T>();
		}

		/// <summary> 개체에 추가된 확장 컴포넌트를 제거합니다. </summary>
		template< class T >
		void RemoveComponent()
		{
			RemoveComponent( typeid( T ).hash_code(), []( Component* c ) -> bool
				{
					return dynamic_cast< T* >( c ) != nullptr;
				}
			);
		}

		/// <summary> 자식 요소까지 포함하여 개체에 추가된 확장 컴포넌트 목록을 가져옵니다. </summary>
		template< class T >
		std::vector<RefPtr<T>> GetComponentsInChildren()
		{
			using namespace std;

			vector<RefPtr<T>> components;
			// for each childs.
			if ( auto c = GetComponent<T>(); c.IsValid )
			{
				components.push_back( c );
			}

			for ( int i = 0; i < NumChilds_get(); ++i )
			{
				auto vec = Childs_get( i )->GetComponentsInChildren<T>();
				components.insert( components.end(), vec.begin(), vec.end() );
			}

			return move( components );
		}

		/// <summary> (Visual Studio 전용) 개체의 변환 상태를 가져옵니다. </summary>
		vs_property_get( RefPtr<Game::Transform>, Transform );

		/// <summary> 개체의 변환 상태를 가져옵니다. </summary>
		RefPtr<Game::Transform> Transform_get();

		/// <summary> (Visual Studio 전용) 개체의 부모를 설정하거나 가져옵니다. </summary>
		vs_property( RefPtr<GameObject>, Parent );

		/// <summary> 개체의 부모를 가져옵니다. </summary>
		RefPtr<GameObject> Parent_get();

		/// <summary> 개체의 부모를 설정합니다. </summary>
		void Parent_set( RefPtr<GameObject> value );

		/// <summary> (Visual Studio 전용) 개체가 포함하고 있는 자식 개체의 개수를 가져옵니다. </summary>
		vs_property_get( int, NumChilds );

		/// <summary> 개체가 포함하고 있는 자식 개체의 개수를 가져옵니다. </summary>
		int NumChilds_get();

		/// <summary> (Visual Studio 전용) 개체가 포함하고 있는 자식 개체를 가져옵니다. </summary>
		vs_property_get( RefPtr<GameObject>, Childs )[];
		
		/// <summary> 개체가 포함하고 있는 자식 개체를 가져옵니다. </summary>
		RefPtr<GameObject> Childs_get( int param0 );

	private:
		void Render( RefPtr<Details::CDeviceContext>& deviceContext );
		void AddComponent( std::size_t type_hash, RefPtr<Component> component );
		RefPtr<Component> GetComponent( std::size_t type_hash, std::function<bool( Component* )> caster );
		void RemoveComponent( std::size_t type_hash, std::function<bool( Component* )> caster );
	};
}