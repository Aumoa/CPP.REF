#pragma once

namespace SC
{
	/// <summary> �Լ��� ������ �μ� �� �ϳ��� ��ȿ���� ���� �� �߻��ϴ� �����Դϴ�. </summary>
	class ArgumentException : public Exception
	{
		String parameterName;

	protected:
		/// <summary> <see cref="ArgumentException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ���� �̸��Դϴ�. </param>
		/// <param name="message"> ���ܿ� ���� ������ �����ϴ� ���� �޽����Դϴ�. </param>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		ArgumentException( String message, Exception* innerException, String name );

		/// <summary> <see cref="ArgumentException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ���� �̸��Դϴ�. </param>
		/// <param name="message"> ���ܿ� ���� ������ �����ϴ� ���� �޽����Դϴ�. </param>
		/// <param name="parameterName"> ���� ���ܸ� �߻���Ų �Ű� ������ �̸��Դϴ�. </param>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		ArgumentException( String message, String parameterName, Exception* innerException, String name );

	public:
		/// <summary> <see cref="ArgumentException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		ArgumentException( Exception* innerException = nullptr );

		/// <summary> <see cref="ArgumentException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="message"> ���ܿ� ���� ������ �����ϴ� ���� �޽����Դϴ�. </param>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		ArgumentException( String message, Exception* innerException = nullptr );

		/// <summary> <see cref="ArgumentException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="message"> ���ܿ� ���� ������ �����ϴ� ���� �޽����Դϴ�. </param>
		/// <param name="parameterName"> ���� ���ܸ� �߻���Ų �Ű� ������ �̸��Դϴ�. </param>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		ArgumentException( String message, String parameterName, Exception* innerException = nullptr );

		/// <summary> (Visual Studio ����) �� ���ܸ� �߻���Ų �Ű� ������ �̸��� �����ɴϴ�. </summary>
		vs_property_get( String, ParamName );

		/// <summary> �� ���ܸ� �߻���Ų �Ű� ������ �̸��� �����ɴϴ�. </summary>
		String ParamName_get();
	};
}