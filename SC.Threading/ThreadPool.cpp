using namespace SC;
using namespace SC::Threading;

namespace
{
	DWORD WINAPI StartRoutine( LPVOID lpThreadParameter )
	{
		RefPtr workItem = ( ThreadWorkItem* )lpThreadParameter;
		workItem->IsLocked = false;
		workItem->Run();

		return 0;
	}
}

RefPtr<IAsyncOperation<object>> ThreadPool::QueueUserWorkItem( AsyncTaskDelegate<object> asyncTask, object args )
{
	RefPtr workItem = new ThreadWorkItem( asyncTask, args );
	workItem->IsLocked = true;
	::QueueUserWorkItem( StartRoutine, workItem.Get(), WT_EXECUTEINPERSISTENTTHREAD );
	return workItem;
}

RefPtr<IAsyncOperation<object>> ThreadPool::CreateTask( AsyncTaskDelegate<object> asyncTask, object args )
{
	RefPtr workItem = new ThreadWorkItem( asyncTask, args );
	workItem->IsLocked = true;
	::QueueUserWorkItem( StartRoutine, workItem.Get(), WT_EXECUTELONGFUNCTION );
	return workItem;
}