#pragma once

namespace SC
{
	/// <summary> 네이티브 포인터 형식에 대한 래퍼 함수를 제공합니다. </summary>
	struct IntPtr : public ValueType, virtual public IEquatable<void*>, virtual public IComparable<void*>
	{
		/// <summary> 네이티브 포인터 값을 설정합니다. </summary>
		void* Value;

		/// <summary> <see cref="IntPtr"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		IntPtr()
			: Value( nullptr )
		{

		}

		/// <summary> <see cref="IntPtr"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		IntPtr( void* ptr )
			: Value( ptr )
		{

		}

		/// <summary> <see cref="IntPtr"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		IntPtr( const IntPtr& copy )
			: Value( copy.Value )
		{

		}

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 현재 개체의 텍스트 표현을 생성하여 반환합니다. </summary>
		String ToString() override
		{
			std::wostringstream woss;
			if ( Value )
			{
				woss << L"SC.IntPtr(" << std::hex << std::showbase << std::setw( sizeof( void* ) * 2 ) << std::setfill( L'0' ) << Value << L")";
				return woss.str().c_str();
			}
			else
			{
				return L"SC.IntPtr(nullptr)";
			}
		}

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 지정한 개체가 현재 개체와 같은지 확인합니다. </summary>
		/// <param name="obj"> 비교할 개체를 전달합니다. </param>
		bool Equals( object obj ) override
		{
			if ( IntPtr v; obj.Is<IntPtr>( &v ) )
			{
				return Equals( v.Value );
			}
			else
			{
				return obj.Get();
			}
		}

		/// <summary> (<see cref="IEquatable"/> 인터페이스에서 구현 됨.) 현재 개체가 동일한 종류의 다른 개체와 동일한지 비교합니다. </summary>
		/// <param name="right"> 동일한 종류의 개체를 전달합니다. </param>
		bool Equals( void* right ) override
		{
			return Value == right;
		}

		/// <summary> (<see cref="IEquatable"/> 인터페이스에서 구현 됨.) 현재 개체와 동일한 형식의 다른 개체를 비교하고 정렬 순서에서 현재 개체의 위치가 다른 개체보다 앞인지, 뒤인지 또는 동일한지를 나타내는 정수를 반환합니다. </summary>
		/// <param name="right"> 동일한 종류의 개체를 전달합니다. </param>
		int CompareTo( void* right ) override
		{
			if ( Value < right ) return -1;
			else if ( Value > right ) return 1;
			else return 0;
		}

		operator void*()
		{
			return Value;
		}

		IntPtr& operator=( const IntPtr& copy )
		{
			Value = copy.Value;
			return *this;
		}
	};
}