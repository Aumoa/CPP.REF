#pragma once

namespace SC::Game::UI
{
	/// <summary> 다중 항목을 격자 형태로 정렬하기 위한 View 요소입니다. </summary>
	class GridView : public ViewBase
	{
		double gridWidth = 0;
		double gridHeight = 0;

	protected:
		/// <summary> (<see cref="Panel"/> 클래스에서 상속 됨.) 요소를 갱신합니다. </summary>
		/// <param name="clientRect"> 부모의 컨텐츠 사각 영역이 전달됩니다. </param>
		Drawing::Rect<double> OnUpdate( Drawing::Rect<double> clientRect ) override;

	public:
		/// <summary> <see cref="GridView"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 개체 이름을 전달합니다. </param>
		GridView( String name );

		/// <summary> (Visual Studio 전용) 격자 각 요소의 넓이를 설정하거나 가져옵니다. </summary>
		vs_property( double, GridWidth );

		/// <summary> 격자 각 요소의 넓이를 가져옵니다. </summary>
		double GridWidth_get();

		/// <summary> 격자 각 요소의 넓이를 설정합니다. </summary>
		void GridWidth_set( double value );

		/// <summary> (Visual Studio 전용) 격자 각 요소의 높이를 설정하거나 가져옵니다. </summary>
		vs_property( double, GridHeight );

		/// <summary> 격자 각 요소의 넓이를 가져옵니다. </summary>
		double GridHeight_get();

		/// <summary> 격자 각 요소의 넓이를 설정합니다. </summary>
		void GridHeight_set( double value );
	};
}