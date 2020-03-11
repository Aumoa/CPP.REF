#pragma once

namespace SC::Game
{
	/// <summary> �ִϸ��̼� �Ű������� ����ϴ� ���� ���� ǥ���մϴ�. </summary>
	struct AnimVar : private std::variant<int, double, bool, Trigger>
	{
	public:
		/// <summary> <see cref="AnimVar"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="value"> int ������ ���� �����մϴ�. </param>
		AnimVar( int value ) : variant( value )
		{

		}

		/// <summary> <see cref="AnimVar"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="value"> double ������ ���� �����մϴ�. </param>
		AnimVar( double value ) : variant( value )
		{

		}

		/// <summary> <see cref="AnimVar"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="value"> bool ������ ���� �����մϴ�. </param>
		AnimVar( bool value ) : variant( value )
		{

		}

		/// <summary> <see cref="AnimVar"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="value"> Trigger ������ ���� �����մϴ�. </param>
		AnimVar( const Trigger& value ) : variant( value )
		{

		}

		/// <summary> (Visual Studio ����) ������ ������ ���� �������� �����ɴϴ�. �� ������ �ǹ̴� ������� Int32, Double, Boolean, Trigger �Դϴ�. </summary>
		vs_property_get( int, Type );

		/// <summary> ������ ������ ���� �������� �����ɴϴ�. �� ������ �ǹ̴� ������� Int32, Double, Boolean, Trigger �Դϴ�. </summary>
		int Type_get() const
		{
			return ( int )index();
		}

		/// <summary> (Visual Studio ����) ������ ���� Int32 �������� �����ϰų� �����ɴϴ�. </summary>
		vs_property( int, Int32 );

		/// <summary> ������ ���� Int32 �������� �����ɴϴ�. </summary>
		int Int32_get() const
		{
			return std::get<0>( *this );
		}

		/// <summary> ������ ���� Int32 �������� �����մϴ�. </summary>
		void Int32_set( int value )
		{
			std::get<0>( *this ) = value;
		}

		/// <summary> (Visual Studio ����) ������ ���� Double �������� �����ϰų� �����ɴϴ�. </summary>
		vs_property( double, Double );

		/// <summary> ������ ���� Double �������� �����ɴϴ�. </summary>
		double Double_get() const
		{
			return std::get<1>( *this );
		}

		/// <summary> ������ ���� Double �������� �����մϴ�. </summary>
		void Double_set( double value )
		{
			std::get<1>( *this ) = value;
		}

		/// <summary> (Visual Studio ����) ������ ���� Boolean �������� �����ϰų� �����ɴϴ�. </summary>
		vs_property( bool, Boolean );

		/// <summary> ������ ���� Boolean �������� �����ɴϴ�. </summary>
		bool Boolean_get() const
		{
			return std::get<2>( *this );
		}

		/// <summary> ������ ���� Boolean �������� �����մϴ�. </summary>
		void Boolean_set( bool value )
		{
			std::get<2>( *this ) = value;
		}

		/// <summary> (Visual Studio ����) ������ ������ Trigger ������ �� Ȱ��ȭ ���¸� �����ϰų� �����ɴϴ�. </summary>
		vs_property( bool, Trigger );

		/// <summary> ������ ������ Trigger ������ �� Ȱ��ȭ ���¸� �����ɴϴ�. </summary>
		bool Trigger_get() const
		{
			return std::get<3>( *this ).IsActive_get();
		}

		/// <summary> ������ ���� Trigger �������� �����մϴ�. </summary>
		void Trigger_set( bool value )
		{
			std::get<3>( *this ).IsActive_set( value );
		}
	};
}