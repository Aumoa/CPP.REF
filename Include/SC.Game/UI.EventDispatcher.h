#pragma once

namespace SC::Game::UI
{
	/// <summary> UI ���� ��ҿ� ��ȣ �ۿ� �Է¿� ���� �̺�Ʈ�� ��ġ�մϴ�. �� Ŭ������ ��ӵ� �� �����ϴ�. </summary>
	class EventDispatcher final : virtual public Object
	{
		friend class Canvas;

		std::queue<RefPtr<DispatcherEventArgs>> messageQueue;

	private:
		[[deprecated]] EventDispatcher();

	public:
		/// <summary> UI �����忡 ������ �̺�Ʈ�� �����մϴ�. </summary>
		/// <param name="sender"> �߼��� ��ü�� �����մϴ�. </param>
		/// <param name="args"> �̺�Ʈ �Ű������� �����մϴ�. </param>
		void SendEvent( object sender, RefPtr<DispatcherEventArgs> args );
	};
}