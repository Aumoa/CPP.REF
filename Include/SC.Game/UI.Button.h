#pragma once

namespace SC::Game::UI
{
	/// <summary> ������� Ŭ�� ��ȣ�ۿ뿡 �����ϴ� ��ư ��Ʈ�� UI ����Դϴ�. </summary>
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
		/// <summary> (<see cref="Element"/> Ŭ�������� ��� ��.) ��Ҹ� �����մϴ�. </summary>
		/// <param name="clientRect"> �θ��� ������ �簢 ������ ���޵˴ϴ�. </param>
		Drawing::Rect<double> OnUpdate( Drawing::Rect<double> clientRect ) override;

		/// <summary> (<see cref="Element"/> Ŭ�������� ��� ��.) ��Ҹ� �������մϴ�. </summary>
		/// <param name="deviceContext"> �÷��� �������� ���� ��ġ ���ؽ�Ʈ�� ���޵˴ϴ�. </param>
		void OnRender( RefPtr<Details::CDeviceContext>& deviceContext ) override;

	public:
		/// <summary> <see cref="Button"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ��ü �̸��� �����մϴ�. </param>
		Button( String name );

		/// <summary> (<see cref="Element"/> Ŭ�������� ��� ��.) UI �̺�Ʈ�� ó���մϴ�. </summary>
		/// <param name="args"> �̺�Ʈ �Ű������� ���޵˴ϴ�. </param>
		void ProcessEvent( RefPtr<DispatcherEventArgs> args ) override;

		/// <summary> ��ư�� Ŭ���Ǿ��� �� �߻��ϴ� �̺�Ʈ�Դϴ�. </summary>
		Event<> Clicked;

	private:
		void OnMouseEnterLeave( object sender, bool hover );
		void OnContentChanged( object sender, object content );
		void OnMouseClick( object sender, MouseClickEventArgs args );
	};
}