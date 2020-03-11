#pragma once

#pragma warning( disable: 4250 )

#include <utility>
#include <sstream>
#include <iomanip>
#include <vector>
#include <functional>
#include <type_traits>
#include <mutex>
#include <atomic>
#include <charconv>
#include <string>
#include <any>
#include <map>
#include <algorithm>

// PRERROCESSOR
#include "SC/property.h"
#include "SC/where.h"
#include "SC/callback.h"
#include "SC/Details.HasOperator.h"

namespace SC
{
	namespace Details
	{
		struct ReferenceCounter;
	}

	using int8 = __int8;
	using int16 = __int16;
	using int32 = __int32;
	using int64 = __int64;
	using uint8 = unsigned __int8;
	using uint16 = unsigned __int16;
	using uint32 = unsigned __int32;
	using uint64 = unsigned __int64;

	// CORE
	namespace Details
	{
		struct ReferenceCounter;
	}
	class Object;
	template< class T > class RefPtr;
	class ValueType;
	template< class T > class NativeType;
	struct WeakPtr;
	class String;

	struct Delegate;
	template< class TEventArgs > struct EventHandler;
	template< class TEventArgs > struct Event;
	template< class T > struct NumericType;

	class EventArgs;
	template< class T > class BasicEnumerator;

	namespace Diagnostics
	{
		class StepTimer;
	}

	class Exception;
	class NativeException;
	class ArgumentException;
	class ArgumentNullException;
	class ArgumentOutOfRangeException;
	class NotSupportedException;
	class InvalidCastException;
	class NullReferenceException;
	class InvalidOperationException;
	class NotImplementedException;
}

using object = SC::RefPtr<SC::Object>;

// CORE
#include "SC/Details.ReferenceCounter.h"
#include "SC/Details.StaticAllocator.h"
#include "SC/remove_refptr.h"
#include "SC/RefPtr.h"
#include "SC/Object.h"
#include "SC/where_is.h"
#include "SC/IComparable.h"
#include "SC/IEquatable.h"
#include "SC/IEnumerator.h"
#include "SC/IEnumerable.h"
#include "SC/ICloneable.h"
#include "SC/ValueType.h"
#include "SC/NativeType.h"
#include "SC/WeakPtr.h"
#include "SC/String.h"

// INTERFACE
#include "SC/IDisposable.h"
#include "SC/Collections.IList.h"

// STRUCT
#include "SC/Delegate.h"
#include "SC/EventHandler.h"
#include "SC/Event.h"
#include "SC/NumericType.h"
#include "SC/IntPtr.h"
#include "SC/Diagnostics.StepTimerCallbackDelegate.h"
#include "SC/Drawing.Rect.h"
#include "SC/Drawing.Point.h"
#include "SC/Drawing.Color.h"

// CLASS
#include "SC/EventArgs.h"
#include "SC/BasicEnumerator.h"
#include "SC/Drawing.EaseFunction.h"
#include "SC/Drawing.EaseInOut.h"
#include "SC/Drawing.EaseLinear.h"
#include "SC/Drawing.EaseOut.h"
#include "SC/Drawing.EaseIn.h"
#include "SC/Diagnostics.StepTimer.h"
#include "SC/Collections.ArrayList.h"

// EXCEPTION
#include "SC/Exception.h"
#include "SC/NativeException.h"
#include "SC/ArgumentException.h"
#include "SC/ArgumentNullException.h"
#include "SC/ArgumentOutOfRangeException.h"
#include "SC/NotSupportedException.h"
#include "SC/InvalidCastException.h"
#include "SC/NullReferenceException.h"
#include "SC/InvalidOperationException.h"
#include "SC/NotImplementedException.h"

// SOURCE
#include "SC/Details.StaticAllocator.hpp"
#include "SC/NativeType.hpp"
#include "SC/String.hpp"
#include "SC/RefPtr.hpp"
#include "SC/EventHandler.hpp"
#include "SC/Event.hpp"
#include "SC/NumericType.hpp"
#include "SC/Diagnostics.StepTimerCallbackDelegate.hpp"