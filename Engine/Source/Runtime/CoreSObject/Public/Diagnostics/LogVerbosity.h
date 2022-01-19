// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

/// <summary>
/// Represents log verbosity.
/// </summary>
enum class ELogVerbosity
{
	/// <summary>
	/// Fatal error. Process will be terminate.
	/// </summary>
	Fatal,

	/// <summary>
	/// Error occurred. Process keep running but unstable.
	/// </summary>
	Error,

	/// <summary>
	/// Warning occurred. Process will bypassing errors, but have potential risks.
	/// </summary>
	Warning,

	/// <summary>
	/// Display information.
	/// </summary>
	Info,

	/// <summary>
	/// Display verbose logging.
	/// </summary>
	Verbose,
};