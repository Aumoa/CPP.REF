// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:EventHandle;

import std.core;
import :PrimitiveTypes;
import :Object;

using namespace std;
using namespace std::chrono;

/// <summary>
/// The system event handle.
/// </summary>
export class EventHandle : virtual public Object
{
public:
	using Super = Object;

	/// <summary>
	/// Represents waiting status.
	/// </summary>
	enum class EStatus
	{
		/// <summary>
		/// Waiting succeeded.
		/// </summary>
		Succeeded,

		/// <summary>
		/// Waiting is timed out.
		/// </summary>
		Timeout,

		/// <summary>
		/// Waiting operation failed.
		/// </summary>
		Failed,
	};

private:
	void* _handle = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="EventHandle"/> instance.
	/// </summary>
	EventHandle();
	~EventHandle() override;

	/// <summary>
	/// Wait for signal.
	/// </summary>
	/// <param name="timeout"> Thread will be return forcely with failed status at timed out. </param>
	EStatus Wait(milliseconds timeout = 0xFFFFFFFFms);

	/// <summary>
	/// Set event to signal state.
	/// </summary>
	void Set();

	/// <summary>
	/// Reset event from signal state.
	/// </summary>
	void Reset();

	/// <summary>
	/// Get native event handle.
	/// </summary>
	void* GetHandle() const { return _handle; }
};