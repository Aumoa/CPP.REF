#pragma once

namespace SC::Game::UI
{
	/// <summary> UI 계층 구조에서 렌더 타겟을 대상으로 하는 최상위 계층 구조 패널을 표현합니다. 이 클래스는 상속될 수 없습니다. </summary>
	class Canvas final : public Element
	{
		RefPtr<Page> page;
		RefPtr<Page> nextPage;

	protected:
		/// <summary> (<see cref="Element"/> 클래스에서 상속 됨.) 요소를 갱신합니다. </summary>
		/// <param name="clientRect"> 부모의 컨텐츠 사각 영역이 전달됩니다. </param>
		Drawing::Rect<double> OnUpdate( Drawing::Rect<double> clientRect ) override;

		/// <summary> (<see cref="Element"/> 클래스에서 상속 됨.) 요소를 렌더링합니다. </summary>
		/// <param name="deviceContext"> 플랫폼 렌더링을 위한 장치 컨텍스트가 전달됩니다. </param>
		void OnRender( RefPtr<Details::CDeviceContext>& deviceContext ) override;

	public:
		/// <summary> <see cref="Canvas"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 개체 이름을 전달합니다. </param>
		Canvas( String name );

		/// <summary> (<see cref="Element"/> 클래스에서 상속 됨.) UI 이벤트를 처리합니다. </summary>
		/// <param name="args"> 이벤트 매개변수가 전달됩니다. </param>
		void ProcessEvent( RefPtr<DispatcherEventArgs> args ) override;

		/// <summary> 프레임의 표시 내용을 지정한 페이지로 설정합니다. </summary>
		void Navigate( RefPtr<Page> page );
	};
}