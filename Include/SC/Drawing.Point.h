#pragma once

namespace SC::Drawing
{
#pragma pack( push, 1 )
	/// <summary> ������ ǥ���մϴ�. </summary>
	template< class T >
	struct Point : public ValueType, virtual public IComparable<Point<T>>, virtual public IEquatable<Point<T>>
	{
		/// <summary> ������ X�� ��ǥ�� �����մϴ�. </summary>
		T X;

		/// <summary> ������ Y�� ��ǥ�� �����մϴ�. </summary>
		T Y;

		/// <summary> <see cref="Point"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Point( T x = 0, T y = 0 )
			: X( x )
			, Y( y )
		{

		}

		/// <summary> <see cref="Point"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Point( const Point& copy )
			: X( copy.X )
			, Y( copy.Y )
		{

		}

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) �簢 ������ �ؽ�Ʈ ǥ���� �����ɴϴ�. </summary>
		String ToString() override
		{
			std::wstringstream wss;
			wss << L"(" << X << L", " << Y << L")";
			return wss.str().c_str();
		}

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ������ ��ü�� ���� ��ü�� ������ Ȯ���մϴ�. </summary>
		/// <param name="obj"> ���� ��ü�� �����մϴ�. </param>
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

		/// <summary> (<see cref="IEquatable"/> �������̽����� ���� ��.) ���� ��ü�� ������ ������ �ٸ� ��ü�� �������� ���մϴ�. </summary>
		/// <param name="right"> ������ ������ ��ü�� �����մϴ�. </param>
		bool Equals( Point right ) override
		{
			return operator==( right );
		}

		/// <summary> (<see cref="IEquatable"/> �������̽����� ���� ��.) ���� ��ü�� ������ ������ �ٸ� ��ü�� ���ϰ� ���� �������� ���� ��ü�� ��ġ�� �ٸ� ��ü���� ������, ������ �Ǵ� ���������� ��Ÿ���� ������ ��ȯ�մϴ�. </summary>
		/// <param name="right"> ������ ������ ��ü�� �����մϴ�. </param>
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

		/// <summary> ��ġ�� �簢 ���� ���ο� ���ԵǾ����� �˻��մϴ�. </summary>
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