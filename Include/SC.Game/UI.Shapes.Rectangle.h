#pragma once

namespace SC::Game::UI::Shapes
{
	/// <summary> 사각형 모양을 표현하는 UI 요소입니다. </summary>
	class Rectangle : public Shape
	{
		double radiusX = 0.0;
		double radiusY = 0.0;

	protected:
		/// <summary> (<see cref="Element"/> 클래스에서 상속 됨.) 요소를 갱신합니다. </summary>
		/// <param name="clientRect"> 부모의 컨텐츠 사각 영역이 전달됩니다. </param>
		Drawing::Rect<double> OnUpdate( Drawing::Rect<double> clientRect ) override;

		/// <summary> (<see cref="Element"/> 클래스에서 상속 됨.) 요소를 렌더링합니다. </summary>
		/// <param name="deviceContext"> 플랫폼 렌더링을 위한 장치 컨텍스트가 전달됩니다. </param>
		void OnRender( RefPtr<Details::CDeviceContext>& deviceContext ) override;

	public:
		/// <summary> <see cref="Rectangle"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 개체 이름을 전달합니다. </param>
		Rectangle( String name );

		/// <summary> (Visual Studio 전용) 사각형의 모서리를 둥글게 하는 데 사용되는 타원의 X축 반경을 설정하거나 가져옵니다. </summary>
		vs_property( double, RadiusX );

		/// <summary> 사각형의 모서리를 둥글게 하는 데 사용되는 타원의 X축 반경을 가져옵니다. </summary>
		double RadiusX_get();

		/// <summary> 사각형의 모서리를 둥글게 하는 데 사용되는 타원의 X축 반경을 설정합니다. </summary>
		void RadiusX_set( double value );

		/// <summary> (Visual Studio 전용) 사각형의 모서리를 둥글게 하는 데 사용되는 타원의 Y축 반경을 가져오거나 설정합니다. </summary>
		vs_property( double, RadiusY );

		/// <summary> 사각형의 모서리를 둥글게 하는 데 사용되는 타원의 Y축 반경을 가져옵니다. </summary>
		double RadiusY_get();

		/// <summary> 사각형의 모서리를 둥글게 하는 데 사용되는 타원의 Y축 반경을 설정합니다. </summary>
		void RadiusY_set( double value );
	};
}