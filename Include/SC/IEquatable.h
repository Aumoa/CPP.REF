#pragma once

namespace SC
{
	/// <summary> 값 형식 또는 클래스 형식에서 개체의 같음을 결정하는 Equals 함수를 제공합니다. </summary>
	template< class T >
	struct IEquatable : virtual public Object
	{
		/// <summary> 현재 개체가 동일한 종류의 다른 개체와 동일한지 비교합니다. </summary>
		/// <param name="right"> 동일한 종류의 개체를 전달합니다. </param>
		virtual bool Equals( T right ) = 0;
	};
}