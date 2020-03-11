#pragma once

namespace SC
{
	/// <summary> �� ���� �Ǵ� Ŭ���� ���Ŀ��� ��ü�� ������ �����ϴ� Equals �Լ��� �����մϴ�. </summary>
	template< class T >
	struct IEquatable : virtual public Object
	{
		/// <summary> ���� ��ü�� ������ ������ �ٸ� ��ü�� �������� ���մϴ�. </summary>
		/// <param name="right"> ������ ������ ��ü�� �����մϴ�. </param>
		virtual bool Equals( T right ) = 0;
	};
}