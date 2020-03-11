#pragma once

namespace SC::Game::UI
{
	/// <summary> UI ���� �������� ���� Ÿ���� ������� �ϴ� �ֻ��� ���� ���� �г��� ǥ���մϴ�. �� Ŭ������ ��ӵ� �� �����ϴ�. </summary>
	class Canvas final : public Element
	{
		RefPtr<Page> page;
		RefPtr<Page> nextPage;

	protected:
		/// <summary> (<see cref="Element"/> Ŭ�������� ��� ��.) ��Ҹ� �����մϴ�. </summary>
		/// <param name="clientRect"> �θ��� ������ �簢 ������ ���޵˴ϴ�. </param>
		Drawing::Rect<double> OnUpdate( Drawing::Rect<double> clientRect ) override;

		/// <summary> (<see cref="Element"/> Ŭ�������� ��� ��.) ��Ҹ� �������մϴ�. </summary>
		/// <param name="deviceContext"> �÷��� �������� ���� ��ġ ���ؽ�Ʈ�� ���޵˴ϴ�. </param>
		void OnRender( RefPtr<Details::CDeviceContext>& deviceContext ) override;

	public:
		/// <summary> <see cref="Canvas"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ��ü �̸��� �����մϴ�. </param>
		Canvas( String name );

		/// <summary> (<see cref="Element"/> Ŭ�������� ��� ��.) UI �̺�Ʈ�� ó���մϴ�. </summary>
		/// <param name="args"> �̺�Ʈ �Ű������� ���޵˴ϴ�. </param>
		void ProcessEvent( RefPtr<DispatcherEventArgs> args ) override;

		/// <summary> �������� ǥ�� ������ ������ �������� �����մϴ�. </summary>
		void Navigate( RefPtr<Page> page );
	};
}