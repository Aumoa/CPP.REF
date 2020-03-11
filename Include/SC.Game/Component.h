#pragma once

namespace SC::Game
{
	/// <summary> 게임 개체에 포함되는 구성 요소에 대한 기본 함수를 제공합니다. </summary>
	class Component : virtual public Object, virtual public ICloneable
	{
		friend class GameObject;

		WeakPtr gameObject;

		bool isEnabled = true;
		bool isFirst = true;

	protected:
		/// <summary> <see cref="Component"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		Component();

		/// <summary> (내부 구성 요소) 컴포넌트의 렌더링 작업을 진행합니다. </summary>
		virtual void Render( RefPtr<Details::CDeviceContext>& deviceContext );

	public:
		/// <summary> 컴포넌트가 개체와 연결될 때 호출됩니다. </summary>
		virtual void Awake();

		/// <summary> 연결된 개체가 시작될 때 실행됩니다. </summary>
		virtual void Start();

		/// <summary> 컴포넌트에 대한 논리 갱신을 수행합니다. </summary>
		/// <param name="time"> 게임 시간을 전달합니다. </param>
		/// <param name="input"> 현재 프레임에서 입력 상태를 전달합니다. </param>
		virtual void Update( Time& time, Input& input );

		/// <summary> 컴포넌트에 대한 고정 프레임 논리 갱신을 수행합니다. </summary>
		/// <param name="time"> 게임 시간을 전달합니다. </param>
		virtual void FixedUpdate( Time& time );

		/// <summary> 컴포넌트에 대한 늦은 논리 갱신을 수행합니다. </summary>
		/// <param name="time"> 게임 시간을 전달합니다. </param>
		/// <param name="input"> 현재 프레임에서 입력 상태를 전달합니다. </param>
		virtual void LateUpdate( Time& time, Input& input );

		/// <summary> 연결된 게임 개체에 확장 컴포넌트를 추가합니다. </summary>
		template< class T, class... CtorArgs, std::enable_if_t<where<Component, T>> * = nullptr >
		RefPtr<T> AddComponent( CtorArgs&&... ctorArgs )
		{
			if ( gameObject.IsValid )
			{
				auto go = gameObject.ResolveAs<GameObject>();
				return go->AddComponent<T>( ctorArgs... );
			}
			else
			{
				return nullptr;
			}
		}

		/// <summary> 연결된 게임 개체에 추가된 확장 컴포넌트를 가져옵니다. </summary>
		template< class T >
		RefPtr<T> GetComponent()
		{
			if ( gameObject.IsValid )
			{
				auto go = gameObject.ResolveAs<GameObject>();
				return go->GetComponent<T>();
			}
			else
			{
				return nullptr;
			}
		}

		/// <summary> (Visual Studio 전용) 연결된 게임 개체를 가져옵니다. </summary>
		vs_property_get( RefPtr<GameObject>, Linked );

		/// <summary> 연결된 게임 개체를 가져옵니다. </summary>
		RefPtr<GameObject> Linked_get();

		/// <summary> (Visual Studio 전용) 연결된 게임 개체의 변환 상태를 가져옵니다. </summary>
		vs_property_get( RefPtr<Game::Transform>, Transform );

		/// <summary> 연결된 게임 개체의 변환 상태를 가져옵니다. </summary>
		RefPtr<Game::Transform> Transform_get();

		/// <summary> (Visual Studio 전용) 구성 요소의 갱신 작업(Update 및 FixedUpdate)이 활성화되었는지 결정하는 값을 설정하거나 가져옵니다. </summary>
		vs_property( bool, IsEnabled );

		/// <summary> 구성 요소의 갱신 작업(Update 및 FixedUpdate)이 활성화되었는지 결정하는 값을 가져옵니다. </summary>
		bool IsEnabled_get();

		/// <summary> 구성 요소의 갱신 작업(Update 및 FixedUpdate)이 활성화되었는지 결정하는 값을 설정합니다. </summary>
		void IsEnabled_set( bool value );
	};
}