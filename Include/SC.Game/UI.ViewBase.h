#pragma once

namespace SC::Game::UI
{
	/// <summary> ���� �׸��� �����ϱ� ���� View ��ҿ� ���� �⺻ �Լ��� �����մϴ�. </summary>
	class ViewBase : public Panel
	{
	protected:
		/// <summary> <see cref="ViewBase"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ��ü �̸��� �����մϴ�. </param>
		ViewBase( String name );
	};
}