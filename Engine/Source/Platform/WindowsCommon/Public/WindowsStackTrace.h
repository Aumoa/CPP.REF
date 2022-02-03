// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "WindowsMinimal.h"

class WINDOWSCOMMON_API WindowsStackTrace
{
	WindowsStackTrace(const WindowsStackTrace&) = delete;
	WindowsStackTrace& operator =(const WindowsStackTrace&) = delete;

public:
	struct ModuleDesc
	{
		std::wstring ImageName;
		std::wstring ModuleName;
		void* BaseAddress;
		DWORD LoadSize;
	};

	class SymbolInfo
	{
        static constexpr const size_t MaxNameLength = 1024;
        std::vector<uint8> SymbolBuf;

    private:
        IMAGEHLP_SYMBOL64* GetSymbolPtr();
        const IMAGEHLP_SYMBOL64* GetSymbolPtr() const;

    public:
        SymbolInfo(HANDLE hProcess, DWORD64 BaseAddr);

        bool IsSymbolLoaded() const;
        std::wstring GetName() const;
        std::wstring GetUndecoratedName() const;
		DWORD64 GetDllBaseAddress() const;
	};

	struct Callstack
	{
		std::unique_ptr<SymbolInfo> Symbol;
        ModuleDesc* Module;
		DWORD64 Counter;

		int32 FrameNumber;
        int32 LineNumber;
        std::wstring FileName;

		std::wstring ModuleName;
		std::wstring FunctionName;
		std::wstring SourceLocation;
	};

private:
	HANDLE hProcess;
	HANDLE hThread;
	DWORD ProcessId;

	LPEXCEPTION_POINTERS Exception;

	std::vector<ModuleDesc> LoadedModule;
	std::vector<Callstack> Callstacks;

public:
	WindowsStackTrace(LPEXCEPTION_POINTERS InExceptions);
	~WindowsStackTrace();

	const std::vector<Callstack>& GetCallstacks() const;

private:
	void SetupSymbolOptions();
	void TraceStack();

private:
	void* LoadModuleSymbols();
    ModuleDesc* FindModule(DWORD64 Addr);
};