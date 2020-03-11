#pragma once

namespace SC::Collections
{
	/// <summary> �ε������� ���������� �׼����� �� �ִ� ��ü�� �÷����� ��Ÿ���ϴ�. </summary>
	template< class T >
	struct IList : virtual public Object, virtual public IEnumerable<T>
	{
		/// <summary> �׸��� �߰��մϴ�. </summary>
		/// <param name="value"> �߰��� ��ü�� �����մϴ�. </param>
		/// <returns> �� ��Ҹ� ������ ��ġ�� ��ȯ�մϴ�. �����Ͽ��� ��� -1�� ��ȯ�մϴ�. </returns>
		/// <exception cref="NotSupportedException">
		/// <para> ��ü�� �б� ���� ����Ʈ�� ��� �߻��մϴ�. </para>
		/// <para> ��ü�� ���� ũ���� ��� �߻��մϴ�. </para>
		/// </exception>
		virtual int Add( T value ) = 0;

		/// <summary> �׸��� ��� �����մϴ�. </summary>
		/// <exception cref="NotSupportedException">
		/// <para> ��ü�� �б� ���� ����Ʈ�� ��� �߻��մϴ�. </para>
		/// <para> ��ü�� ���� ũ���� ��� �߻��մϴ�. </para>
		/// </exception>
		virtual void Clear() = 0;

		/// <summary> Ư�� �׸��� �����ϴ��� ���θ� Ȯ���մϴ�. </summary>
		/// <param name="value"> ã�� ��ü�� �����մϴ�. </param>
		/// <returns> ��ü�� ����Ʈ�� ������ true, �׷��� �ʴٸ� false�� ��ȯ�մϴ�. </returns>
		virtual bool Contains( T value ) = 0;

		/// <summary> Ư�� �ε������� �����Ͽ� ��Ҹ� ������ �迭�� �����մϴ�. </summary>
		/// <param name="array"> ���� ��� �迭 �������Դϴ�. ����� ũ�Ⱑ �غ�Ǿ�� �մϴ�. </param>
		/// <param name="index"> ���簡 ���۵Ǵ� �ε����Դϴ�. </param>
		/// <exception cref="ArgumentNullException"> <paramref name="array"/>�� nullptr�� ��� �߻��մϴ�. </exception>
		/// <exception cref="ArgumentOutOfRangeException"> <paramref name="index"/>�� 0���� �۰ų� �迭�� ũ�⺸�� ũ�ų� ���� ��� ��� �߻��մϴ�. </exception>
		virtual void CopyTo( T* array, int index = 0 ) = 0;

		/// <summary> Ư�� �׸��� �ε����� �˻��մϴ�. </summary>
		/// <param name="value"> ã�� ��ü�� �����մϴ�. </param>
		/// <returns> ��Ͽ� ��ü�� �����ϸ� ��ü�� �ε�����, �׷��� �ʴٸ� -1�� ��ȯ�մϴ�. </returns>
		virtual int IndexOf( T value ) = 0;

		/// <summary> ����Ʈ�� �� ��Ҹ� ������ �ε����� �����մϴ�. </summary>
		/// <param name="index"> ������ �ε����� �����մϴ�. </param>
		/// <param name="value"> ������ ��ü�� �����մϴ�. </param>
		/// <exception cref="ArgumentOutOfRangeException"> <paramref name="index"/>�� 0���� �۰ų� �迭�� ũ�⺸�� Ŭ ��� ��� �߻��մϴ�. </exception>
		/// <exception cref="NotSupportedException">
		/// <para> ��ü�� �б� ���� ����Ʈ�� ��� �߻��մϴ�. </para>
		/// <para> ��ü�� ���� ũ���� ��� �߻��մϴ�. </para>
		/// </exception>
		virtual void Insert( int index, T value ) = 0;

		/// <summary> ����Ʈ���� ���� ó�� �߰ߵǴ� Ư�� ��ü�� �����մϴ�. </summary>
		/// <param name="value"> ������ ��ü�� �����մϴ�. </param>
		/// <exception cref="NotSupportedException">
		/// <para> ��ü�� �б� ���� ����Ʈ�� ��� �߻��մϴ�. </para>
		/// <para> ��ü�� ���� ũ���� ��� �߻��մϴ�. </para>
		/// </exception>
		virtual void Remove( T value ) = 0;

		/// <summary> ����Ʈ���� ���� ó�� �߰ߵǴ� Ư�� ��ü�� �����մϴ�. </summary>
		/// <param name="index"> ������ �׸��� �ε����� �����մϴ�. </param>
		/// <exception cref="ArgumentOutOfRangeException"> <paramref name="index"/>�� 0���� �۰ų� �迭�� ũ�⺸�� ũ�ų� ���� ��� ��� �߻��մϴ�. </exception>
		/// <exception cref="NotSupportedException">
		/// <para> ��ü�� �б� ���� ����Ʈ�� ��� �߻��մϴ�. </para>
		/// <para> ��ü�� ���� ũ���� ��� �߻��մϴ�. </para>
		/// </exception>
		virtual void RemoveAt( int index ) = 0;

		/// <summary> (Visual Studio ����) ���Ե� ��� ���� �����ɴϴ�. </summary>
		vs_property_get( int, Count );

		/// <summary> ���Ե� ��� ���� �����ɴϴ�. </summary>
		virtual int Count_get() = 0;

		/// <summary> (Visual Studio ����) ũ�Ⱑ �����Ǿ� �ִ��� ���θ� ��Ÿ���� ���� �����ɴϴ�. </summary>
		vs_property_get( bool, IsFixedSize );

		/// <summary> ũ�Ⱑ �����Ǿ� �ִ��� ���θ� ��Ÿ���� ���� �����ɴϴ�. </summary>
		virtual bool IsFixedSize_get() = 0;

		/// <summary> (Visual Studio ����) ������ �ε����� �ִ� ��Ҹ� �������ų� �����մϴ�. </summary>
		vs_property( T, Item )[];

		/// <summary> ������ �ε����� �ִ� ��Ҹ� �����ɴϴ�. </summary>
		virtual T Item_get( int index ) = 0;

		/// <summary> ������ �ε����� �ִ� ��Ҹ� �����մϴ�. </summary>
		virtual void Item_set( int index, T value ) = 0;
	};
}