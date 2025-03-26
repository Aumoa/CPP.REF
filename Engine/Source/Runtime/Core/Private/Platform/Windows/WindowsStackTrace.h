// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__

#include "String_.h"
#include "Threading/Spinlock.h"
#include "Platform/PlatformCommon.h"
#include "Platform/PlatformMacros.h"
#include "Diagnostics/StackFrame.h"
#include "Platform/PlatformLocalization.h"
#include <map>
#include <optional>

namespace Ayla
{
	struct ModuleInfo
	{
		String ImageName;
		String ModuleName;
		void* BaseAddress;
		size_t LoadSize;
	};

	class SymbolInstaller
	{
		HANDLE hProcess = NULL;
		bool bInitialized = false;

		std::map<HMODULE, ModuleInfo> LoadedModules;
		std::vector<HMODULE> LoadBuf;
		void* BaseModuleAddr = nullptr;

	public:
		SymbolInstaller() noexcept
			: LoadBuf(1)
		{
			hProcess = GetCurrentProcess();
			bInitialized = (bool)SymInitializeW(hProcess, L"", FALSE);
		}

		~SymbolInstaller() noexcept
		{
			if (bInitialized)
			{
				SymCleanup(hProcess);
				bInitialized = false;
			}
		}

		inline PIMAGE_NT_HEADERS ReloadSymbols() noexcept
		{
			// Enum modules with first handle, for getting required size.
			DWORD RequiredSize;
			if (!EnumProcessModules(hProcess, LoadBuf.data(), sizeof(HMODULE), &RequiredSize))
			{
				return nullptr;
			}

			LoadBuf.resize(RequiredSize / sizeof(HMODULE));
			if (!EnumProcessModules(hProcess, LoadBuf.data(), RequiredSize, &RequiredSize))
			{
				return nullptr;
			}

			if (BaseModuleAddr == nullptr && !LoadBuf.empty())
			{
				BaseModuleAddr = this->ConfigureInfo(LoadBuf.front());
				if (BaseModuleAddr == nullptr)
				{
					return nullptr;
				}
			}

			// Getting module descriptions.
			for (auto& hModule : LoadBuf)
			{
				this->ConfigureInfo(hModule);
			}

			return ImageNtHeader(BaseModuleAddr);
		}

		inline bool IsInitialized() const noexcept
		{
			return bInitialized;
		}

		inline HANDLE GetHandle() const noexcept
		{
			return hProcess;
		}

		inline const ModuleInfo* FindModule(size_t Addr)
		{
			for (auto& [hModule, Info] : LoadedModules)
			{
				if ((size_t)Info.BaseAddress <= Addr && ((size_t)Info.BaseAddress + Info.LoadSize) > Addr)
				{
					return &Info;
				}
			}

			return nullptr;
		}

	private:
		void* ConfigureInfo(HMODULE hModule)
		{
			if (auto It = LoadedModules.find(hModule); It != LoadedModules.end())
			{
				return It->second.BaseAddress;
			}

			MODULEINFO Info;
			if (!GetModuleInformation(hProcess, hModule, &Info, sizeof(Info)))
			{
				return nullptr;
			}

			CHAR FilenameBuf[1024];
			if (!GetModuleFileNameA(hModule, FilenameBuf, ARRAYSIZE(FilenameBuf)))
			{
				return nullptr;
			}

			CHAR BasenameBuf[1024];
			if (!GetModuleBaseNameA(hProcess, hModule, BasenameBuf, ARRAYSIZE(BasenameBuf)))
			{
				return nullptr;
			}

			if (SymLoadModule64(hProcess, nullptr, FilenameBuf, BasenameBuf, (DWORD64)Info.lpBaseOfDll, Info.SizeOfImage) == 0)
			{
				return nullptr;
			}

			LoadedModules.emplace(hModule, ModuleInfo
				{
					.ImageName = String(FilenameBuf),
					.ModuleName = String(BasenameBuf),
					.BaseAddress = Info.lpBaseOfDll,
					.LoadSize = (size_t)Info.SizeOfImage
				});

			return Info.lpBaseOfDll;
		}
	};

	static inline Spinlock Lck;
	static inline std::optional<SymbolInstaller> SymbolInst;

	class SymbolInfo
	{
		static constexpr const size_t MAX_NAME_LENGTH = 1024;
		std::vector<uint8> SymbolBuf;

	public:
		inline SymbolInfo(DWORD64 InAddr) noexcept
			: SymbolBuf(sizeof(IMAGEHLP_SYMBOL64) + sizeof(CHAR) * MAX_NAME_LENGTH)
		{
			IMAGEHLP_SYMBOL64& Symb = this->GetRef();
			Symb.SizeOfStruct = sizeof(Symb);
			Symb.Address = InAddr;
			Symb.MaxNameLength = (DWORD)MAX_NAME_LENGTH;

			DWORD64 disp;
			if (!SymGetSymFromAddr64(SymbolInst->GetHandle(), InAddr, &disp, &Symb))
			{
				this->SymbolBuf.clear();
			}
		}

		[[nodiscard]]
		inline bool IsLoaded() const noexcept
		{
			return !this->SymbolBuf.empty();
		}

		[[nodiscard]]
		inline String GetSymbolName() const noexcept
		{
			if (this->SymbolBuf.empty())
			{
				return TEXT("<Unknown Symbol>");
			}
			else
			{
				return String((LPSTR)this->GetRef().Name);
			}
		}

		[[nodiscard]]
		inline String GetUndecoratedName() const noexcept
		{
			if (this->SymbolBuf.empty())
			{
				return TEXT("<Unknown Symbol>");
			}

			static thread_local std::vector<char> sUndeco;
			sUndeco.clear();
			sUndeco.resize(MAX_NAME_LENGTH);

			DWORD Len = UnDecorateSymbolName(this->GetRef().Name, sUndeco.data(), (DWORD)MAX_NAME_LENGTH, UNDNAME_COMPLETE);
			if (Len == 0)
			{
				return String(this->GetRef().Name);
			}

			return String(sUndeco.data(), (size_t)Len);
		}

		[[nodiscard]]
		inline size_t GetSymbolBaseAddr() const noexcept
		{
			return this->GetRef().Address;
		}

	public:
		inline const IMAGEHLP_SYMBOL64& GetRef() const noexcept
		{
			return *reinterpret_cast<const IMAGEHLP_SYMBOL64*>(this->SymbolBuf.data());
		}

		inline IMAGEHLP_SYMBOL64& GetRef() noexcept
		{
			return *reinterpret_cast<IMAGEHLP_SYMBOL64*>(this->SymbolBuf.data());
		}
	};

	inline PIMAGE_NT_HEADERS ImplSymbolLoad() noexcept
	{
		if (!SymbolInst.has_value())
		{
			SymbolInst.emplace();
		}
		return SymbolInst->ReloadSymbols();
	}

	inline std::vector<StackFrame> ImplStacktrace(PIMAGE_NT_HEADERS Image, HANDLE hThread, CONTEXT* InContext) noexcept
	{
		if (Image == nullptr)
		{
			return {};
		}

		std::vector<StackFrame> StackFrames;

		CONTEXT Context = *InContext;
		STACKFRAME64 S = {};
		S.AddrPC.Offset = Context.Rip;
		S.AddrPC.Mode = AddrModeFlat;
		S.AddrStack.Offset = Context.Rsp;
		S.AddrStack.Mode = AddrModeFlat;
		S.AddrFrame.Offset = Context.Rbp;
		S.AddrFrame.Mode = AddrModeFlat;
		DWORD ImageType = Image->FileHeader.Machine;
		HANDLE hProcess = SymbolInst->GetHandle();

		StackFrames.reserve(128);
		int32 Skip = 1;
		while (StackWalk64(ImageType, hProcess, hThread, &S, &Context, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr)
			&& S.AddrReturn.Offset != 0)
		{
			if (Skip-- > 0)
			{
				continue;
			}

			SymbolInfo SI(S.AddrPC.Offset);
			const ModuleInfo* MI = SymbolInst->FindModule(SI.GetSymbolBaseAddr());

			StackFrame Frame;
			if (MI)
			{
				Frame.Module = MI->ModuleName;
			}

			Frame.Description = SI.GetUndecoratedName();

			DWORD Disp;
			IMAGEHLP_LINE64 Line;
			if (SymGetLineFromAddr64(hProcess, S.AddrPC.Offset, &Disp, &Line))
			{
				Frame.Location = String(Line.FileName);
				Frame.Line = Line.LineNumber;

				if (Line.LineNumber > 1'000'000)
				{
					// Compiler-generated frame.
					continue;
				}
			}

			Frame.AddressOf = (int64)S.AddrPC.Offset;
			StackFrames.emplace_back(Frame);
		}

		return StackFrames;
	}

	inline std::vector<StackFrame> Capture(HANDLE hThread, CONTEXT* InContext) noexcept
	{
		std::unique_lock Lock(Lck);
		PIMAGE_NT_HEADERS Image = ImplSymbolLoad();
		return ImplStacktrace(Image, hThread, InContext);
	}
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif