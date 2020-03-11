#pragma once

namespace SC
{
	/// <summary> ��û�� �޼��� �Ǵ� ������ �������� ���� �� �߻��ϴ� �����Դϴ�. </summary>
	class NotImplementedException : public Exception
	{
	protected:
		/// <summary> <see cref="NotImplementedException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="message"> ���ܿ� ���� ������ �����ϴ� ���� �޽����Դϴ�. </param>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		/// <param name="name"> ���� �̸��Դϴ�. </param>
		NotImplementedException( String message, Exception* innerException, String name );

	public:
		/// <summary> <see cref="NotImplementedException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		NotImplementedException( Exception* innerException = nullptr );

		/// <summary> <see cref="NotImplementedException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="message"> ���ܿ� ���� ������ �����ϴ� ���� �޽����Դϴ�. </param>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		NotImplementedException( String message, Exception* innerException = nullptr );
	};
}