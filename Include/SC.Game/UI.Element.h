#pragma once

namespace SC::Game::UI
{
	/// <summary> UI 요소 형식 계층 구조의 루트입니다. 파생 클래스에 UI 기본 기능을 제공합니다. </summary>
	class Element : public DependencyObject, virtual public IEquatable<RefPtr<Element>>, virtual public IEquatable<String>
	{
		friend class Application;

		double width = 0;
		double height = 0;
		Thickness margin;

		Drawing::Rect<double> actualRenderingRect;

		object content;
		String name;
		Anchor anchor;

		bool hovered = false;

	protected:
		/// <summary> <see cref="Element"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 개체 이름을 전달합니다. </param>
		/// <param name="createFontRenderingDevice"> 폰트 렌더링 장치를 생성합니다. <see cref="Content"/> 속성에 <see cref="String"/> 개체가 설정될 경우 텍스트 렌더링을 진행합니다. </param>
		/// <param name="noBorder"> 경계 UI 요소를 생성하지 않습니다. </param>
		Element( String name );

		/// <summary> 요소를 갱신합니다. </summary>
		/// <param name="clientRect"> 부모의 컨텐츠 사각 영역이 전달됩니다. </param>
		/// <returns> 요소의 실제 렌더링 영역을 반환합니다. </returns>
		virtual Drawing::Rect<double> OnUpdate( Drawing::Rect<double> clientRect ) = 0;

		/// <summary> 요소를 렌더링합니다. </summary>
		/// <param name="deviceContext"> 플랫폼 렌더링을 위한 장치 컨텍스트가 전달됩니다. </param>
		virtual void OnRender( RefPtr<Details::CDeviceContext>& deviceContext ) = 0;

	public:
		~Element() override;

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 개체의 요약 정보를 텍스트 형식으로 반환합니다. </summary>
		String ToString() override;

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 두 개체의 이름이 일치하는지 검사합니다. </summary>
		/// <param name="obj"> 비교할 개체를 전달합니다. </param>
		bool Equals( object obj ) override;

		/// <summary> (<see cref="IEquatable"/> 인터페이스에서 구현 됨.) 두 개체의 이름이 일치하는지 검사합니다. </summary>
		/// <param name="right"> 비교할 개체를 전달합니다. </param>
		bool Equals( RefPtr<Element> right ) override;

		/// <summary> (<see cref="IEquatable"/> 인터페이스에서 구현 됨.) 개체의 이름과 지정한 텍스트가 일치하는지 검사합니다. </summary>
		/// <param name="right"> 비교할 개체를 전달합니다. </param>
		bool Equals( String right ) override;

		/// <summary> 요소의 갱신을 시작합니다. </summary>
		/// <param name="clientRect"> 요소에서 사용할 사각 영역을 전달합니다. </param>
		void Update( Drawing::Rect<double> clientRect );

		/// <summary> 요소를 렌더링합니다. </summary>
		/// <param name="deviceContext"> 렌더링에 사용할 장치 컨텍스트를 전달합니다. </param>
		void Render( RefPtr<Details::CDeviceContext>& deviceContext );

		/// <summary> UI 이벤트를 처리합니다. </summary>
		/// <param name="args"> 이벤트 매개변수가 전달됩니다. </param>
		virtual void ProcessEvent( RefPtr<DispatcherEventArgs> args );

		/// <summary> (Visual Studio 전용) UI 컨텐츠 요소를 설정하거나 가져옵니다. </summary>
		vs_property( object, Content );

		/// <summary> UI 컨텐츠 요소를 가져옵니다. </summary>
		object Content_get();

		/// <summary> UI 컨텐츠 요소를 설정합니다. </summary>
		void Content_set( object value );

		/// <summary> (Visual Studio 전용) UI 컨텐츠 요소의 너비를 설정하거나 가져옵니다. </summary>
		vs_property( double, Width );

		/// <summary> UI 요소의 너비를 가져옵니다. </summary>
		double Width_get();

		/// <summary> UI 요소의 너비를 설정합니다. </summary>
		/// <param name="value"> 값을 전달합니다. </param>
		void Width_set( double value );

		/// <summary> (Visual Studio 전용) UI 컨텐츠 요소의 높이를 설정하거나 가져옵니다. </summary>
		vs_property( double, Height );

		/// <summary> UI 요소의 높이를 가져옵니다. </summary>
		double Height_get();

		/// <summary> UI 요소의 높이를 설정합니다. </summary>
		/// <param name="value"> 값을 전달합니다. </param>
		void Height_set( double value );

		/// <summary> (Visual Studio 전용) UI 컨텐츠 요소의 렌더링 사각 영역을 가져옵니다. </summary>
		vs_property( Drawing::Rect<double>, ActualContentRect );

		/// <summary> UI 요소의 렌더링 사각 영역을 가져옵니다. </summary>
		Drawing::Rect<double> ActualContentRect_get();

		/// <summary> (Visual Studio 전용) UI 요소의 여분 크기를 설정하거나 가져옵니다. </summary>
		vs_property( Thickness, Margin );

		/// <summary> UI 요소의 여분 크기를 가져옵니다. </summary>
		Thickness Margin_get();

		/// <summary> UI 요소의 여분 크기를 설정합니다. </summary>
		void Margin_set( Thickness value );

		/// <summary> (Visual Studio 전용) 요소의 이름을 설정하거나 가져옵니다. </summary>
		vs_property( String, Name );

		/// <summary> 요소의 이름을 가져옵니다. </summary>
		String Name_get();

		/// <summary> 요소의 이름을 설정합니다. </summary>
		void Name_set( String value );

		/// <summary> (Visual Studio 전용) 요소의 기준 위치를 설정하거나 가져옵니다. </summary>
		vs_property( UI::Anchor, Anchor );

		/// <summary> 요소의 기준 위치를 가져옵니다. </summary>
		UI::Anchor Anchor_get();

		/// <summary> 요소의 기준 위치를 설정합니다. </summary>
		void Anchor_set( UI::Anchor value );

		/// <summary> 요소의 컨텐츠 개체가 변경되었을 경우 발생하는 이벤트입니다. </summary>
		Event<object> ContentChanged;

		/// <summary> 요소에 커서가 포함되거나 제외되었을 경우 발생하는 이벤트입니다. </summary>
		Event<bool> MouseEnterLeave;

		/// <summary> 요소 내부에서 커서가 이동될 경우 발생하는 이벤트입니다. </summary>
		Event<Drawing::Point<double>> MouseMove;

		/// <summary> 요소의 컨텐츠 크기가 변경되었을 경우 발생하는 이벤트입니다. </summary>
		Event<Drawing::Point<double>> ContentSizing;

		/// <summary> 요소 내부에서 마우스 클릭이 일어났을 경우 발생하는 이벤트입니다. </summary>
		Event<UI::MouseClickEventArgs> MouseClick;

	private:
		void OnMouseMove( MouseMoveEventArgs args );
		void OnMouseClick( MouseClickEventArgs args );
	};
}