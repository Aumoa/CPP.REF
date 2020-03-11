#pragma once

namespace SC::Threading
{
	class ThreadWorkItem : virtual public Object, virtual public IAsyncOperation<object>
	{
		AsyncTaskDelegate<object> asyncTask;
		AsyncTaskChainDelegate<object> asyncTaskChain;
		object args;

		std::mutex locker;
		RefPtr<Exception> exception;
		AsyncStatus status = AsyncStatus::Pending;
		object results;
		Event resultEvent = Event( true );

		RefPtr<ThreadWorkItem> thenWork;

	public:
		ThreadWorkItem( AsyncTaskDelegate<object> asyncTask, object args )
			: asyncTask( asyncTask )
			, args( args )
		{

		}

		ThreadWorkItem( AsyncTaskChainDelegate<object> asyncTask, object args )
			: asyncTaskChain( asyncTask )
			, args( args )
		{

		}

		String ToString() override;

		void Cancel() override;
		void Close() override;
		int64 Id_get() override;
		RefPtr<Exception> Error_get() override;
		AsyncStatus Status_get() override;

		object GetResults() override;
		RefPtr<IAsyncOperation<object>> Then( AsyncTaskChainDelegate<object> asyncTask, object args ) override;

		void Run();
		void Run( object prevResults );
	};
}