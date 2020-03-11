#pragma once

namespace SC
{
	/// <summary> �÷��ǿ��� �ܼ��ϰ� �ݺ��� �� �ֵ��� �����ϴ� �����ڸ� �����մϴ�. </summary>
	template< class T >
	struct IEnumerable : virtual public Object
	{
		/// <summary> �÷��� ��ü�� �ݺ��ϴ� �����ڸ� ��ȯ�մϴ�. </summary>
		virtual RefPtr<IEnumerator<T>> GetEnumerator() = 0;
	};
}