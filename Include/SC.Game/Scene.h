#pragma once

namespace SC::Game
{
	namespace Core
	{
		class GameLogic;
	}

	/// <summary> ���� ��鿡 ���� ���� Ŭ������ ǥ���մϴ�. </summary>
	class Scene : virtual public Object, virtual public Collections::IList<RefPtr<GameObject>>
	{
		friend class Details::GameLogic;

		std::vector<RefPtr<GameObject>> gameObjects;

		Time time;
		Input input;
		Diagnostics::StepTimer timer;
		Diagnostics::StepTimer fixedTimer;

		bool firstUpdate = false;

	public:
		/// <summary> <see cref="Scene"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Scene();
		~Scene() override;

		/// <summary> (<see cref="IEnumerable"/> �������̽����� ���� ��.) �÷��� ��ü�� �ݺ��ϴ� �����ڸ� ��ȯ�մϴ�. </summary>
		RefPtr<IEnumerator<RefPtr<GameObject>>> GetEnumerator() override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) �׸��� �߰��մϴ�. </summary>
		/// <param name="value"> �߰��� ��ü�� �����մϴ�. </param>
		/// <returns> �� ��Ҹ� ������ ��ġ�� ��ȯ�մϴ�. �����Ͽ��� ��� -1�� ��ȯ�մϴ�. </returns>
		/// <exception cref="NotSupportedException">
		/// <para> ��ü�� �б� ���� ����Ʈ�� ��� �߻��մϴ�. </para>
		/// <para> ��ü�� ���� ũ���� ��� �߻��մϴ�. </para>
		/// </exception>
		int Add( RefPtr<GameObject> value ) override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) �׸��� ��� �����մϴ�. </summary>
		/// <exception cref="NotSupportedException">
		/// <para> ��ü�� �б� ���� ����Ʈ�� ��� �߻��մϴ�. </para>
		/// <para> ��ü�� ���� ũ���� ��� �߻��մϴ�. </para>
		/// </exception>
		void Clear() override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) Ư�� �׸��� �����ϴ��� ���θ� Ȯ���մϴ�. </summary>
		/// <param name="value"> ã�� ��ü�� �����մϴ�. </param>
		/// <returns> ��ü�� ����Ʈ�� ������ true, �׷��� �ʴٸ� false�� ��ȯ�մϴ�. </returns>
		bool Contains( RefPtr<GameObject> value ) override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) Ư�� �ε������� �����Ͽ� ��Ҹ� ������ �迭�� �����մϴ�. </summary>
		/// <param name="array"> ���� ��� �迭 �������Դϴ�. ����� ũ�Ⱑ �غ�Ǿ�� �մϴ�. </param>
		/// <param name="index"> ���簡 ���۵Ǵ� �ε����Դϴ�. </param>
		/// <exception cref="ArgumentNullException"> <paramref name="array"/>�� nullptr�� ��� �߻��մϴ�. </exception>
		/// <exception cref="ArgumentOutOfRangeException"> <paramref name="index"/>�� 0���� �۰ų� �迭�� ũ�⺸�� ũ�ų� ���� ��� ��� �߻��մϴ�. </exception>
		void CopyTo( RefPtr<GameObject>* array, int index = 0 ) override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) Ư�� �׸��� �ε����� �˻��մϴ�. </summary>
		/// <param name="value"> ã�� ��ü�� �����մϴ�. </param>
		/// <returns> ��Ͽ� ��ü�� �����ϸ� ��ü�� �ε�����, �׷��� �ʴٸ� -1�� ��ȯ�մϴ�. </returns>
		int IndexOf( RefPtr<GameObject> value ) override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) ����Ʈ�� �� ��Ҹ� ������ �ε����� �����մϴ�. </summary>
		/// <param name="index"> ������ �ε����� �����մϴ�. </param>
		/// <param name="value"> ������ ��ü�� �����մϴ�. </param>
		/// <exception cref="ArgumentOutOfRangeException"> <paramref name="index"/>�� 0���� �۰ų� �迭�� ũ�⺸�� Ŭ ��� ��� �߻��մϴ�. </exception>
		/// <exception cref="NotSupportedException">
		/// <para> ��ü�� �б� ���� ����Ʈ�� ��� �߻��մϴ�. </para>
		/// <para> ��ü�� ���� ũ���� ��� �߻��մϴ�. </para>
		/// </exception>
		void Insert( int index, RefPtr<GameObject> value ) override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) ����Ʈ���� ���� ó�� �߰ߵǴ� Ư�� ��ü�� �����մϴ�. </summary>
		/// <param name="value"> ������ ��ü�� �����մϴ�. </param>
		/// <exception cref="NotSupportedException">
		/// <para> ��ü�� �б� ���� ����Ʈ�� ��� �߻��մϴ�. </para>
		/// <para> ��ü�� ���� ũ���� ��� �߻��մϴ�. </para>
		/// </exception>
		void Remove( RefPtr<GameObject> value ) override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) ����Ʈ���� ���� ó�� �߰ߵǴ� Ư�� ��ü�� �����մϴ�. </summary>
		/// <param name="index"> ������ �׸��� �ε����� �����մϴ�. </param>
		/// <exception cref="ArgumentOutOfRangeException"> <paramref name="index"/>�� 0���� �۰ų� �迭�� ũ�⺸�� ũ�ų� ���� ��� ��� �߻��մϴ�. </exception>
		/// <exception cref="NotSupportedException">
		/// <para> ��ü�� �б� ���� ����Ʈ�� ��� �߻��մϴ�. </para>
		/// <para> ��ü�� ���� ũ���� ��� �߻��մϴ�. </para>
		/// </exception>
		void RemoveAt( int index ) override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) ���Ե� ��� ���� �����ɴϴ�. </summary>
		int Count_get() override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) ũ�Ⱑ �����Ǿ� �ִ��� ���θ� ��Ÿ���� ���� �����ɴϴ�. </summary>
		bool IsFixedSize_get() override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) ������ �ε����� �ִ� ��Ҹ� �����ɴϴ�. </summary>
		RefPtr<GameObject> Item_get( int index ) override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) ������ �ε����� �ִ� ��Ҹ� �����մϴ�. </summary>
		void Item_set( int index, RefPtr<GameObject> value ) override;

		/// <summary> ��鿡�� ù Update�� ȣ��Ǳ� �� ����˴ϴ�. </summary>
		virtual void Start();

		/// <summary> ��鿡 ���Ե� ��� ��ü�� ���� �� ������ ������� �����մϴ�. </summary>
		virtual void Update();

		/// <summary> ��鿡 ���Ե� ��� ��ü�� ���� ���� ������ �� ������ ������� �����մϴ�. </summary>
		virtual void FixedUpdate();

		/// <summary> ��鿡 ���Ե� ��� ��ü�� ���� ���� ������ �� ������ ������� �����մϴ�. </summary>
		 virtual void LateUpdate();

		/// <summary> ����� �ε�� �� ȣ��Ǵ� �Լ��Դϴ�. </summary>
		/// <param name="asyncLoad"> ����� �񵿱� ���·� �ε�� �� �񵿱� ���� ��ü�� ���޵˴ϴ�. </param>
		virtual void Load( RefPtr<IAsyncLoad> asyncLoad );

		/// <summary> ����� ��ε�� �� ȣ��Ǵ� �Լ��Դϴ�. </summary>
		/// <param name="progress"> �۾��� ������� ������ ������ �����Դϴ�. </param>
		virtual void Unload();

	private:
		void Render( RefPtr<Details::CDeviceContext>& deviceContext );
	};
}