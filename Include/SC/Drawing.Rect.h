#pragma once

namespace SC::Drawing
{
#pragma pack( push, 1 )
	/// <summary> �簢 ������ ǥ���մϴ�. </summary>
	template< class T >
	struct Rect : public ValueType, virtual public IComparable<Rect<T>>, virtual public IEquatable<Rect<T>>
	{
		/// <summary> �簢���� ���� �κ� ��ġ�� �����մϴ�. </summary>
		T Left;

		/// <summary> �簢���� ���� �κ� ��ġ�� �����մϴ�. </summary>
		T Top;

		/// <summary> �簢���� ������ �κ� ��ġ�� �����մϴ�. </summary>
		T Right;

		/// <summary> �簢���� �Ʒ��� �κ� ��ġ�� �����մϴ�. </summary>
		T Bottom;
		
		/// <summary> <see cref="Rect"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Rect()
			: Rect( 0, 0, 0, 0 )
		{

		}

		/// <summary> <see cref="Rect"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Rect( const Rect& copy )
			: Left( copy.Left )
			, Top( copy.Top )
			, Right( copy.Right )
			, Bottom( copy.Bottom )
		{

		}

		/// <summary> <see cref="Rect"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Rect( T left, T top, T right, T bottom )
			: Left( left )
			, Top( top )
			, Right( right )
			, Bottom( bottom )
		{

		}

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) �簢 ������ �ؽ�Ʈ ǥ���� �����ɴϴ�. </summary>
		String ToString() override
		{
			std::wstringstream wss;
			wss << L"{LT:(" << Left << L", " << Top << L"), RB:(" << Right << L", " << Bottom << L"), Size:(" << Width_get() << L"*" << Height_get() << L")}";
			return wss.str().c_str();
		}

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ������ ��ü�� ���� ��ü�� ������ Ȯ���մϴ�. </summary>
		/// <param name="obj"> ���� ��ü�� �����մϴ�. </param>
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

		/// <summary> (<see cref="IEquatable"/> �������̽����� ���� ��.) ���� ��ü�� ������ ������ �ٸ� ��ü�� �������� ���մϴ�. </summary>
		/// <param name="right"> ������ ������ ��ü�� �����մϴ�. </param>
		bool Equals( Rect right ) override
		{
			return operator==( right );
		}

		/// <summary> (<see cref="IEquatable"/> �������̽����� ���� ��.) ���� ��ü�� ������ ������ �ٸ� ��ü�� ���ϰ� ���� �������� ���� ��ü�� ��ġ�� �ٸ� ��ü���� ������, ������ �Ǵ� ���������� ��Ÿ���� ������ ��ȯ�մϴ�. </summary>
		/// <param name="right"> ������ ������ ��ü�� �����մϴ�. </param>
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

		/// <summary> (Visual Studio ����) �簢 ������ ���̸� �����ɴϴ�. </summary>
		vs_property_get( T, Width );

		/// <summary> �簢 ������ ���̸� �����ɴϴ�. </summary>
		T Width_get()
		{
			return Right - Left;
		}

		/// <summary> (Visual Studio ����) �簢 ������ ���̸� �����ɴϴ�. </summary>
		vs_property_get( T, Height );

		/// <summary> �簢 ������ ���̸� �����ɴϴ�. </summary>
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