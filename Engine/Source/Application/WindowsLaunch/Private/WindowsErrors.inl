// Copyright 2020-2021 Aumoa.lib. All right reserved.

DECLARE_LOG_CATEGORY(, LogWindowsLaunch);

[[noreturn]]
inline void ReportWindowsError(std::wstring_view InMessage = L"", int32 Error = (int32)GetLastError())
{
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

	SE_LOG(LogWindowsLaunch, Fatal, L"An error occurred from Windows: {}\nErrorCode: {}, FormattedMessage: {}", InMessage, Error, ScopedBuf);
}

[[noreturn]]
inline void ReportCOMError(std::wstring_view InMessage, HRESULT Error)
{
	_com_error ComError(Error);
	SE_LOG(LogWindowsLaunch, Fatal, L"An error occurred from COM: {}\nErrorCode: 0x{:08X}, FormattedMessage: {}", InMessage, (uint32)Error, TCHAR_TO_WCHAR(ComError.ErrorMessage()));
}