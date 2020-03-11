#pragma once

namespace SC
{
	/// <summary> ȣ���� ����� �������� �ʴ� ��ü�� ���� �۾��� �����Ϸ��� ���� ��� �߻��ϴ� �����Դϴ�. </summary>
	class NotSupportedException : public Exception
	{
	protected:
		/// <summary> <see cref="NotSupportedException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="message"> ���ܿ� ���� ������ �����ϴ� ���� �޽����Դϴ�. </param>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		/// <param name="name"> ���� �̸��Դϴ�. </param>
		NotSupportedException( String message, Exception* innerException, String name );

	public:
		/// <summary> <see cref="NotSupportedException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		NotSupportedException( Exception* innerException = nullptr );

		/// <summary> <see cref="NotSupportedException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="message"> ���ܿ� ���� ������ �����ϴ� ���� �޽����Դϴ�. </param>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		NotSupportedException( String message, Exception* innerException = nullptr );
	};
}