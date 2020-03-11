#pragma once

namespace SC::Threading
{
	template< class >
	class AsyncOperationCompletedEventArgs;

	/// <summary> �Ϸ� �� ����� ��ȯ�ϴ� �񵿱� �۾��� ���� ��ȯ ����� �����մϴ�. </summary>
	template< class TResult >
	struct IAsyncOperation : virtual public Object, virtual public IAsyncInfo
	{
		/// <summary> �� �۾��� ��� ���� �����ɴϴ�. </summary>
		virtual TResult GetResults() = 0;

		/// <summary> �� �۾� ���Ŀ� ����� �۾��� �����մϴ�. </summary>
		virtual RefPtr<IAsyncOperation<TResult>> Then( AsyncTaskChainDelegate<TResult> asyncTask, object args ) = 0;

		/// <summary> �۾��� ��ҵǾ��� ��� �߻��ϴ� �̺�Ʈ�Դϴ�. </summary>
		SC::Event<> Canceled;
	};
}