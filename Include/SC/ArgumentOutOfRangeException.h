#pragma once

namespace SC
{
	/// <summary> �μ� ���� ȣ��� �޼��忡�� ������ ���� ��� ������ ��� �� �߻��ϴ� �����Դϴ�. </summary>
	class ArgumentOutOfRangeException : public ArgumentException
	{
	protected:
		/// <summary> <see cref="ArgumentNullException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ���� �̸��Դϴ�. </param>
		/// <param name="message"> ���ܿ� ���� ������ �����ϴ� ���� �޽����Դϴ�. </param>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		ArgumentOutOfRangeException( String message, Exception* innerException, String name );

	public:
		/// <summary> <see cref="ArgumentNullException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		ArgumentOutOfRangeException( Exception* innerException = nullptr );

		/// <summary> <see cref="ArgumentNullException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="message"> ���ܿ� ���� ������ �����ϴ� ���� �޽����Դϴ�. </param>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		ArgumentOutOfRangeException( String message, Exception* innerException = nullptr );
	};
}