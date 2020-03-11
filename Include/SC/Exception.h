#pragma once

namespace SC
{
	/// <summary> �� ���� �� �߻��ϴ� ��� ���ܿ� ���� �⺻ �׸��� �����մϴ�. </summary>
	class Exception : virtual public Object
	{
		String name;
		String message;
		RefPtr<Exception> innerException = nullptr;

	protected:
		/// <summary> <see cref="Exception"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="message"> ���ܿ� ���� ������ �����ϴ� ���� �޽����Դϴ�. </param>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		/// <param name="name"> ���� �̸��Դϴ�. </param>
		Exception( String message, Exception* innerException, String name );

	public:
		/// <summary> <see cref="Exception"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Exception();

		/// <summary> <see cref="Exception"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="message"> ���ܿ� ���� ������ �����ϴ� ���� �޽����Դϴ�. </param>
		/// <param name="innerException"> ���� ���ܸ� �߻���Ų �����Դϴ�. </param>
		Exception( String message, Exception* innerException = nullptr );

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ���ܿ� ���� ��� ������ �ؽ�Ʈ ǥ���� �����ɴϴ�. </summary>
		String ToString() override;

		/// <summary> (Visual Studio ����) ���� ���� �̸��� �����ɴϴ�. </summary>
		vs_property_get( String, Name );

		/// <summary> ���� ���� �̸��� �����ɴϴ�. </summary>
		String Name_get();

		/// <summary> (Visual Studio ����) ���� ���ܸ� �����ϴ� �޽����� �����ɴϴ�. </summary>
		vs_property_get( String, Message );

		/// <summary> ���� ���ܸ� �����ϴ� �޽����� �����ɴϴ�. </summary>
		String Message_get();

		/// <summary> (Visual Studio ����) ���� ���ܸ� �߻���Ų <see cref="SC::Exception"/> ��ü�� �����ɴϴ�. </summary>
		vs_property_get( RefPtr<Exception>, InnerException );

		/// <summary> ���� ���ܸ� �߻���Ų <see cref="Exception"/> ��ü�� �����ɴϴ�. </summary>
		RefPtr<Exception> InnerException_get();
	};
}