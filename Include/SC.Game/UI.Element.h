#pragma once

namespace SC::Game::UI
{
	/// <summary> UI ��� ���� ���� ������ ��Ʈ�Դϴ�. �Ļ� Ŭ������ UI �⺻ ����� �����մϴ�. </summary>
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
		/// <summary> <see cref="Element"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ��ü �̸��� �����մϴ�. </param>
		/// <param name="createFontRenderingDevice"> ��Ʈ ������ ��ġ�� �����մϴ�. <see cref="Content"/> �Ӽ��� <see cref="String"/> ��ü�� ������ ��� �ؽ�Ʈ �������� �����մϴ�. </param>
		/// <param name="noBorder"> ��� UI ��Ҹ� �������� �ʽ��ϴ�. </param>
		Element( String name );

		/// <summary> ��Ҹ� �����մϴ�. </summary>
		/// <param name="clientRect"> �θ��� ������ �簢 ������ ���޵˴ϴ�. </param>
		/// <returns> ����� ���� ������ ������ ��ȯ�մϴ�. </returns>
		virtual Drawing::Rect<double> OnUpdate( Drawing::Rect<double> clientRect ) = 0;

		/// <summary> ��Ҹ� �������մϴ�. </summary>
		/// <param name="deviceContext"> �÷��� �������� ���� ��ġ ���ؽ�Ʈ�� ���޵˴ϴ�. </param>
		virtual void OnRender( RefPtr<Details::CDeviceContext>& deviceContext ) = 0;

	public:
		~Element() override;

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ��ü�� ��� ������ �ؽ�Ʈ �������� ��ȯ�մϴ�. </summary>
		String ToString() override;

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) �� ��ü�� �̸��� ��ġ�ϴ��� �˻��մϴ�. </summary>
		/// <param name="obj"> ���� ��ü�� �����մϴ�. </param>
		bool Equals( object obj ) override;

		/// <summary> (<see cref="IEquatable"/> �������̽����� ���� ��.) �� ��ü�� �̸��� ��ġ�ϴ��� �˻��մϴ�. </summary>
		/// <param name="right"> ���� ��ü�� �����մϴ�. </param>
		bool Equals( RefPtr<Element> right ) override;

		/// <summary> (<see cref="IEquatable"/> �������̽����� ���� ��.) ��ü�� �̸��� ������ �ؽ�Ʈ�� ��ġ�ϴ��� �˻��մϴ�. </summary>
		/// <param name="right"> ���� ��ü�� �����մϴ�. </param>
		bool Equals( String right ) override;

		/// <summary> ����� ������ �����մϴ�. </summary>
		/// <param name="clientRect"> ��ҿ��� ����� �簢 ������ �����մϴ�. </param>
		void Update( Drawing::Rect<double> clientRect );

		/// <summary> ��Ҹ� �������մϴ�. </summary>
		/// <param name="deviceContext"> �������� ����� ��ġ ���ؽ�Ʈ�� �����մϴ�. </param>
		void Render( RefPtr<Details::CDeviceContext>& deviceContext );

		/// <summary> UI �̺�Ʈ�� ó���մϴ�. </summary>
		/// <param name="args"> �̺�Ʈ �Ű������� ���޵˴ϴ�. </param>
		virtual void ProcessEvent( RefPtr<DispatcherEventArgs> args );

		/// <summary> (Visual Studio ����) UI ������ ��Ҹ� �����ϰų� �����ɴϴ�. </summary>
		vs_property( object, Content );

		/// <summary> UI ������ ��Ҹ� �����ɴϴ�. </summary>
		object Content_get();

		/// <summary> UI ������ ��Ҹ� �����մϴ�. </summary>
		void Content_set( object value );

		/// <summary> (Visual Studio ����) UI ������ ����� �ʺ� �����ϰų� �����ɴϴ�. </summary>
		vs_property( double, Width );

		/// <summary> UI ����� �ʺ� �����ɴϴ�. </summary>
		double Width_get();

		/// <summary> UI ����� �ʺ� �����մϴ�. </summary>
		/// <param name="value"> ���� �����մϴ�. </param>
		void Width_set( double value );

		/// <summary> (Visual Studio ����) UI ������ ����� ���̸� �����ϰų� �����ɴϴ�. </summary>
		vs_property( double, Height );

		/// <summary> UI ����� ���̸� �����ɴϴ�. </summary>
		double Height_get();

		/// <summary> UI ����� ���̸� �����մϴ�. </summary>
		/// <param name="value"> ���� �����մϴ�. </param>
		void Height_set( double value );

		/// <summary> (Visual Studio ����) UI ������ ����� ������ �簢 ������ �����ɴϴ�. </summary>
		vs_property( Drawing::Rect<double>, ActualContentRect );

		/// <summary> UI ����� ������ �簢 ������ �����ɴϴ�. </summary>
		Drawing::Rect<double> ActualContentRect_get();

		/// <summary> (Visual Studio ����) UI ����� ���� ũ�⸦ �����ϰų� �����ɴϴ�. </summary>
		vs_property( Thickness, Margin );

		/// <summary> UI ����� ���� ũ�⸦ �����ɴϴ�. </summary>
		Thickness Margin_get();

		/// <summary> UI ����� ���� ũ�⸦ �����մϴ�. </summary>
		void Margin_set( Thickness value );

		/// <summary> (Visual Studio ����) ����� �̸��� �����ϰų� �����ɴϴ�. </summary>
		vs_property( String, Name );

		/// <summary> ����� �̸��� �����ɴϴ�. </summary>
		String Name_get();

		/// <summary> ����� �̸��� �����մϴ�. </summary>
		void Name_set( String value );

		/// <summary> (Visual Studio ����) ����� ���� ��ġ�� �����ϰų� �����ɴϴ�. </summary>
		vs_property( UI::Anchor, Anchor );

		/// <summary> ����� ���� ��ġ�� �����ɴϴ�. </summary>
		UI::Anchor Anchor_get();

		/// <summary> ����� ���� ��ġ�� �����մϴ�. </summary>
		void Anchor_set( UI::Anchor value );

		/// <summary> ����� ������ ��ü�� ����Ǿ��� ��� �߻��ϴ� �̺�Ʈ�Դϴ�. </summary>
		Event<object> ContentChanged;

		/// <summary> ��ҿ� Ŀ���� ���Եǰų� ���ܵǾ��� ��� �߻��ϴ� �̺�Ʈ�Դϴ�. </summary>
		Event<bool> MouseEnterLeave;

		/// <summary> ��� ���ο��� Ŀ���� �̵��� ��� �߻��ϴ� �̺�Ʈ�Դϴ�. </summary>
		Event<Drawing::Point<double>> MouseMove;

		/// <summary> ����� ������ ũ�Ⱑ ����Ǿ��� ��� �߻��ϴ� �̺�Ʈ�Դϴ�. </summary>
		Event<Drawing::Point<double>> ContentSizing;

		/// <summary> ��� ���ο��� ���콺 Ŭ���� �Ͼ�� ��� �߻��ϴ� �̺�Ʈ�Դϴ�. </summary>
		Event<UI::MouseClickEventArgs> MouseClick;

	private:
		void OnMouseMove( MouseMoveEventArgs args );
		void OnMouseClick( MouseClickEventArgs args );
	};
}