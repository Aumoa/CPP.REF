#pragma once

namespace SC
{
	/// <summary> 값 형식 또는 클래스 형식에서 해당 개체의 순서를 정렬하기 위해 구현하는 CompareTo 함수를 제공합니다. </summary>
	template< class T >
	struct IComparable : virtual public Object
	{
		/// <summary> 현재 개체와 동일한 형식의 다른 개체를 비교하고 정렬 순서에서 현재 개체의 위치가 다른 개체보다 앞인지, 뒤인지 또는 동일한지를 나타내는 정수를 반환합니다. </summary>
		/// <param name="right"> 동일한 종류의 개체를 전달합니다. </param>
		virtual int CompareTo( T right ) = 0;
	};
}