#pragma once

namespace SC::Game
{
	/// <summary> ���� �÷������� �����ϴ� ����� ���� �Լ��� ����մϴ�. �� Ŭ������ ��ӵ� �� �����ϴ�. </summary>
	class Debug abstract final : virtual public SC::Object
	{
	public:
		/// <summary> �ؽ�Ʈ �α׸� �߻���ŵ�ϴ�. </summary>
		/// <param name="logText"> �ؽ�Ʈ ��ȯ ������ ��ü�� �����մϴ�. </param>
		static void Log( object logText );
	};
}