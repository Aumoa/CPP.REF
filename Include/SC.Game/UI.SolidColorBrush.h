#pragma once

namespace SC::Game::UI
{
	/// <summary> 단색으로 영역을 그립니다. </summary>
	class SolidColorBrush : public Brush
	{
	public:
		/// <summary> <see cref="SolidColorBrush"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="color"> 브러시의 색을 설정합니다. </param>
		SolidColorBrush( Drawing::Color color );

		/// <summary> 단색 브러시의 색을 설정하거나 가져옵니다. </summary>
		vs_property( Drawing::Color, SolidColor );

		/// <summary> 단색 브러시의 색을 가져옵니다. </summary>
		Drawing::Color SolidColor_get();

		/// <summary> 단색 브러시의 색을 설정합니다. </summary>
		void SolidColor_set( Drawing::Color value );
	};
}