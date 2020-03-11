#pragma once

namespace SC::Game::UI
{
	/// <summary> 사각형 프레임의 두께를 표현합니다. </summary>
	struct Thickness : public ValueType
	{
		/// <summary> 왼쪽 두께를 설정합니다. </summary>
		double Left = 0;

		/// <summary> 위쪽 두께를 설정합니다. </summary>
		double Top = 0;

		/// <summary> 오른쪽 두께를 설정합니다. </summary>
		double Right = 0;

		/// <summary> 아래쪽 두께를 설정합니다. </summary>
		double Bottom = 0;

		/// <summary> <see cref="Thickness"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		Thickness() = default;

		/// <summary> <see cref="Thickness"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		Thickness( const Thickness& copy )
			: Left( copy.Left )
			, Top( copy.Top )
			, Right( copy.Right )
			, Bottom( copy.Bottom )
		{

		}

		/// <summary> <see cref="Thickness"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		Thickness( double value )
			: Left( value )
			, Top( value )
			, Right( value )
			, Bottom( value )
		{

		}

		/// <summary> <see cref="Thickness"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		Thickness( double horizontal, double vertical )
			: Left( horizontal )
			, Top( vertical )
			, Right( horizontal )
			, Bottom( vertical )
		{

		}

		/// <summary> <see cref="Thickness"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		Thickness( double left, double top, double right, double bottom )
			: Left( left )
			, Top( top )
			, Right( right )
			, Bottom( bottom )
		{

		}

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 해당 개체의 텍스트 표현을 가져옵니다. </summary>
		String ToString() override
		{
			return String::Format( "{0}, {1}, {2}, {3}", Left, Top, Right, Bottom );
		}

		/// <summary> (Visual Studio 전용) 개체의 넓이 변화량을 가져옵니다. </summary>
		vs_property_get( double, WidthDelta );

		/// <summary> 개체의 넓이 변화량을 가져옵니다. </summary>
		double WidthDelta_get()
		{
			return Left + Right;
		}

		/// <summary> (Visual Studio 전용) 개체의 높이 변화량을 가져옵니다. </summary>
		vs_property_get( double, HeightDelta );

		/// <summary> 개체의 높이 변화량을 가져옵니다. </summary>
		double HeightDelta_get()
		{
			return Top + Bottom;
		}

		Thickness& operator=( const Thickness& copy )
		{
			Left = copy.Left;
			Top = copy.Top;
			Right = copy.Right;
			Bottom = copy.Bottom;

			return *this;
		}
	};
}