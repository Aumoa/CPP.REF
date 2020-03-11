#pragma once

namespace SC
{
	/// <summary> 컬렉션에서 단순하게 반복할 수 있도록 지원하는 열거자를 노출합니다. </summary>
	template< class T >
	struct IEnumerable : virtual public Object
	{
		/// <summary> 컬렉션 전체를 반복하는 열거자를 반환합니다. </summary>
		virtual RefPtr<IEnumerator<T>> GetEnumerator() = 0;
	};
}