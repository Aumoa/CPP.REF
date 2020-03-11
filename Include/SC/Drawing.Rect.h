#pragma once

namespace SC::Drawing
{
#pragma pack( push, 1 )
	/// <summary> 사각 영역을 표현합니다. </summary>
	template< class T >
	struct Rect : public ValueType, virtual public IComparable<Rect<T>>, virtual public IEquatable<Rect<T>>
	{
		/// <summary> 사각형의 왼쪽 부분 위치를 설정합니다. </summary>
		T Left;

		/// <summary> 사각형의 위쪽 부분 위치를 설정합니다. </summary>
		T Top;

		/// <summary> 사각형의 오른쪽 부분 위치를 설정합니다. </summary>
		T Right;

		/// <summary> 사각형의 아래쪽 부분 위치를 설정합니다. </summary>
		T Bottom;
		
		/// <summary> <see cref="Rect"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		Rect()
			: Rect( 0, 0, 0, 0 )
		{

		}

		/// <summary> <see cref="Rect"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		Rect( const Rect& copy )
			: Left( copy.Left )
			, Top( copy.Top )
			, Right( copy.Right )
			, Bottom( copy.Bottom )
		{

		}

		/// <summary> <see cref="Rect"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		Rect( T left, T top, T right, T bottom )
			: Left( left )
			, Top( top )
			, Right( right )
			, Bottom( bottom )
		{

		}

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 사각 영역의 텍스트 표현을 가져옵니다. </summary>
		String ToString() override
		{
			std::wstringstream wss;
			wss << L"{LT:(" << Left << L", " << Top << L"), RB:(" << Right << L", " << Bottom << L"), Size:(" << Width_get() << L"*" << Height_get() << L")}";
			return wss.str().c_str();
		}

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 지정한 개체가 현재 개체와 같은지 확인합니다. </summary>
		/// <param name="obj"> 비교할 개체를 전달합니다. </param>
		bool Equals( object obj ) override
		{
			if ( Rect r; obj.Is<Rect>( &r ) )
			{
				return Equals( r );
			}
			else
			{
				return false;
			}
		}

		/// <summary> (<see cref="IEquatable"/> 인터페이스에서 구현 됨.) 현재 개체가 동일한 종류의 다른 개체와 동일한지 비교합니다. </summary>
		/// <param name="right"> 동일한 종류의 개체를 전달합니다. </param>
		bool Equals( Rect right ) override
		{
			return operator==( right );
		}

		/// <summary> (<see cref="IEquatable"/> 인터페이스에서 구현 됨.) 현재 개체와 동일한 형식의 다른 개체를 비교하고 정렬 순서에서 현재 개체의 위치가 다른 개체보다 앞인지, 뒤인지 또는 동일한지를 나타내는 정수를 반환합니다. </summary>
		/// <param name="right"> 동일한 종류의 개체를 전달합니다. </param>
		int CompareTo( Rect right ) override
		{
			if ( Left == right.Left )
				if ( Top == right.Top )
					if ( Right == right.Right )
						if ( Bottom == right.Bottom )
							return 0;
						else
							return Bottom < right.Bottom ? -1 : 1;
					else
						return Right < right.Right ? -1 : 1;
				else
					return Top < right.Top ? -1 : 1;
			else
				return Left < right.Left ? -1 : 1;
		}

		/// <summary> (Visual Studio 전용) 사각 영역의 넓이를 가져옵니다. </summary>
		vs_property_get( T, Width );

		/// <summary> 사각 영역의 넓이를 가져옵니다. </summary>
		T Width_get()
		{
			return Right - Left;
		}

		/// <summary> (Visual Studio 전용) 사각 영역의 높이를 가져옵니다. </summary>
		vs_property_get( T, Height );

		/// <summary> 사각 영역의 높이를 가져옵니다. </summary>
		T Height_get()
		{
			return Bottom - Top;
		}

		Rect& operator=( const Rect& copy )
		{
			Left = copy.Left;
			Top = copy.Top;
			Right = copy.Right;
			Bottom = copy.Bottom;
			return *this;
		}

		bool operator==( Rect value )
		{
			return Left == value.Left && Top == value.Top && Right == value.Right && Bottom == value.Bottom;
		}

		bool operator!=( Rect value )
		{
			return Left != value.Left || Top != value.Top || Right != value.Right || Bottom != value.Bottom;
		}

		bool operator< ( Rect value )
		{
			return CompareTo( value ) < 0;
		}

		bool operator> ( Rect value )
		{
			return CompareTo( value ) > 0;
		}

		bool operator<=( Rect value )
		{
			return CompareTo( value ) <= 0;
		}

		bool operator>=( Rect value )
		{
			return CompareTo( value ) >= 0;
		}

		template< class O >
		operator Rect<O>()
		{
			return Rect<O>( ( O )Left, ( O )Top, ( O )Right, ( O )Bottom );
		}
	};
#pragma pack( pop )
}