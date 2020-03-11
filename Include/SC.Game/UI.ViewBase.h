#pragma once

namespace SC::Game::UI
{
	/// <summary> 다중 항목을 정렬하기 위한 View 요소에 대한 기본 함수를 제공합니다. </summary>
	class ViewBase : public Panel
	{
	protected:
		/// <summary> <see cref="ViewBase"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 개체 이름을 전달합니다. </param>
		ViewBase( String name );
	};
}