#pragma once

namespace SC::Game::UI
{
	/// <summary> 이미지를 표시하는 UI 요소입니다. </summary>
	class Image : public Element
	{
		RefPtr<Shapes::Rectangle> rectangle;
		RefPtr<Texture2D> imageSource;
		double opacity = 1.0;

	protected:
		/// <summary> (<see cref="Element"/> 클래스에서 상속 됨.) 요소를 갱신합니다. </summary>
		/// <param name="clientRect"> 부모의 컨텐츠 사각 영역이 전달됩니다. </param>
		Drawing::Rect<double> OnUpdate( Drawing::Rect<double> clientRect ) override;

		/// <summary> (<see cref="Element"/> 클래스에서 상속 됨.) 요소를 렌더링합니다. </summary>
		/// <param name="deviceContext"> 플랫폼 렌더링을 위한 장치 컨텍스트가 전달됩니다. </param>
		void OnRender( RefPtr<Details::CDeviceContext>& deviceContext ) override;

	public:
		/// <summary> <see cref="Image"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 개체 이름을 전달합니다. </param>
		Image( String name );

		/// <summary> (Visual Studio 전용) 이미지 원본 개체를 설정하거나 가져옵니다. </summary>
		vs_property( RefPtr<Texture2D>, Source );

		/// <summary> 이미지 원본 개체를 가져옵니다. </summary>
		RefPtr<Texture2D> Source_get();

		/// <summary> 이미지 원본 개체를 설정합니다. </summary>
		void Source_set( RefPtr<Texture2D> value );

		/// <summary> (Visual Studio 전용) 이미지 렌더링 불투명도를 설정하거나 가져옵니다. </summary>
		vs_property( double, Opacity );

		/// <summary> 이미지 렌더링 불투명도를 가져옵니다. </summary>
		double Opacity_get();

		/// <summary> 이미지 렌더링 불투명도를 설정합니다. </summary>
		void Opacity_set( double value );
	};
}