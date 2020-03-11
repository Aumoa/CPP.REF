#pragma once

namespace SC::Game::UI
{
	/// <summary> UI 구성 요소에 상호 작용 입력에 대한 이벤트를 배치합니다. 이 클래스는 상속될 수 없습니다. </summary>
	class EventDispatcher final : virtual public Object
	{
		friend class Canvas;

		std::queue<RefPtr<DispatcherEventArgs>> messageQueue;

	private:
		[[deprecated]] EventDispatcher();

	public:
		/// <summary> UI 스레드에 지정한 이벤트를 전송합니다. </summary>
		/// <param name="sender"> 발송자 개체를 전달합니다. </param>
		/// <param name="args"> 이벤트 매개변수를 전달합니다. </param>
		void SendEvent( object sender, RefPtr<DispatcherEventArgs> args );
	};
}