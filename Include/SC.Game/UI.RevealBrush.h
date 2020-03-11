#pragma once

namespace SC::Game::UI
{
	/// <summary> 커서의 위치에 따라 이동하는 그라디언트 타원으로 영역을 그립니다. </summary>
	class RevealBrush : public Brush
	{
	public:
		/// <summary> <see cref="RevealBrush"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="color"> 브러시의 색을 설정합니다. </param>
		RevealBrush( Drawing::Color color );

		/// <summary> (Visual Studio 전용) 브러시의 색을 설정하거나 가져옵니다. </summary>
		vs_property( Drawing::Color, RevealColor );

		/// <summary> 브러시의 색을 가져옵니다. </summary>
		Drawing::Color RevealColor_get();

		/// <summary> 브러시의 색을 설정합니다. </summary>
		void RevealColor_set( Drawing::Color value );

		/// <summary> (Visual Studio 전용) 나타나는 거리를 설정하거나 가져옵니다. </summary>
		vs_property( double, RevealDistance );

		/// <summary> 나타나는 거리를 가져옵니다. </summary>
		double RevealDistance_get();

		/// <summary> 나타나는 거리를 설정합니다. </summary>
		void RevealDistance_set( double value );

		/// <summary> (Visual Studio 전용) 브러시의 최소 불투명도를 설정하거나 가져옵니다. </summary>
		vs_property( double, MinOpacity );

		/// <summary> 브러시의 최소 불투명도를 가져옵니다. </summary>
		double MinOpacity_get();

		/// <summary> 브러시의 최소 불투명도를 설정합니다. </summary>
		void MinOpacity_set( double value );
	};
}