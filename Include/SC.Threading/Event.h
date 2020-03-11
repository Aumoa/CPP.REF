#pragma once

namespace SC::Threading
{
	/// <summary> ���� �ٸ� ������ �� ����ȭ ���¸� �����մϴ�. </summary>
	class Event : public AsyncWait
	{
		bool isManual = false;

	public:
		/// <summary> <see cref="Event"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="isManual"> ��ȣ ���¸� �������� �����մϴ�. �� ��� WaitFor... �Լ��� ����Ͽ� ����Ͽ��� ��ȣ ���°� ������� �ʽ��ϴ�. </param>
		Event( bool isManual = false );

		/// <summary> �̺�Ʈ�� ��ȣ ���¸� Ȱ��ȭ�մϴ�. </summary>
		void Set();

		/// <summary> �̺�Ʈ�� ��ȣ ���¸� ��Ȱ��ȭ�մϴ�. </summary>
		void Reset();

		/// <summary> (Visual Studio ����) �̺�Ʈ ��ü�� ���� ��ȣ ���¸� �����ɴϴ�. </summary>
		vs_property_get( bool, IsSignaled );

		/// <summary> �̺�Ʈ ��ü�� ���� ��ȣ ���¸� �����ɴϴ�. </summary>
		[[nodiscard]] bool IsSignaled_get() const;
	};
}