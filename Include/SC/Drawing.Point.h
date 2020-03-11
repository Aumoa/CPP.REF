#pragma once

namespace SC::Drawing
{
#pragma pack( push, 1 )
	/// <summary> 지점을 표현합니다. </summary>
	template< class T >
	struct Point : public ValueType, virtual public IComparable<Point<T>>, virtual public IEquatable<Point<T>>
	{
		/// <summary> 지점의 X축 좌표를 설정합니다. </summary>
		T X;

		/// <summary> 지점의 Y축 좌표를 설정합니다. </summary>
		T Y;

		/// <summary> <see cref="Point"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		Point( T x = 0, T y = 0 )
			: X( x )
			, Y( y )
		{

		}

		/// <summary> <see cref="Point"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		Point( const Point& copy )
			: X( copy.X )
			, Y( copy.Y )
		{

		}

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 사각 영역의 텍스트 표현을 가져옵니다. </summary>
		String ToString() override
		{
			std::wstringstream wss;
			wss << L"(" << X << L", " << Y << L")";
			return wss.str().c_str();
		}

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 지정한 개체가 현재 개체와 같은지 확인합니다. </summary>
		/// <param name="obj"> 비교할 개체를 전달합니다. </param>
		bool Equals( object obj ) override
		{
			if ( Point r; obj.Is<Point>( &r ) )
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
		bool Equals( Point right ) override
		{
			return operator==( right );
		}

		/// <summary> (<see cref="IEquatable"/> 인터페이스에서 구현 됨.) 현재 개체와 동일한 형식의 다른 개체를 비교하고 정렬 순서에서 현재 개체의 위치가 다른 개체보다 앞인지, 뒤인지 또는 동일한지를 나타내는 정수를 반환합니다. </summary>
		/// <param name="right"> 동일한 종류의 개체를 전달합니다. </param>
		int CompareTo( Point right ) override
		{
			if ( X == right.X )
				if ( Y == right.Y )
					return 0;
				else
					return Y < right.Y ? -1 : 1;
			else
				return X < right.X ? -1 : 1;
		}

		/// <summary> 위치가 사각 영역 내부에 포함되었는지 검사합니다. </summary>
		bool InRect( Rect<T> rect )
		{
			if ( X >= rect.Left && Y >= rect.Top &&
				X <= rect.Right && Y <= rect.Bottom )
			{
				return true;
			}

			return false;
		}

		Point operator+( Point right )
		{
			return Point( X + right.X, Y + right.Y );
		}

		Point operator-( Point right )
		{
			return Point( X - right.X, Y - right.Y );
		}

		Point operator*( Point right )
		{
			return Point( X * right.X, Y * right.Y );
		}

		Point operator/( Point right )
		{
			return Point( X / right.X, Y / right.Y );
		}

		Point& operator=( const Point& copy )
		{
			X = copy.X;
			Y = copy.Y;
			return *this;
		}

		bool operator==( Point value )
		{
			return X == value.X && Y == value.Y;
		}

		bool operator!=( Point value )
		{
			return X != value.X || Y != value.Y;
		}

		bool operator< ( Point value )
		{
			return CompareTo( value ) < 0;
		}

		bool operator> ( Point value )
		{
			return CompareTo( value ) > 0;
		}

		bool operator<=( Point value )
		{
			return CompareTo( value ) <= 0;
		}

		bool operator>=( Point value )
		{
			return CompareTo( value ) >= 0;
		}

		template< class O >
		operator Point<O>()
		{
			return Point<O>( ( O )X, ( O )Y );
		}
	};
#pragma pack( pop )
}