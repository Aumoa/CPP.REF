#pragma once

namespace SC::Game
{
	/// <summary> ���� ������ �ڻ� �ҷ����� ���¸� ǥ���ϴ� �������̽� ��ü�Դϴ�. </summary>
	struct IAsyncLoad : virtual public Object, virtual public Threading::IAsyncOperation<object>
	{
		/// <summary> (Visual Studio ����) �۾��� ���� ���¸� 0���� 1������ �Ǽ��� ��Ÿ���� ���� �����ϰų� �����ɴϴ�. ���� ������ Load �Լ��� �Ű����������� ����� �� �ֽ��ϴ�. </summary>
		vs_property( double, Progress );

		/// <summary> �۾��� ���� ���¸� 0���� 1������ �Ǽ��� ��Ÿ���� ���� �����ɴϴ�. </summary>
		virtual double Progress_get() = 0;

		/// <summary> �۾��� ���� ���¸� 0���� 1������ �Ǽ��� ��Ÿ���� ���� �����մϴ�. Load �Լ��� �Ű����������� ����� �� �ֽ��ϴ�. </summary>
		virtual void Progress_set( double value ) = 0;
	};
}