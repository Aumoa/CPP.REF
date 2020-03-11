#pragma once

namespace SC::Game::UI::Shapes
{
	/// <summary> �簢�� ����� ǥ���ϴ� UI ����Դϴ�. </summary>
	class Rectangle : public Shape
	{
		double radiusX = 0.0;
		double radiusY = 0.0;

	protected:
		/// <summary> (<see cref="Element"/> Ŭ�������� ��� ��.) ��Ҹ� �����մϴ�. </summary>
		/// <param name="clientRect"> �θ��� ������ �簢 ������ ���޵˴ϴ�. </param>
		Drawing::Rect<double> OnUpdate( Drawing::Rect<double> clientRect ) override;

		/// <summary> (<see cref="Element"/> Ŭ�������� ��� ��.) ��Ҹ� �������մϴ�. </summary>
		/// <param name="deviceContext"> �÷��� �������� ���� ��ġ ���ؽ�Ʈ�� ���޵˴ϴ�. </param>
		void OnRender( RefPtr<Details::CDeviceContext>& deviceContext ) override;

	public:
		/// <summary> <see cref="Rectangle"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ��ü �̸��� �����մϴ�. </param>
		Rectangle( String name );

		/// <summary> (Visual Studio ����) �簢���� �𼭸��� �ձ۰� �ϴ� �� ���Ǵ� Ÿ���� X�� �ݰ��� �����ϰų� �����ɴϴ�. </summary>
		vs_property( double, RadiusX );

		/// <summary> �簢���� �𼭸��� �ձ۰� �ϴ� �� ���Ǵ� Ÿ���� X�� �ݰ��� �����ɴϴ�. </summary>
		double RadiusX_get();

		/// <summary> �簢���� �𼭸��� �ձ۰� �ϴ� �� ���Ǵ� Ÿ���� X�� �ݰ��� �����մϴ�. </summary>
		void RadiusX_set( double value );

		/// <summary> (Visual Studio ����) �簢���� �𼭸��� �ձ۰� �ϴ� �� ���Ǵ� Ÿ���� Y�� �ݰ��� �������ų� �����մϴ�. </summary>
		vs_property( double, RadiusY );

		/// <summary> �簢���� �𼭸��� �ձ۰� �ϴ� �� ���Ǵ� Ÿ���� Y�� �ݰ��� �����ɴϴ�. </summary>
		double RadiusY_get();

		/// <summary> �簢���� �𼭸��� �ձ۰� �ϴ� �� ���Ǵ� Ÿ���� Y�� �ݰ��� �����մϴ�. </summary>
		void RadiusY_set( double value );
	};
}