#pragma once

namespace SC
{
	/// <summary> ����� �޸� ���� ����� �ʿ��� ��ü���� ������ ��ü�� �����ϴ� �Լ��� �������̽��� ǥ���մϴ�. </summary>
	struct IDisposable : virtual public Object
	{
		/// <summary> ���� �ڿ��� �����մϴ�. </summary>
		virtual void Dispose() = 0;
	};
}