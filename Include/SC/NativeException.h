#pragma once

namespace SC
{
	/// <summary> �ۿ��� �������� �ʴ� ����Ƽ�� ���ܰ� �߻��Ͽ��� ��� �߻��ϴ� �����Դϴ�. �� Ŭ������ ��ӵ� �� �����ϴ�. </summary>
	class NativeException final : public Exception
	{
		std::exception_ptr native_ptr;

	public:
		/// <summary> <see cref="NativeException"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="native"> �߻��� ����Ƽ�� ���� �����͸� �����մϴ�. </param>
		NativeException( std::exception_ptr native );

		/// <summary> (Visual Studio ����) ����Ƽ�� ���ܿ� ���� �����͸� �����ɴϴ�. </summary>
		vs_property_get( std::exception_ptr, NativePtr );

		/// <summary> ����Ƽ�� ���ܿ� ���� �����͸� �����ɴϴ�. </summary>
		[[nodiscard]] std::exception_ptr NativePtr_get() const;
	};
}