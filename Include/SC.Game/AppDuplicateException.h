#pragma once

namespace SC::Game
{
	/// <summary> �� ��ü�� �� ���� �ʰ��Ͽ� �����Ǿ��� ��� �߻��ϴ� �����Դϴ�. </summary>
	class AppDuplicateException : public Exception
	{
	public:
		/// <summary> <see cref="AppDuplicateException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		AppDuplicateException( Exception* innerException = nullptr );
	};
}