#pragma once

namespace SC
{
	/// <summary> �߸��� ĳ������ �����Ͽ��� ��� �߻��ϴ� �����Դϴ�. </summary>
	class InvalidCastException : public Exception
	{
	protected:
		/// <summary> <see cref="InvalidCastException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ���� �̸��Դϴ�. </param>
		/// <param name="message"> ���ܿ� ���� ������ �����ϴ� ���� �޽����Դϴ�. </param>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		InvalidCastException( String message, Exception* innerException, String name );

	public:
		/// <summary> <see cref="InvalidCastException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		InvalidCastException( Exception* innerException = nullptr );

		/// <summary> <see cref="InvalidCastException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="message"> ���ܿ� ���� ������ �����ϴ� ���� �޽����Դϴ�. </param>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		InvalidCastException( String message, Exception* innerException = nullptr );
	};
}