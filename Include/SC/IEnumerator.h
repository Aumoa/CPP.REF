#pragma once

namespace SC
{
	/// <summary> 컬렉션을 단순하게 반복할 수 있도록 지원합니다. </summary>
	template< class T >
	struct IEnumerator : virtual public Object
	{
		/// <summary> 열거자를 컬렉션의 다음 요소로 이동합니다. </summary>
		/// <exception cref="InvalidOperationException"> 열거자가 만들어진 이후 컬렉션이 수정되었을 경우 발생합니다. </exception>
		virtual bool MoveNext() = 0;

		/// <summary> 컬렉션의 첫 번째 요소 앞의 초기 위치에 열거자를 설정합니다. </summary>
		/// <exception cref="InvalidOperationException"> 열거자가 만들어진 이후 컬렉션이 수정되었을 경우 발생합니다. </exception>
		virtual void Reset() = 0;

		/// <summary> (Visual Studio 전용) 컬렉션에서 열거자의 현재 위치에 있는 요소를 가져옵니다. </summary>
		vs_property_get( T, Current );

		/// <summary> 컬렉션에서 열거자의 현재 위치에 있는 요소를 가져옵니다. </summary>
		virtual T Current_get() = 0;

		bool operator!=( RefPtr<IEnumerator<T>> right )
		{
			return MoveNext();
		}
	};
}

template< class T >
inline SC::RefPtr<SC::IEnumerator<T>>& operator++( SC::RefPtr<SC::IEnumerator<T>>& enumerator )
{
	return enumerator;
}

template< class T >
inline T operator*( SC::RefPtr<SC::IEnumerator<T>> enumerator )
{
	return enumerator->Current_get();
}

namespace std
{
	template< class T >
	inline SC::RefPtr<SC::IEnumerator<T>> begin( SC::RefPtr<SC::IEnumerator<T>> enumerator )
	{
		return enumerator;
	}

	template< class T >
	inline SC::RefPtr<SC::IEnumerator<T>> end( SC::RefPtr<SC::IEnumerator<T>> enumerator )
	{
		return enumerator;
	}
}