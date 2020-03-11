#pragma once

namespace SC::Game::UI
{
	/// <summary> UI ��Ҹ� �����ϴ� �г� ���� ���� ������ ��Ʈ�Դϴ�. �Ļ� Ŭ������ �г� ������ �⺻ ����� �����մϴ�. </summary>
	class Panel : public Element, virtual public Collections::IList<RefPtr<Element>>
	{
		class Enumerator;

	public:
		using ContentType = std::shared_ptr<std::vector<RefPtr<Element>>>;

	private:
		ContentType contents;

	protected:
		/// <summary> <see cref="Panel"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ��ü �̸��� �����մϴ�. </param>
		Panel( String name );

		/// <summary> (<see cref="Element"/> Ŭ�������� ��� ��.) ��Ҹ� �������մϴ�. </summary>
		/// <param name="deviceContext"> �÷��� �������� ���� ��ġ ���ؽ�Ʈ�� ���޵˴ϴ�. </param>
		void OnRender( RefPtr<Details::CDeviceContext>& deviceContext ) override;

	public:
		/// <summary> (<see cref="IEnumerable"/> �������̽����� ���� ��.) �÷��� ��ü�� �ݺ��ϴ� �����ڸ� ��ȯ�մϴ�. </summary>
		RefPtr<IEnumerator<RefPtr<Element>>> GetEnumerator() override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) �׸��� �߰��մϴ�. </summary>
		/// <param name="value"> �߰��� ��ü�� �����մϴ�. </param>
		/// <returns> �� ��Ҹ� ������ ��ġ�� ��ȯ�մϴ�. �����Ͽ��� ��� -1�� ��ȯ�մϴ�. </returns>
		/// <exception cref="NotSupportedException">
		/// <para> ��ü�� �б� ���� ����Ʈ�� ��� �߻��մϴ�. </para>
		/// <para> ��ü�� ���� ũ���� ��� �߻��մϴ�. </para>
		/// </exception>
		int Add( RefPtr<Element> value ) override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) �׸��� ��� �����մϴ�. </summary>
		/// <exception cref="NotSupportedException">
		/// <para> ��ü�� �б� ���� ����Ʈ�� ��� �߻��մϴ�. </para>
		/// <para> ��ü�� ���� ũ���� ��� �߻��մϴ�. </para>
		/// </exception>
		void Clear() override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) Ư�� �׸��� �����ϴ��� ���θ� Ȯ���մϴ�. </summary>
		/// <param name="value"> ã�� ��ü�� �����մϴ�. </param>
		/// <returns> ��ü�� ����Ʈ�� ������ true, �׷��� �ʴٸ� false�� ��ȯ�մϴ�. </returns>
		bool Contains( RefPtr<Element> value ) override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) Ư�� �ε������� �����Ͽ� ��Ҹ� ������ �迭�� �����մϴ�. </summary>
		/// <param name="array"> ���� ��� �迭 �������Դϴ�. ����� ũ�Ⱑ �غ�Ǿ�� �մϴ�. </param>
		/// <param name="index"> ���簡 ���۵Ǵ� �ε����Դϴ�. </param>
		/// <exception cref="ArgumentNullException"> <paramref name="array"/>�� nullptr�� ��� �߻��մϴ�. </exception>
		/// <exception cref="ArgumentOutOfRangeException"> <paramref name="index"/>�� 0���� �۰ų� �迭�� ũ�⺸�� ũ�ų� ���� ��� ��� �߻��մϴ�. </exception>
		void CopyTo( RefPtr<Element>* array, int index = 0 ) override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) Ư�� �׸��� �ε����� �˻��մϴ�. </summary>
		/// <param name="value"> ã�� ��ü�� �����մϴ�. </param>
		/// <returns> ��Ͽ� ��ü�� �����ϸ� ��ü�� �ε�����, �׷��� �ʴٸ� -1�� ��ȯ�մϴ�. </returns>
		int IndexOf( RefPtr<Element> value ) override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) ����Ʈ�� �� ��Ҹ� ������ �ε����� �����մϴ�. </summary>
		/// <param name="index"> ������ �ε����� �����մϴ�. </param>
		/// <param name="value"> ������ ��ü�� �����մϴ�. </param>
		/// <exception cref="ArgumentOutOfRangeException"> <paramref name="index"/>�� 0���� �۰ų� �迭�� ũ�⺸�� Ŭ ��� ��� �߻��մϴ�. </exception>
		/// <exception cref="NotSupportedException">
		/// <para> ��ü�� �б� ���� ����Ʈ�� ��� �߻��մϴ�. </para>
		/// <para> ��ü�� ���� ũ���� ��� �߻��մϴ�. </para>
		/// </exception>
		void Insert( int index, RefPtr<Element> value ) override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) ����Ʈ���� ���� ó�� �߰ߵǴ� Ư�� ��ü�� �����մϴ�. </summary>
		/// <param name="value"> ������ ��ü�� �����մϴ�. </param>
		/// <exception cref="NotSupportedException">
		/// <para> ��ü�� �б� ���� ����Ʈ�� ��� �߻��մϴ�. </para>
		/// <para> ��ü�� ���� ũ���� ��� �߻��մϴ�. </para>
		/// </exception>
		void Remove( RefPtr<Element> value ) override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) ����Ʈ���� ���� ó�� �߰ߵǴ� Ư�� ��ü�� �����մϴ�. </summary>
		/// <param name="index"> ������ �׸��� �ε����� �����մϴ�. </param>
		/// <exception cref="ArgumentOutOfRangeException"> <paramref name="index"/>�� 0���� �۰ų� �迭�� ũ�⺸�� ũ�ų� ���� ��� ��� �߻��մϴ�. </exception>
		/// <exception cref="NotSupportedException">
		/// <para> ��ü�� �б� ���� ����Ʈ�� ��� �߻��մϴ�. </para>
		/// <para> ��ü�� ���� ũ���� ��� �߻��մϴ�. </para>
		/// </exception>
		void RemoveAt( int index ) override;

		/// <summary> (<see cref="Element"/> Ŭ�������� ��� ��.) UI �̺�Ʈ�� ó���մϴ�. </summary>
		/// <param name="args"> �̺�Ʈ �Ű������� ���޵˴ϴ�. </param>
		void ProcessEvent( RefPtr<DispatcherEventArgs> args ) override;

		/// <summary> Ư�� �׸��� �ε����� �˻��մϴ�. </summary>
		/// <param name="name"> ã�� ��ü�� �̸��� �����մϴ�. </param>
		/// <returns> ��Ͽ� ��ü�� �����ϸ� ��ü�� �ε�����, �׷��� �ʴٸ� -1�� ��ȯ�մϴ�. </returns>
		int IndexOf( String name );

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) ���Ե� ��� ���� �����ɴϴ�. </summary>
		int Count_get() override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) ũ�Ⱑ �����Ǿ� �ִ��� ���θ� ��Ÿ���� ���� �����ɴϴ�. </summary>
		bool IsFixedSize_get() override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) ������ �ε����� �ִ� ��Ҹ� �����ɴϴ�. </summary>
		RefPtr<Element> Item_get( int index ) override;

		/// <summary> (<see cref="Collections::IList"/> �������̽����� ���� ��.) ������ �ε����� �ִ� ��Ҹ� �����մϴ�. </summary>
		void Item_set( int index, RefPtr<Element> value ) override;

		/// <summary> �� Ŭ������ �̸� �ؽ� ���� �����ɴϴ�. </summary>
		static const std::size_t ClassName;

		/// <summary> �гο� ���Ե� ����� Z ������ �����ϴ� �Ӽ��� �ؽ� ���� �����ɴϴ�. </summary>
		static const std::size_t ZOrder;
	};
}