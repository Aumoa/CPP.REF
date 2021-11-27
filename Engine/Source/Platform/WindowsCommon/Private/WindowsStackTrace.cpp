// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "WindowsStackTrace.h"
#include <iostream>

class SymbolScopeInstaller
{
	HANDLE hProcess;
	bool bInitialized;

public:
	SymbolScopeInstaller(HANDLE hProcess, const std::wstring& UserSearchPath = L"")
		: hProcess(hProcess)
		, bInitialized(false)
	{
		const WCHAR* p_UserSearchPath = nullptr;
		if (!UserSearchPath.empty())
		{
			p_UserSearchPath = UserSearchPath.c_str();
		}

		bInitialized = (bool)SymInitializeW(hProcess, p_UserSearchPath, FALSE);
	}

	~SymbolScopeInstaller() noexcept
	{
		if (bInitialized)
		{
			SymCleanup(hProcess);
		}
	}

	bool IsInitialized() const
	{
		return bInitialized;
	}
};

IMAGEHLP_SYMBOL64* WindowsStackTrace::SymbolInfo::GetSymbolPtr()
{
    return reinterpret_cast<IMAGEHLP_SYMBOL64*>(SymbolBuf.data());
}

const IMAGEHLP_SYMBOL64* WindowsStackTrace::SymbolInfo::GetSymbolPtr() const
{
    return reinterpret_cast<const IMAGEHLP_SYMBOL64*>(SymbolBuf.data());
}

WindowsStackTrace::SymbolInfo::SymbolInfo(HANDLE hProcess, DWORD64 BaseAddr)
    : SymbolBuf(sizeof(IMAGEHLP_SYMBOL64) + sizeof(CHAR) * MaxNameLength)
{
    DWORD64 _;

    GetSymbolPtr()->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
    GetSymbolPtr()->Address = BaseAddr;
    GetSymbolPtr()->MaxNameLength = (DWORD)MaxNameLength;

    if (!SymGetSymFromAddr64(hProcess, BaseAddr, &_, GetSymbolPtr()))
    {
        SymbolBuf.clear();
    }
}

bool WindowsStackTrace::SymbolInfo::IsSymbolLoaded() const
{
    return SymbolBuf.size() != 0;
}

std::wstring WindowsStackTrace::SymbolInfo::GetName() const
{
    if (IsSymbolLoaded())
    {
        return ANSI_TO_WCHAR(GetSymbolPtr()->Name);
    }
    else
    {
        return L"<Unknown Symbol>";
    }
}

std::wstring WindowsStackTrace::SymbolInfo::GetUndecoratedName() const
{
    std::string Undeco;
    Undeco.resize(MaxNameLength);

    if (!IsSymbolLoaded())
    {
        return L"<Unknown Symbol>";
    }

    DWORD Len = UnDecorateSymbolName(GetSymbolPtr()->Name, Undeco.data(), (DWORD)MaxNameLength, UNDNAME_COMPLETE);
    if (Len == 0)
    {
        return ANSI_TO_WCHAR(GetSymbolPtr()->Name);
    }

    Undeco.resize(Len);
    return ANSI_TO_WCHAR(Undeco);
}

DWORD64 WindowsStackTrace::SymbolInfo::GetDllBaseAddress() const
{
    if (IsSymbolLoaded())
    {
        return GetSymbolPtr()->Address;
    }
    else
    {
        return 0;
    }
}

WindowsStackTrace::WindowsStackTrace(PEXCEPTION_POINTERS InExceptions)
    : hProcess(GetCurrentProcess())
    , ProcessId(GetCurrentProcessId())
    , Exception(InExceptions)
{
    HANDLE hExceptionThread = GetCurrentThread();
    if (!DuplicateHandle(hProcess, hExceptionThread, hProcess, &hCapturedThread, 0, false, DUPLICATE_SAME_ACCESS))
    {
        ReportWindowsError(L"StackTrace failed. Could not capture exception thread.");
        return;
    }

    TraceStack();
}

WindowsStackTrace::~WindowsStackTrace()
{
}

auto WindowsStackTrace::GetCallstacks() const -> const std::vector<Callstack>&
{
    return Callstacks;
}

void WindowsStackTrace::SetupSymbolOptions()
{
    SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
}

void WindowsStackTrace::TraceStack()
{
    CONTEXT& ThreadCtx = *Exception->ContextRecord;
    SymbolScopeInstaller ScopedInstaller(hProcess);

    SetupSymbolOptions();
    void* BaseSymbolAddr = LoadModuleSymbols();

    STACKFRAME64 S = {};
    S.AddrPC.Offset = ThreadCtx.Rip;
    S.AddrPC.Mode = AddrModeFlat;
    S.AddrStack.Offset = ThreadCtx.Rsp;
    S.AddrStack.Mode = AddrModeFlat;
    S.AddrFrame.Offset = ThreadCtx.Rbp;
    S.AddrFrame.Mode = AddrModeFlat;

    IMAGE_NT_HEADERS* ImageHeader = ImageNtHeader(BaseSymbolAddr);
    DWORD ImageType = ImageHeader->FileHeader.Machine;

    for (int32 FrameNumber = 0;
        StackWalk64(ImageType, hProcess, hCapturedThread, &S, &ThreadCtx, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, NULL) &&
        S.AddrReturn.Offset != 0;
        ++FrameNumber)
    {
        Callstack CurrentFrame = {};
        CurrentFrame.FrameNumber = FrameNumber + 1;
        CurrentFrame.Symbol = std::make_unique<SymbolInfo>(hProcess, S.AddrPC.Offset);
        CurrentFrame.Module = FindModule(CurrentFrame.Symbol->GetDllBaseAddress());
        CurrentFrame.Counter = S.AddrPC.Offset;

        IMAGEHLP_LINE64 Line = { sizeof(IMAGEHLP_LINE64) };
        DWORD _;
        if (SymGetLineFromAddr64(hProcess, S.AddrPC.Offset, &_, &Line))
        {
            CurrentFrame.LineNumber = Line.LineNumber;
            CurrentFrame.FileName = ANSI_TO_WCHAR(Line.FileName);
            CurrentFrame.SourceLocation = std::format(L"{}:{}", CurrentFrame.FileName, CurrentFrame.LineNumber);
        }
        else
        {
            DWORD64 Offset = S.AddrPC.Offset;
            if (CurrentFrame.Module)
            {
                Offset = S.AddrPC.Offset - (DWORD64)CurrentFrame.Module->BaseAddress;
            }

            CurrentFrame.SourceLocation = std::format(L"+{}", Offset);
        }

        if (CurrentFrame.Symbol && CurrentFrame.Symbol->IsSymbolLoaded())
        {
            CurrentFrame.FunctionName = CurrentFrame.Symbol->GetUndecoratedName();
        }

        if (CurrentFrame.Module)
        {
            CurrentFrame.ModuleName = CurrentFrame.Module->ModuleName;
        }

        Callstacks.emplace_back(std::move(CurrentFrame));
    }
}

void* WindowsStackTrace::LoadModuleSymbols()
{
    std::vector<HMODULE> Modules(1);

    // Enum modules with first handle, for getting required size.
    DWORD RequiredSize;
    if (!EnumProcessModules(hProcess, Modules.data(), sizeof(HMODULE), &RequiredSize))
    {
        return nullptr;
    }

    Modules.resize(RequiredSize / sizeof(HMODULE));
    if (!EnumProcessModules(hProcess, Modules.data(), RequiredSize, &RequiredSize))
    {
        return nullptr;
    }

    // Getting module descriptions.
    LoadedModule.reserve(Modules.size());
    for (auto& Module : Modules)
    {
        MODULEINFO Info;
        if (!GetModuleInformation(hProcess, Module, &Info, sizeof(Info)))
        {
            continue;
        }

        CHAR ModuleFilenameBuf[1024];
        if (!GetModuleFileNameA(Module, ModuleFilenameBuf, 1024))
        {
            continue;
        }

        CHAR ModuleBasenameBuf[1024];
        if (!GetModuleBaseNameA(hProcess, Module, ModuleBasenameBuf, 1024))
        {
            continue;
        }

        if (SymLoadModule64(hProcess, nullptr, ModuleFilenameBuf, ModuleBasenameBuf, (DWORD64)Info.lpBaseOfDll, Info.SizeOfImage) == 0)
        {
            continue;
        }

        LoadedModule.emplace_back() =
        {
            .ImageName = ANSI_TO_WCHAR(ModuleFilenameBuf),
            .ModuleName = ANSI_TO_WCHAR(ModuleBasenameBuf),
            .BaseAddress = Info.lpBaseOfDll,
            .LoadSize = Info.SizeOfImage
        };
    }

    if (LoadedModule.size())
    {
        return LoadedModule.front().BaseAddress;
    }
    else
    {
        return nullptr;
    }
}

auto WindowsStackTrace::FindModule(DWORD64 Addr) -> ModuleDesc*
{
    for (auto& Module : LoadedModule)
    {
        if ((DWORD64)Module.BaseAddress <= Addr && ((DWORD64)Module.BaseAddress + Module.LoadSize) > Addr)
        {
            return &Module;
        }
    }

    return nullptr;
}