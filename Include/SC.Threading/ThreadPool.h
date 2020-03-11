#pragma once

namespace SC::Threading
{
	/// <summary> 작업 실행, 작업 항목 게시, 비동기 I/O 처리, 다른 스레드 대신 기다리기 및 타이머 처리 등에 사용할 수 있는 스레드 풀 함수를 제공합니다. </summary>
	class ThreadPool : virtual public Object
	{
	public:
		/// <summary> 실행을 위해 비동기 함수를 스레드 큐에 대기시킵니다. </summary>
		/// <param name="asyncTask"> 실행할 비동기 함수를 전달합니다. </param>
		/// <param name="args"> 비동기 함수에 전달할 매개변수를 전달합니다. </param>
		/// <remarks> 이 함수는 비교적 간단한 실행 명령에 최적화되어 있습니다. 비교적 오랜 시간 작업하는 스레드를 연결할 경우 CreateTask 함수를 사용하세요. </remarks>
		static RefPtr<IAsyncOperation<object>> QueueUserWorkItem( AsyncTaskDelegate<object> asyncTask, object args );

		/// <summary> 실행을 위해 비동기 함수를 스레드 큐에 대기시킵니다. </summary>
		/// <param name="asyncTask"> 실행할 비동기 함수를 전달합니다. </param>
		/// <param name="args"> 비동기 함수에 전달할 매개변수를 전달합니다. </param>
		/// <remarks> 이 함수는 비교적 오랜 시간 실행 명령에 최적화되어 있습니다. 간단한 실행 명령을 사용하는 스레드를 연결할 경우 QueueUserWorkItem 함수를 사용하세요. </remarks>
		static RefPtr<IAsyncOperation<object>> CreateTask( AsyncTaskDelegate<object> asyncTask, object args );
	};
}