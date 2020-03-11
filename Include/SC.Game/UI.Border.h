#pragma once

namespace SC::Game::UI
{
	/// <summary> UI 요소에서 경계를 표현합니다. </summary>
	class Border : public Element
	{
		double strokeWidth = 0.0f;
		Thickness padding = 0;

		double radiusX = 0.0;
		double radiusY = 0.0;

		RefPtr<UI::Brush> brush;

	protected:
		/// <summary> (<see cref="Element"/> 클래스에서 상속 됨.) 요소를 갱신합니다. </summary>
		/// <param name="clientRect"> 부모의 컨텐츠 사각 영역이 전달됩니다. </param>
		Drawing::Rect<double> OnUpdate( Drawing::Rect<double> clientRect ) override;

		/// <summary> (<see cref="Element"/> 클래스에서 상속 됨.) 요소를 렌더링합니다. </summary>
		/// <param name="deviceContext"> 플랫폼 렌더링을 위한 장치 컨텍스트가 전달됩니다. </param>
		void OnRender( RefPtr<Details::CDeviceContext>& deviceContext ) override;
		
	public:
		~Border() override;

		/// <summary> <see cref="Border"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 개체 이름을 전달합니다. </param>
		/// <param name="borderThickness"> Thickness 속성에 대한 기본값을 제공합니다. </param>
		/// <param name="padding"> Padding 속성에 대한 기본값을 제공합니다. </param>
		Border( String name, double borderThickness = 1.0, Thickness padding = 0 );

		/// <summary> (<see cref="Element"/> 클래스에서 상속 됨.) UI 이벤트를 처리합니다. </summary>
		/// <param name="args"> 이벤트 매개변수가 전달됩니다. </param>
		void ProcessEvent( RefPtr<DispatcherEventArgs> args ) override;

		/// <summary> (Visual Studio 전용) 경계 선분의 굵기를 설정하거나 가져옵니다. </summary>
		vs_property( double, StrokeWidth );

		/// <summary> 경계 선분의 굵기를 가져옵니다. </summary>
		double StrokeWidth_get();

		/// <summary> 경계 선분의 굵기를 설정합니다. </summary>
		void StrokeWidth_set( double value );

		/// <summary> (Visual Studio 전용) 경계 사각형의 모서리를 둥글게 하는 데 사용되는 타원의 X축 반경을 설정하거나 가져옵니다. </summary>
		vs_property( double, RadiusX );

		/// <summary> 경계 사각형의 모서리를 둥글게 하는 데 사용되는 타원의 X축 반경을 가져옵니다. </summary>
		double RadiusX_get();

		/// <summary> 경계 사각형의 모서리를 둥글게 하는 데 사용되는 타원의 X축 반경을 설정합니다. </summary>
		void RadiusX_set( double value );

		/// <summary> (Visual Studio 전용) 경계 사각형의 모서리를 둥글게 하는 데 사용되는 타원의 Y축 반경을 가져오거나 설정합니다. </summary>
		vs_property( double, RadiusY );

		/// <summary> 경계 사각형의 모서리를 둥글게 하는 데 사용되는 타원의 Y축 반경을 가져옵니다. </summary>
		double RadiusY_get();

		/// <summary> 경계 사각형의 모서리를 둥글게 하는 데 사용되는 타원의 Y축 반경을 설정합니다. </summary>
		void RadiusY_set( double value );

		/// <summary> (Visual Studio 전용) 경계 내부의 여백을 설정하거나 가져옵니다. </summary>
		vs_property( Thickness, Padding );

		/// <summary> 경계 내부의 여백을 가져옵니다. </summary>
		Thickness Padding_get();

		/// <summary> 경계 내부의 여백을 설정합니다. </summary>
		void Padding_set( Thickness value );

		/// <summary> (Visual Studio 전용) 경계 선분의 색을 결정할 브러시를 설정하거나 가져옵니다. </summary>
		vs_property( RefPtr<Brush>, Fill );

		/// <summary> 경계 선분의 색을 가져옵니다. </summary>
		virtual RefPtr<Brush> Fill_get();

		/// <summary> 경계 선분의 색을 설정합니다. </summary>
		virtual void Fill_set( RefPtr<Brush> value );
	};
}