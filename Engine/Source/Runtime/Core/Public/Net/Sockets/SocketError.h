// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

namespace Ayla
{
	enum class SocketError
	{
		Success = 0,
		SocketError = -1,
		Fault = 10014,
		InvalidArgument = 10022,
		AccessDenied = 10013,
		InvalidOperation = 10045,
		NotSocket = 10038,
		AddressFamilyNotSupported = 10047,
		ProtocolNotSupported = 10043,
		SocketTypeNotSupported = 10044,
		OperationNotSupported = 10045,
		NetworkDown = 10050,
		NetworkUnreachable = 10051,
		NetworkReset = 10052,
		ConnectionAborted = 10053,
		ConnectionReset = 10054,
		NoBufferSpaceAvailable = 10055,
		AlreadyConnected = 10056,
		NotConnected = 10057,
		Shutdown = 10058,
		TimedOut = 10060,
		ConnectionRefused = 10061,
		HostDown = 10064,
		HostUnreachable = 10065,
		ProcessLimit = 10067,
		SystemNotReady = 10091,
		VersionNotSupported = 10092,
		NotInitialized = 10093,
		Disconnecting = 10101,
		OperationAborted = 995,
		IOPending = 997,
		Interrupted = 10004,
		WouldBlock = 10035,
		InProgress = 10036,
		AlreadyInProgress = 10037,
		NotSupported = 10129,
		AddressAlreadyInUse = 10048,
		AddressNotAvailable = 10049,
		MessageSize = 10040
	};
}