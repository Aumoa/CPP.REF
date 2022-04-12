// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Diagnostics/LogSystem.h"
#include "Misc/String.h"

[[noreturn]]
inline void ReportWindowsError(std::wstring_view InMessage = L"", int32 Error = (int32)GetLastError())
{
	using namespace ::libty;

	WCHAR* Buf = nullptr;
	std::wstring ScopedBuf;
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, 0, (DWORD)Error, 0, (WCHAR*)&Buf, 0, nullptr);
	if (Buf)
	{
		ScopedBuf = Buf;
		LocalFree(Buf);
	}
	else
	{
		ScopedBuf = L"<Unknown>";
	}

	throw FatalException(String::Format("An error occurred from Windows: {}\nErrorCode: {}, FormattedMessage: {}", String::AsMultibyte(InMessage), Error, String::AsMultibyte(ScopedBuf)));
}

[[noreturn]]
inline void ReportCOMError(std::wstring_view InMessage, HRESULT Error)
{
	using namespace ::libty;

	_com_error ComError(Error);
	throw FatalException(String::Format("An error occurred from COM: {}\nErrorCode: 0x{:08X}, FormattedMessage: {}", String::AsMultibyte(InMessage), (uint32)Error, String::AsMultibyte(ComError.ErrorMessage())));
}

#define HR(x) \
if (HRESULT __HR__ = (x); __HR__ != DXGI_ERROR_NOT_FOUND && FAILED(__HR__)) \
{ \
	ReportCOMError(L ## #x, __HR__); \
}