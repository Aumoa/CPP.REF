#pragma once

namespace SC::Game::UI
{
	/// <summary> �ܻ����� ������ �׸��ϴ�. </summary>
	class SolidColorBrush : public Brush
	{
	public:
		/// <summary> <see cref="SolidColorBrush"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="color"> �귯���� ���� �����մϴ�. </param>
		SolidColorBrush( Drawing::Color color );

		/// <summary> �ܻ� �귯���� ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( Drawing::Color, SolidColor );

		/// <summary> �ܻ� �귯���� ���� �����ɴϴ�. </summary>
		Drawing::Color SolidColor_get();

		/// <summary> �ܻ� �귯���� ���� �����մϴ�. </summary>
		void SolidColor_set( Drawing::Color value );
	};
}