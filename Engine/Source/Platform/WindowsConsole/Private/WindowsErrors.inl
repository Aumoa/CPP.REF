// Copyright 2020-2021 Aumoa.lib. All right reserved.

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

	SE_LOG(LogWindowsConsole, Fatal, L"An error occurred from Windows: {}\nErrorCode: {}, FormattedMessage: {}", InMessage, Error, ScopedBuf);
}