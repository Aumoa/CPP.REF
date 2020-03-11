#pragma once

namespace SC::Game::UI
{
	/// <summary> 사용자의 클릭 상호작용에 응답하는 버튼 컨트롤 UI 요소입니다. </summary>
	class Button : public Element
	{
	public:
		using ContentType = String;

	private:
		RefPtr<Border> border;
		RefPtr<TextBlock> buttonText;

		RefPtr<Element> innerElement;

		bool push = false;
		bool hover = false;

	protected:
		/// <summary> (<see cref="Element"/> 클래스에서 상속 됨.) 요소를 갱신합니다. </summary>
		/// <param name="clientRect"> 부모의 컨텐츠 사각 영역이 전달됩니다. </param>
		Drawing::Rect<double> OnUpdate( Drawing::Rect<double> clientRect ) override;

		/// <summary> (<see cref="Element"/> 클래스에서 상속 됨.) 요소를 렌더링합니다. </summary>
		/// <param name="deviceContext"> 플랫폼 렌더링을 위한 장치 컨텍스트가 전달됩니다. </param>
		void OnRender( RefPtr<Details::CDeviceContext>& deviceContext ) override;

	public:
		/// <summary> <see cref="Button"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 개체 이름을 전달합니다. </param>
		Button( String name );

		/// <summary> (<see cref="Element"/> 클래스에서 상속 됨.) UI 이벤트를 처리합니다. </summary>
		/// <param name="args"> 이벤트 매개변수가 전달됩니다. </param>
		void ProcessEvent( RefPtr<DispatcherEventArgs> args ) override;

		/// <summary> 버튼이 클릭되었을 때 발생하는 이벤트입니다. </summary>
		Event<> Clicked;

	private:
		void OnMouseEnterLeave( object sender, bool hover );
		void OnContentChanged( object sender, object content );
		void OnMouseClick( object sender, MouseClickEventArgs args );
	};
}