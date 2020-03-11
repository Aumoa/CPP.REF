using namespace SC;
using namespace SC::Threading;

using namespace std;

void ThreadWorkItem::Cancel()
{
	lock_guard<mutex> lock( locker );
	if ( status == AsyncStatus::Started )
	{
		throw new NotSupportedException( "ThreadPool work already start. Started work cannot be canceled." );
	}

	if ( status == AsyncStatus::Pending )
	{
		status = AsyncStatus::Canceled;
		asyncTask = nullptr;
		asyncTaskChain = nullptr;
		args = nullptr;
		Canceled( this );
	}
}

String ThreadWorkItem::ToString()
{
	return String::Format( "SC.Threading.ThreadWorkItem({0})", status );
}

void ThreadWorkItem::Close()
{
	throw new NotSupportedException( "ThreadPool work thread cannot be closed." );
}

int64 ThreadWorkItem::Id_get()
{
	throw new NotSupportedException( "ThreadPool work thread cannot have id." );
}

RefPtr<Exception> ThreadWorkItem::Error_get()
{
	return exception;
}

AsyncStatus ThreadWorkItem::Status_get()
{
	return status;
}

object ThreadWorkItem::GetResults()
{
	if ( status != AsyncStatus::Completed )
	{
		resultEvent.WaitForSingleObject();
	}

	return results;
}

RefPtr<IAsyncOperation<object>> ThreadWorkItem::Then( AsyncTaskChainDelegate<object> asyncTask, object args )
{
	locker.lock();
	if ( status == AsyncStatus::Completed )
	{
		locker.unlock();
		AsyncTaskDelegate<object> task = bind( asyncTask, results, placeholders::_1 );
		return ThreadPool::QueueUserWorkItem( task, args );
	}
	else
	{
		thenWork = new ThreadWorkItem( asyncTask, args );
		locker.unlock();
		return thenWork;
	}
}

void ThreadWorkItem::Run()
{
	try
	{
		locker.lock();
		if ( asyncTask )
		{
			status = AsyncStatus::Started;
			locker.unlock();
			results = asyncTask( args );

			locker.lock();
			status = AsyncStatus::Completed;
			resultEvent.Set();

			auto thenWork = this->thenWork;
			locker.unlock();

			if ( thenWork.Get() )
			{
				thenWork->Run( results );
			}
		}
		else
		{
			locker.unlock();
		}
	}
	catch ( Exception * e )
	{
		exception = e;
		status = AsyncStatus::Error;
	}
}

void ThreadWorkItem::Run( object prevResults )
{
	try
	{
		locker.lock();
		if ( asyncTaskChain )
		{
			status = AsyncStatus::Started;
			locker.unlock();
			results = asyncTaskChain( prevResults, args );

			locker.lock();
			status = AsyncStatus::Completed;
			resultEvent.Set();

			auto thenWork = this->thenWork;
			locker.unlock();

			if ( thenWork )
			{
				thenWork->Run( results );
			}
		}
		else
		{
			locker.unlock();
		}
	}
	catch ( Exception * e )
	{
		exception = e;
		status = AsyncStatus::Error;
	}
}