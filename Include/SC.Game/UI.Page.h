#pragma once

namespace SC::Game::UI
{
	/// <summary> UI 페이지를 표현합니다. </summary>
	class Page : virtual public Object
	{
		friend class Canvas;

		RefPtr<Element> content;

	protected:
		/// <summary> <see cref="Page"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		Page();

		/// <summary> 페이지가 외부에서 탐색되었을 때 호출됩니다. </summary>
		/// <param name="args"> 페이지 탐색 매개변수가 젼달됩니다. </param>
		virtual void OnNavigatedTo( RefPtr<NavigationEventArgs> args );

		/// <summary> 페이지가 외부로 탐색될 때 호출됩니다. </summary>
		/// <param name="args"> 페이지 탐색 매개변수가 젼달됩니다. </param>
		virtual void OnNavigatedFrom( RefPtr<NavigationEventArgs> args );

	public:
		/// <summary> 요소를 갱신합니다. </summary>
		/// <param name="clientRect"> 부모의 컨텐츠 사각 영역이 전달됩니다. </param>
		virtual void Update( Drawing::Rect<double> clientRect );

		/// <summary> 요소를 렌더링합니다. </summary>
		/// <param name="deviceContext"> 플랫폼 렌더링을 위한 장치 컨텍스트가 전달됩니다. </param>
		virtual void Render( RefPtr<Details::CDeviceContext>& deviceContext );

		/// <summary> UI 이벤트를 처리합니다. </summary>
		/// <param name="args"> 이벤트 매개변수가 전달됩니다. </param>
		virtual void ProcessEvent( RefPtr<DispatcherEventArgs> args );

		/// <summary> (Visual Studio 전용) UI 컨텐츠 요소를 설정하거나 가져옵니다. </summary>
		vs_property( RefPtr<Element>, Content );

		/// <summary> UI 컨텐츠 요소를 가져옵니다. </summary>
		RefPtr<Element> Content_get();

		/// <summary> UI 컨텐츠 요소를 설정합니다. </summary>
		void Content_set( RefPtr<Element> value );
	};
}