#pragma once

namespace SC::Game::UI
{
	/// <summary> 탐색 요청을 취소할 수 없는 탐색 함수 및 이벤트 핸들러에 대한 매개변수를 제공합니다. </summary>
	class NavigationEventArgs : public EventArgs
	{
		RefPtr<Page> pageFrom;
		RefPtr<Page> pageTo;

	public:
		/// <summary> <see cref="NavigationEventArgs"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="pageFrom"> 탐색 이전 페이지를 전달합니다. </param>
		/// <param name="pageTo"> 탐색 이후 페이지를 전달합니다. </param>
		NavigationEventArgs( RefPtr<Page> pageFrom, RefPtr<Page> pageTo );

		/// <summary> (Visual Studio 전용) 탐색 이전 페이지를 가져옵니다. </summary>
		vs_property_get( RefPtr<Page>, PageFrom );

		/// <summary> 탐색 이전 페이지를 가져옵니다. </summary>
		RefPtr<Page> PageFrom_get();

		/// <summary> (Visual Studio 전용) 탐색 이후 페이지를 가져옵니다. </summary>
		vs_property_get( RefPtr<Page>, PageTo );

		/// <summary> 탐색 이후 페이지를 가져옵니다. </summary>
		RefPtr<Page> PageTo_get();
	};
}