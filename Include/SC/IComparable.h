#pragma once

namespace SC
{
	/// <summary> �� ���� �Ǵ� Ŭ���� ���Ŀ��� �ش� ��ü�� ������ �����ϱ� ���� �����ϴ� CompareTo �Լ��� �����մϴ�. </summary>
	template< class T >
	struct IComparable : virtual public Object
	{
		/// <summary> ���� ��ü�� ������ ������ �ٸ� ��ü�� ���ϰ� ���� �������� ���� ��ü�� ��ġ�� �ٸ� ��ü���� ������, ������ �Ǵ� ���������� ��Ÿ���� ������ ��ȯ�մϴ�. </summary>
		/// <param name="right"> ������ ������ ��ü�� �����մϴ�. </param>
		virtual int CompareTo( T right ) = 0;
	};
}