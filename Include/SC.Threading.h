#pragma once

#include "SC.h"

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <atomic>

/* Enum */
#include "SC.Threading/AsyncStatus.h"
#include "SC.Threading/AsyncWaitResult.h"

namespace SC::Threading
{
	struct IAsyncInfo;
	template< class TResult > struct IAsyncOperation;

	class AsyncWait;
	class Event;
	template< class TResult > class AsyncOperationCompletedEventArgs;
	class ThreadPool;
}

/* Delegate */
#include "SC.Threading/AsyncTaskDelegate.h"

/* Interface */
#include "SC.Threading/IAsyncInfo.h"
#include "SC.Threading/IAsyncOperation.h"

/* Class */
#include "SC.Threading/AsyncWait.h"
#include "SC.Threading/Event.h"
#include "SC.Threading/AsyncOperationCompletedEventArgs.h"
#include "SC.Threading/ThreadPool.h"