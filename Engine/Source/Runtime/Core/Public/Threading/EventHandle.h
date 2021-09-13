// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include <chrono>

/// <summary>
/// The system event handle.
/// </summary>
class CORE_API SEventHandle : virtual public SObject
{
public:
	using Super = SObject;

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
	/// Initialize new <see cref="SEventHandle"/> instance.
	/// </summary>
	SEventHandle();
	~SEventHandle() override;

	/// <summary>
	/// Wait for signal.
	/// </summary>
	/// <param name="timeout"> Thread will be return forcely with failed status at timed out. </param>
	EStatus Wait(std::chrono::milliseconds timeout = std::chrono::milliseconds(0xFFFFFFFF));

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