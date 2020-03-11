#pragma once

namespace SC::Game::UI
{
	/// <summary> �̹����� ǥ���ϴ� UI ����Դϴ�. </summary>
	class Image : public Element
	{
		RefPtr<Shapes::Rectangle> rectangle;
		RefPtr<Texture2D> imageSource;
		double opacity = 1.0;

	protected:
		/// <summary> (<see cref="Element"/> Ŭ�������� ��� ��.) ��Ҹ� �����մϴ�. </summary>
		/// <param name="clientRect"> �θ��� ������ �簢 ������ ���޵˴ϴ�. </param>
		Drawing::Rect<double> OnUpdate( Drawing::Rect<double> clientRect ) override;

		/// <summary> (<see cref="Element"/> Ŭ�������� ��� ��.) ��Ҹ� �������մϴ�. </summary>
		/// <param name="deviceContext"> �÷��� �������� ���� ��ġ ���ؽ�Ʈ�� ���޵˴ϴ�. </param>
		void OnRender( RefPtr<Details::CDeviceContext>& deviceContext ) override;

	public:
		/// <summary> <see cref="Image"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ��ü �̸��� �����մϴ�. </param>
		Image( String name );

		/// <summary> (Visual Studio ����) �̹��� ���� ��ü�� �����ϰų� �����ɴϴ�. </summary>
		vs_property( RefPtr<Texture2D>, Source );

		/// <summary> �̹��� ���� ��ü�� �����ɴϴ�. </summary>
		RefPtr<Texture2D> Source_get();

		/// <summary> �̹��� ���� ��ü�� �����մϴ�. </summary>
		void Source_set( RefPtr<Texture2D> value );

		/// <summary> (Visual Studio ����) �̹��� ������ �������� �����ϰų� �����ɴϴ�. </summary>
		vs_property( double, Opacity );

		/// <summary> �̹��� ������ �������� �����ɴϴ�. </summary>
		double Opacity_get();

		/// <summary> �̹��� ������ �������� �����մϴ�. </summary>
		void Opacity_set( double value );
	};
}