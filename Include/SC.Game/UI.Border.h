#pragma once

namespace SC::Game::UI
{
	/// <summary> UI ��ҿ��� ��踦 ǥ���մϴ�. </summary>
	class Border : public Element
	{
		double strokeWidth = 0.0f;
		Thickness padding = 0;

		double radiusX = 0.0;
		double radiusY = 0.0;

		RefPtr<UI::Brush> brush;

	protected:
		/// <summary> (<see cref="Element"/> Ŭ�������� ��� ��.) ��Ҹ� �����մϴ�. </summary>
		/// <param name="clientRect"> �θ��� ������ �簢 ������ ���޵˴ϴ�. </param>
		Drawing::Rect<double> OnUpdate( Drawing::Rect<double> clientRect ) override;

		/// <summary> (<see cref="Element"/> Ŭ�������� ��� ��.) ��Ҹ� �������մϴ�. </summary>
		/// <param name="deviceContext"> �÷��� �������� ���� ��ġ ���ؽ�Ʈ�� ���޵˴ϴ�. </param>
		void OnRender( RefPtr<Details::CDeviceContext>& deviceContext ) override;
		
	public:
		~Border() override;

		/// <summary> <see cref="Border"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ��ü �̸��� �����մϴ�. </param>
		/// <param name="borderThickness"> Thickness �Ӽ��� ���� �⺻���� �����մϴ�. </param>
		/// <param name="padding"> Padding �Ӽ��� ���� �⺻���� �����մϴ�. </param>
		Border( String name, double borderThickness = 1.0, Thickness padding = 0 );

		/// <summary> (<see cref="Element"/> Ŭ�������� ��� ��.) UI �̺�Ʈ�� ó���մϴ�. </summary>
		/// <param name="args"> �̺�Ʈ �Ű������� ���޵˴ϴ�. </param>
		void ProcessEvent( RefPtr<DispatcherEventArgs> args ) override;

		/// <summary> (Visual Studio ����) ��� ������ ���⸦ �����ϰų� �����ɴϴ�. </summary>
		vs_property( double, StrokeWidth );

		/// <summary> ��� ������ ���⸦ �����ɴϴ�. </summary>
		double StrokeWidth_get();

		/// <summary> ��� ������ ���⸦ �����մϴ�. </summary>
		void StrokeWidth_set( double value );

		/// <summary> (Visual Studio ����) ��� �簢���� �𼭸��� �ձ۰� �ϴ� �� ���Ǵ� Ÿ���� X�� �ݰ��� �����ϰų� �����ɴϴ�. </summary>
		vs_property( double, RadiusX );

		/// <summary> ��� �簢���� �𼭸��� �ձ۰� �ϴ� �� ���Ǵ� Ÿ���� X�� �ݰ��� �����ɴϴ�. </summary>
		double RadiusX_get();

		/// <summary> ��� �簢���� �𼭸��� �ձ۰� �ϴ� �� ���Ǵ� Ÿ���� X�� �ݰ��� �����մϴ�. </summary>
		void RadiusX_set( double value );

		/// <summary> (Visual Studio ����) ��� �簢���� �𼭸��� �ձ۰� �ϴ� �� ���Ǵ� Ÿ���� Y�� �ݰ��� �������ų� �����մϴ�. </summary>
		vs_property( double, RadiusY );

		/// <summary> ��� �簢���� �𼭸��� �ձ۰� �ϴ� �� ���Ǵ� Ÿ���� Y�� �ݰ��� �����ɴϴ�. </summary>
		double RadiusY_get();

		/// <summary> ��� �簢���� �𼭸��� �ձ۰� �ϴ� �� ���Ǵ� Ÿ���� Y�� �ݰ��� �����մϴ�. </summary>
		void RadiusY_set( double value );

		/// <summary> (Visual Studio ����) ��� ������ ������ �����ϰų� �����ɴϴ�. </summary>
		vs_property( Thickness, Padding );

		/// <summary> ��� ������ ������ �����ɴϴ�. </summary>
		Thickness Padding_get();

		/// <summary> ��� ������ ������ �����մϴ�. </summary>
		void Padding_set( Thickness value );

		/// <summary> (Visual Studio ����) ��� ������ ���� ������ �귯�ø� �����ϰų� �����ɴϴ�. </summary>
		vs_property( RefPtr<Brush>, Fill );

		/// <summary> ��� ������ ���� �����ɴϴ�. </summary>
		virtual RefPtr<Brush> Fill_get();

		/// <summary> ��� ������ ���� �����մϴ�. </summary>
		virtual void Fill_set( RefPtr<Brush> value );
	};
}