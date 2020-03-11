#pragma once

namespace SC
{
	/// <summary> Null �����͸� �ùٸ��� ���� �μ��� �޴� �Լ��� Null �����Ͱ� ���޵Ǹ� �߻��ϴ� �����Դϴ�. </summary>
	class ArgumentNullException : public ArgumentException
	{
	protected:
		/// <summary> <see cref="ArgumentNullException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ���� �̸��Դϴ�. </param>
		/// <param name="message"> ���ܿ� ���� ������ �����ϴ� ���� �޽����Դϴ�. </param>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		ArgumentNullException( String message, Exception* innerException, String name );

		/// <summary> <see cref="ArgumentNullException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ���� �̸��Դϴ�. </param>
		/// <param name="message"> ���ܿ� ���� ������ �����ϴ� ���� �޽����Դϴ�. </param>
		/// <param name="parameterName"> ���� ���ܸ� �߻���Ų �Ű� ������ �̸��Դϴ�. </param>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		ArgumentNullException( String message, String parameterName, Exception* innerException, String name );

	public:
		/// <summary> <see cref="ArgumentNullException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		ArgumentNullException( Exception* innerException = nullptr );

		/// <summary> <see cref="ArgumentNullException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="message"> ���ܿ� ���� ������ �����ϴ� ���� �޽����Դϴ�. </param>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		ArgumentNullException( String message, Exception* innerException = nullptr );

		/// <summary> <see cref="ArgumentNullException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="message"> ���ܿ� ���� ������ �����ϴ� ���� �޽����Դϴ�. </param>
		/// <param name="parameterName"> ���� ���ܸ� �߻���Ų �Ű� ������ �̸��Դϴ�. </param>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		ArgumentNullException( String message, String parameterName, Exception* innerException = nullptr );
	};
}