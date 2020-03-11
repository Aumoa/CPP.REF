#pragma once

namespace SC
{
	/// <summary> ���� ���� ����Ͽ� �� �ν��Ͻ��� ����� ���� �Լ��� ���� ������ �����մϴ�. </summary>
	struct ICloneable : virtual public Object
	{
		/// <summary> ���� �ν��Ͻ��� ���纻�� �� ��ü�� �����մϴ�. </summary>
		virtual object Clone() = 0;
	};
}