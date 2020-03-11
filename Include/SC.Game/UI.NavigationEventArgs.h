#pragma once

namespace SC::Game::UI
{
	/// <summary> Ž�� ��û�� ����� �� ���� Ž�� �Լ� �� �̺�Ʈ �ڵ鷯�� ���� �Ű������� �����մϴ�. </summary>
	class NavigationEventArgs : public EventArgs
	{
		RefPtr<Page> pageFrom;
		RefPtr<Page> pageTo;

	public:
		/// <summary> <see cref="NavigationEventArgs"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="pageFrom"> Ž�� ���� �������� �����մϴ�. </param>
		/// <param name="pageTo"> Ž�� ���� �������� �����մϴ�. </param>
		NavigationEventArgs( RefPtr<Page> pageFrom, RefPtr<Page> pageTo );

		/// <summary> (Visual Studio ����) Ž�� ���� �������� �����ɴϴ�. </summary>
		vs_property_get( RefPtr<Page>, PageFrom );

		/// <summary> Ž�� ���� �������� �����ɴϴ�. </summary>
		RefPtr<Page> PageFrom_get();

		/// <summary> (Visual Studio ����) Ž�� ���� �������� �����ɴϴ�. </summary>
		vs_property_get( RefPtr<Page>, PageTo );

		/// <summary> Ž�� ���� �������� �����ɴϴ�. </summary>
		RefPtr<Page> PageTo_get();
	};
}