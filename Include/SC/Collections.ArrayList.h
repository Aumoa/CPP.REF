#pragma once

namespace SC::Collections
{
	/// <summary> �ʿ信 ���� ũ�Ⱑ �������� �����ϴ� �迭�� ����Ͽ� IList �������̽��� �����մϴ�. </summary>
	template< class T >
	class ArrayList : virtual public Object, virtual public IList<T>
	{
		T* dynamicArray = nullptr;
		int count = 0;
		int capacity = 0;

	public:
		/// <summary> <see cref="ArrayList"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="capacity"> �ʱ� �뷮�� �����մϴ�. </param>
		ArrayList( int capacity = 16 );
	};
}