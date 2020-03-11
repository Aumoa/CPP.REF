#pragma once

namespace SC::Game
{
	/// <summary> ���� ��ü�� ���ԵǴ� ���� ��ҿ� ���� �⺻ �Լ��� �����մϴ�. </summary>
	class Component : virtual public Object, virtual public ICloneable
	{
		friend class GameObject;

		WeakPtr gameObject;

		bool isEnabled = true;
		bool isFirst = true;

	protected:
		/// <summary> <see cref="Component"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Component();

		/// <summary> (���� ���� ���) ������Ʈ�� ������ �۾��� �����մϴ�. </summary>
		virtual void Render( RefPtr<Details::CDeviceContext>& deviceContext );

	public:
		/// <summary> ������Ʈ�� ��ü�� ����� �� ȣ��˴ϴ�. </summary>
		virtual void Awake();

		/// <summary> ����� ��ü�� ���۵� �� ����˴ϴ�. </summary>
		virtual void Start();

		/// <summary> ������Ʈ�� ���� �� ������ �����մϴ�. </summary>
		/// <param name="time"> ���� �ð��� �����մϴ�. </param>
		/// <param name="input"> ���� �����ӿ��� �Է� ���¸� �����մϴ�. </param>
		virtual void Update( Time& time, Input& input );

		/// <summary> ������Ʈ�� ���� ���� ������ �� ������ �����մϴ�. </summary>
		/// <param name="time"> ���� �ð��� �����մϴ�. </param>
		virtual void FixedUpdate( Time& time );

		/// <summary> ������Ʈ�� ���� ���� �� ������ �����մϴ�. </summary>
		/// <param name="time"> ���� �ð��� �����մϴ�. </param>
		/// <param name="input"> ���� �����ӿ��� �Է� ���¸� �����մϴ�. </param>
		virtual void LateUpdate( Time& time, Input& input );

		/// <summary> ����� ���� ��ü�� Ȯ�� ������Ʈ�� �߰��մϴ�. </summary>
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

		/// <summary> ����� ���� ��ü�� �߰��� Ȯ�� ������Ʈ�� �����ɴϴ�. </summary>
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

		/// <summary> (Visual Studio ����) ����� ���� ��ü�� �����ɴϴ�. </summary>
		vs_property_get( RefPtr<GameObject>, Linked );

		/// <summary> ����� ���� ��ü�� �����ɴϴ�. </summary>
		RefPtr<GameObject> Linked_get();

		/// <summary> (Visual Studio ����) ����� ���� ��ü�� ��ȯ ���¸� �����ɴϴ�. </summary>
		vs_property_get( RefPtr<Game::Transform>, Transform );

		/// <summary> ����� ���� ��ü�� ��ȯ ���¸� �����ɴϴ�. </summary>
		RefPtr<Game::Transform> Transform_get();

		/// <summary> (Visual Studio ����) ���� ����� ���� �۾�(Update �� FixedUpdate)�� Ȱ��ȭ�Ǿ����� �����ϴ� ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( bool, IsEnabled );

		/// <summary> ���� ����� ���� �۾�(Update �� FixedUpdate)�� Ȱ��ȭ�Ǿ����� �����ϴ� ���� �����ɴϴ�. </summary>
		bool IsEnabled_get();

		/// <summary> ���� ����� ���� �۾�(Update �� FixedUpdate)�� Ȱ��ȭ�Ǿ����� �����ϴ� ���� �����մϴ�. </summary>
		void IsEnabled_set( bool value );
	};
}