#pragma once

namespace SC::Threading
{
	template< class >
	class AsyncOperationCompletedEventArgs;

	/// <summary> 완료 시 결과를 반환하는 비동기 작업에 대해 반환 결과를 제공합니다. </summary>
	template< class TResult >
	struct IAsyncOperation : virtual public Object, virtual public IAsyncInfo
	{
		/// <summary> 이 작업의 결과 값을 가져옵니다. </summary>
		virtual TResult GetResults() = 0;

		/// <summary> 이 작업 이후에 실행될 작업을 연결합니다. </summary>
		virtual RefPtr<IAsyncOperation<TResult>> Then( AsyncTaskChainDelegate<TResult> asyncTask, object args ) = 0;

		/// <summary> 작업이 취소되었을 경우 발생하는 이벤트입니다. </summary>
		SC::Event<> Canceled;
	};
}