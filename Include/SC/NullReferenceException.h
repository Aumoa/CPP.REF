#pragma once

namespace SC
{
	/// <summary> Null ���� ������ ��ü�� �������Ϸ��� �� ��� �߻��ϴ� �����Դϴ�. </summary>
	class NullReferenceException : public Exception
	{
	protected:
		/// <summary> <see cref="NullReferenceException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ���� �̸��Դϴ�. </param>
		/// <param name="message"> ���ܿ� ���� ������ �����ϴ� ���� �޽����Դϴ�. </param>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		NullReferenceException( String message, Exception* innerException, String name );

	public:
		/// <summary> <see cref="NullReferenceException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		NullReferenceException( Exception* innerException = nullptr );

		/// <summary> <see cref="NullReferenceException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="message"> ���ܿ� ���� ������ �����ϴ� ���� �޽����Դϴ�. </param>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		NullReferenceException( String message, Exception* innerException = nullptr );
	};
}