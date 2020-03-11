#pragma once

namespace SC::Game::UI
{
	/// <summary> Ŀ���� ��ġ�� ���� �̵��ϴ� �׶���Ʈ Ÿ������ ������ �׸��ϴ�. </summary>
	class RevealBrush : public Brush
	{
	public:
		/// <summary> <see cref="RevealBrush"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="color"> �귯���� ���� �����մϴ�. </param>
		RevealBrush( Drawing::Color color );

		/// <summary> (Visual Studio ����) �귯���� ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( Drawing::Color, RevealColor );

		/// <summary> �귯���� ���� �����ɴϴ�. </summary>
		Drawing::Color RevealColor_get();

		/// <summary> �귯���� ���� �����մϴ�. </summary>
		void RevealColor_set( Drawing::Color value );

		/// <summary> (Visual Studio ����) ��Ÿ���� �Ÿ��� �����ϰų� �����ɴϴ�. </summary>
		vs_property( double, RevealDistance );

		/// <summary> ��Ÿ���� �Ÿ��� �����ɴϴ�. </summary>
		double RevealDistance_get();

		/// <summary> ��Ÿ���� �Ÿ��� �����մϴ�. </summary>
		void RevealDistance_set( double value );

		/// <summary> (Visual Studio ����) �귯���� �ּ� �������� �����ϰų� �����ɴϴ�. </summary>
		vs_property( double, MinOpacity );

		/// <summary> �귯���� �ּ� �������� �����ɴϴ�. </summary>
		double MinOpacity_get();

		/// <summary> �귯���� �ּ� �������� �����մϴ�. </summary>
		void MinOpacity_set( double value );
	};
}