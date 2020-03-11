#pragma once

namespace SC::Game::UI::Shapes
{
	/// <summary> 모양을 표현하는 UI 요소 형식 계층 구조의 루트입니다. </summary>
	class Shape : public Element
	{
		RefPtr<Brush> brush;

	protected:
		/// <summary> <see cref="Shape"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 개체 이름을 전달합니다. </param>
		Shape( String name );

	public:
		/// <summary> (Visual Studio 전용) 모양의 채우기 브러시를 설정하거나 가져옵니다. </summary>
		vs_property( RefPtr<Brush>, Fill );

		/// <summary> 모양의 채우기 브러시를 가져옵니다. </summary>
		RefPtr<Brush> Fill_get();

		/// <summary> 모양의 채우기 브러시를 설정합니다. </summary>
		void Fill_set( RefPtr<Brush> value );
	};
}