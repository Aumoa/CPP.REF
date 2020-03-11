#pragma once

namespace SC
{
	/// <summary> �޼��� ȣ���� ��ü�� ���� ���¿� ���� ��ȿ���� ���� �� �߻��ϴ� �����Դϴ�. </summary>
	class InvalidOperationException : public Exception
	{
	protected:
		/// <summary> <see cref="InvalidOperationException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ���� �̸��Դϴ�. </param>
		/// <param name="message"> ���ܿ� ���� ������ �����ϴ� ���� �޽����Դϴ�. </param>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		InvalidOperationException( String message, Exception* innerException, String name );

	public:
		/// <summary> <see cref="InvalidOperationException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		InvalidOperationException( Exception* innerException = nullptr );

		/// <summary> <see cref="InvalidOperationException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="message"> ���ܿ� ���� ������ �����ϴ� ���� �޽����Դϴ�. </param>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		InvalidOperationException( String message, Exception* innerException = nullptr );
	};
}