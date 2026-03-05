// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Net/Sockets/SocketException.h"

namespace Ayla
{
	static String GetSocketErrorMessage(SocketError error)
	{
		switch (error)
		{
		// Common errors
		case SocketError::Success:
			return TEXT("The operation completed successfully");
		case SocketError::SocketError:
			return TEXT("Socket error");

		// Windows ERROR_* codes
		case SocketError::HandleEOF:
			return TEXT("Reached the end of the file");
		case SocketError::ErrorNotSupported:
			return TEXT("The request is not supported");
		case SocketError::NetNameDeleted:
			return TEXT("The specified network name is no longer available");
		case SocketError::MoreData:
			return TEXT("More data is available");
		case SocketError::NoMoreItems:
			return TEXT("No more data is available");
		case SocketError::ErrorConnectionRefused:
			return TEXT("The connection was refused by the remote host");
		case SocketError::GracefulDisconnect:
			return TEXT("The connection was gracefully closed");
		case SocketError::ErrorConnectionInvalid:
			return TEXT("The connection is invalid");
		case SocketError::OperationAborted:
			return TEXT("Overlapped operation aborted");
		case SocketError::IOPending:
			return TEXT("Overlapped I/O operation is in progress");

		// WSA errors
		case SocketError::Interrupted:
			return TEXT("Interrupted function call");
		case SocketError::AccessDenied:
			return TEXT("Permission denied");
		case SocketError::Fault:
			return TEXT("Bad address");
		case SocketError::InvalidArgument:
			return TEXT("Invalid argument");
		case SocketError::WouldBlock:
			return TEXT("Resource temporarily unavailable");
		case SocketError::InProgress:
			return TEXT("Operation now in progress");
		case SocketError::AlreadyInProgress:
			return TEXT("Operation already in progress");
		case SocketError::NotSocket:
			return TEXT("Socket operation on non-socket");
		case SocketError::MessageSize:
			return TEXT("Message too long");
		case SocketError::ProtocolNotSupported:
			return TEXT("Protocol not supported");
		case SocketError::SocketTypeNotSupported:
			return TEXT("Socket type not supported");
		case SocketError::InvalidOperation: // Same value as OperationNotSupported (10045)
			return TEXT("Invalid operation / Operation not supported");
		case SocketError::AddressFamilyNotSupported:
			return TEXT("Address family not supported by protocol family");
		case SocketError::AddressAlreadyInUse:
			return TEXT("Address already in use");
		case SocketError::AddressNotAvailable:
			return TEXT("Cannot assign requested address");
		case SocketError::NetworkDown:
			return TEXT("Network is down");
		case SocketError::NetworkUnreachable:
			return TEXT("Network is unreachable");
		case SocketError::NetworkReset:
			return TEXT("Network dropped connection on reset");
		case SocketError::ConnectionAborted:
			return TEXT("Software caused connection abort");
		case SocketError::ConnectionReset:
			return TEXT("Connection reset by peer");
		case SocketError::NoBufferSpaceAvailable:
			return TEXT("No buffer space available");
		case SocketError::AlreadyConnected:
			return TEXT("Socket is already connected");
		case SocketError::NotConnected:
			return TEXT("Socket is not connected");
		case SocketError::Shutdown:
			return TEXT("Cannot send after socket shutdown");
		case SocketError::TimedOut:
			return TEXT("Connection timed out");
		case SocketError::ConnectionRefused:
			return TEXT("Connection refused");
		case SocketError::HostDown:
			return TEXT("Host is down");
		case SocketError::HostUnreachable:
			return TEXT("No route to host");
		case SocketError::ProcessLimit:
			return TEXT("Too many processes");
		case SocketError::SystemNotReady:
			return TEXT("Network subsystem is unavailable");
		case SocketError::VersionNotSupported:
			return TEXT("Winsock.dll version out of range");
		case SocketError::NotInitialized:
			return TEXT("Successful WSAStartup not yet performed");
		case SocketError::Disconnecting:
			return TEXT("Graceful shutdown in progress");
		case SocketError::NotSupported:
			return TEXT("The attempted operation is not supported for the type of object referenced");
		default:
			return TEXT("Unknown socket error");
		}
	}

	SocketException::SocketException(SocketError error)
		: Exception(String::Format(TEXT("{0} ({1})"), GetSocketErrorMessage(error), static_cast<int32>(error)))
	{
	}
}