#pragma once

namespace SC
{
	/// <summary> �̺�Ʈ �����͸� �����ϴ� Ŭ������ �⺻ Ŭ������ ��Ÿ����, �̺�Ʈ �����͸� �������� �ʴ� �̺�Ʈ�� ����� ���� �����մϴ�. </summary>
	class EventArgs : virtual public Object
	{
	public:
		/// <summary> <see cref="EventArgs"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		EventArgs();

		/// <summary> (<see cref="SC::Object"/> Ŭ�������� ��� ��.) ���� ��ü�� �ؽ�Ʈ ǥ���� �����Ͽ� ��ȯ�մϴ�. </summary>
		String ToString() override;
	};
}