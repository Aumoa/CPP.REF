#pragma once

namespace SC::Game
{
	/// <summary> 애니메이션 매개변수로 사용하는 변수 값을 표현합니다. </summary>
	struct AnimVar : private std::variant<int, double, bool, Trigger>
	{
	public:
		/// <summary> <see cref="AnimVar"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="value"> int 형식의 값을 전달합니다. </param>
		AnimVar( int value ) : variant( value )
		{

		}

		/// <summary> <see cref="AnimVar"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="value"> double 형식의 값을 전달합니다. </param>
		AnimVar( double value ) : variant( value )
		{

		}

		/// <summary> <see cref="AnimVar"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="value"> bool 형식의 값을 전달합니다. </param>
		AnimVar( bool value ) : variant( value )
		{

		}

		/// <summary> <see cref="AnimVar"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="value"> Trigger 형식의 값을 전달합니다. </param>
		AnimVar( const Trigger& value ) : variant( value )
		{

		}

		/// <summary> (Visual Studio 전용) 변수의 형식을 정수 유형으로 가져옵니다. 각 정수의 의미는 순서대로 Int32, Double, Boolean, Trigger 입니다. </summary>
		vs_property_get( int, Type );

		/// <summary> 변수의 형식을 정수 유형으로 가져옵니다. 각 정수의 의미는 순서대로 Int32, Double, Boolean, Trigger 입니다. </summary>
		int Type_get() const
		{
			return ( int )index();
		}

		/// <summary> (Visual Studio 전용) 변수의 값을 Int32 형식으로 설정하거나 가져옵니다. </summary>
		vs_property( int, Int32 );

		/// <summary> 변수의 값을 Int32 형식으로 가져옵니다. </summary>
		int Int32_get() const
		{
			return std::get<0>( *this );
		}

		/// <summary> 변수의 값을 Int32 형식으로 설정합니다. </summary>
		void Int32_set( int value )
		{
			std::get<0>( *this ) = value;
		}

		/// <summary> (Visual Studio 전용) 변수의 값을 Double 형식으로 설정하거나 가져옵니다. </summary>
		vs_property( double, Double );

		/// <summary> 변수의 값을 Double 형식으로 가져옵니다. </summary>
		double Double_get() const
		{
			return std::get<1>( *this );
		}

		/// <summary> 변수의 값을 Double 형식으로 설정합니다. </summary>
		void Double_set( double value )
		{
			std::get<1>( *this ) = value;
		}

		/// <summary> (Visual Studio 전용) 변수의 값을 Boolean 형식으로 설정하거나 가져옵니다. </summary>
		vs_property( bool, Boolean );

		/// <summary> 변수의 값을 Boolean 형식으로 가져옵니다. </summary>
		bool Boolean_get() const
		{
			return std::get<2>( *this );
		}

		/// <summary> 변수의 값을 Boolean 형식으로 설정합니다. </summary>
		void Boolean_set( bool value )
		{
			std::get<2>( *this ) = value;
		}

		/// <summary> (Visual Studio 전용) 변수의 형식이 Trigger 형식일 때 활성화 상태를 설정하거나 가져옵니다. </summary>
		vs_property( bool, Trigger );

		/// <summary> 변수의 형식이 Trigger 형식일 때 활성화 상태를 가져옵니다. </summary>
		bool Trigger_get() const
		{
			return std::get<3>( *this ).IsActive_get();
		}

		/// <summary> 변수의 값을 Trigger 형식으로 설정합니다. </summary>
		void Trigger_set( bool value )
		{
			std::get<3>( *this ).IsActive_set( value );
		}
	};
}