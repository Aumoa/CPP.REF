#pragma once

namespace SC::Game::UI
{
	/// <summary> UI �������� ǥ���մϴ�. </summary>
	class Page : virtual public Object
	{
		friend class Canvas;

		RefPtr<Element> content;

	protected:
		/// <summary> <see cref="Page"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Page();

		/// <summary> �������� �ܺο��� Ž���Ǿ��� �� ȣ��˴ϴ�. </summary>
		/// <param name="args"> ������ Ž�� �Ű������� ���޵˴ϴ�. </param>
		virtual void OnNavigatedTo( RefPtr<NavigationEventArgs> args );

		/// <summary> �������� �ܺη� Ž���� �� ȣ��˴ϴ�. </summary>
		/// <param name="args"> ������ Ž�� �Ű������� ���޵˴ϴ�. </param>
		virtual void OnNavigatedFrom( RefPtr<NavigationEventArgs> args );

	public:
		/// <summary> ��Ҹ� �����մϴ�. </summary>
		/// <param name="clientRect"> �θ��� ������ �簢 ������ ���޵˴ϴ�. </param>
		virtual void Update( Drawing::Rect<double> clientRect );

		/// <summary> ��Ҹ� �������մϴ�. </summary>
		/// <param name="deviceContext"> �÷��� �������� ���� ��ġ ���ؽ�Ʈ�� ���޵˴ϴ�. </param>
		virtual void Render( RefPtr<Details::CDeviceContext>& deviceContext );

		/// <summary> UI �̺�Ʈ�� ó���մϴ�. </summary>
		/// <param name="args"> �̺�Ʈ �Ű������� ���޵˴ϴ�. </param>
		virtual void ProcessEvent( RefPtr<DispatcherEventArgs> args );

		/// <summary> (Visual Studio ����) UI ������ ��Ҹ� �����ϰų� �����ɴϴ�. </summary>
		vs_property( RefPtr<Element>, Content );

		/// <summary> UI ������ ��Ҹ� �����ɴϴ�. </summary>
		RefPtr<Element> Content_get();

		/// <summary> UI ������ ��Ҹ� �����մϴ�. </summary>
		void Content_set( RefPtr<Element> value );
	};
}