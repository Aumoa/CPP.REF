#pragma once

namespace SC
{
	/// <summary> �÷����� �ܼ��ϰ� �ݺ��� �� �ֵ��� �����մϴ�. </summary>
	template< class T >
	struct IEnumerator : virtual public Object
	{
		/// <summary> �����ڸ� �÷����� ���� ��ҷ� �̵��մϴ�. </summary>
		/// <exception cref="InvalidOperationException"> �����ڰ� ������� ���� �÷����� �����Ǿ��� ��� �߻��մϴ�. </exception>
		virtual bool MoveNext() = 0;

		/// <summary> �÷����� ù ��° ��� ���� �ʱ� ��ġ�� �����ڸ� �����մϴ�. </summary>
		/// <exception cref="InvalidOperationException"> �����ڰ� ������� ���� �÷����� �����Ǿ��� ��� �߻��մϴ�. </exception>
		virtual void Reset() = 0;

		/// <summary> (Visual Studio ����) �÷��ǿ��� �������� ���� ��ġ�� �ִ� ��Ҹ� �����ɴϴ�. </summary>
		vs_property_get( T, Current );

		/// <summary> �÷��ǿ��� �������� ���� ��ġ�� �ִ� ��Ҹ� �����ɴϴ�. </summary>
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