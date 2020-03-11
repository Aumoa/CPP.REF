#pragma once

namespace SC::Collections
{
	/// <summary> 필요에 따라 크기가 동적으로 증가하는 배열을 사용하여 IList 인터페이스를 구현합니다. </summary>
	template< class T >
	class ArrayList : virtual public Object, virtual public IList<T>
	{
		T* dynamicArray = nullptr;
		int count = 0;
		int capacity = 0;

	public:
		/// <summary> <see cref="ArrayList"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="capacity"> 초기 용량을 지정합니다. </param>
		ArrayList( int capacity = 16 );
	};
}