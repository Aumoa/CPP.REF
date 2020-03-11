#pragma once

namespace SC::Threading
{
	/// <summary> �񵿱� �۾��� �Ϸ�Ǿ��� ��� �߻��Ǵ� �̺�Ʈ�� ���� �̺�Ʈ �Ű������� �����մϴ�. </summary>
	template< class TResult >
	class AsyncOperationCompletedEventArgs : public EventArgs
	{
	public:
		/// <summary> (Visual Studio ����) �񵿱� �۾� ����� ���� �������̽��� �����ɴϴ�. </summary>
		vs_property_get( RefPtr<IAsyncOperation<TResult>>, AsyncInfo );

		/// <summary> (Visual Studio ����) �񵿱� �۾� ����� ���� �������̽��� �����ɴϴ�. </summary>
		virtual RefPtr<IAsyncOperation<TResult>> AsyncInfo_get() const = 0;
	};
}