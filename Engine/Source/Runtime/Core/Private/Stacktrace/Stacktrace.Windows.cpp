// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Stacktrace/Stacktrace.h"
#include "Misc/PlatformMacros.h"

using namespace ::libty;

#if PLATFORM_WINDOWS

#include "Threading/Spinlock.h"
#include "Threading/Thread.h"
#include <map>

#pragma push_macro("TEXT")
#undef TEXT

#pragma pack(push, 8)
#include <Windows.h>
#include <Psapi.h>
#include <imagehlp.h>
#pragma pack(pop)

#pragma pop_macro("TEXT")

struct Stacktrace::_Stacktrace_impl
{
	struct _Module_info
	{
		String ImageName;
		String ModuleName;
		void* BaseAddress;
		size_t LoadSize;
	};

	class _Symbol_installer
	{
		HANDLE _hProcess = NULL;
		bool _bInitialized = false;

		std::map<HMODULE, _Module_info> _loadedModules;
		std::vector<HMODULE> _loadBuf;
		void* _baseModuleAddr = nullptr;

	public:
		_Symbol_installer() noexcept
			: _loadBuf(1)
		{
			_hProcess = GetCurrentProcess();
			_bInitialized = (bool)SymInitializeW(_hProcess, L"", FALSE);
		}

		~_Symbol_installer() noexcept
		{
			if (_bInitialized)
			{
				SymCleanup(_hProcess);
				_bInitialized = false;
			}
		}

		inline PIMAGE_NT_HEADERS ReloadSymbols() noexcept
		{
			// Enum modules with first handle, for getting required size.
			DWORD requiredSize;
			if (!EnumProcessModules(_hProcess, _loadBuf.data(), sizeof(HMODULE), &requiredSize))
			{
				return nullptr;
			}

			_loadBuf.resize(requiredSize / sizeof(HMODULE));
			if (!EnumProcessModules(_hProcess, _loadBuf.data(), requiredSize, &requiredSize))
			{
				return nullptr;
			}

			if (_baseModuleAddr == nullptr && !_loadBuf.empty())
			{
				_baseModuleAddr = this->_Configure_info(_loadBuf.front());
				if (_baseModuleAddr == nullptr)
				{
					return nullptr;
				}
			}

			// Getting module descriptions.
			for (auto& hModule : _loadBuf)
			{
				this->_Configure_info(hModule);
			}

			return ImageNtHeader(_baseModuleAddr);
		}

		inline bool IsInitialized() const noexcept
		{
			return _bInitialized;
		}

		inline HANDLE GetHandle() const noexcept
		{
			return _hProcess;
		}

		inline const _Module_info* FindModule(size_t Addr)
		{
			for (auto& [hModule, info] : _loadedModules)
			{
				if ((size_t)info.BaseAddress <= Addr && ((size_t)info.BaseAddress + info.LoadSize) > Addr)
				{
					return &info;
				}
			}

			return nullptr;
		}

	private:
		void* _Configure_info(HMODULE hModule)
		{
			if (auto it = _loadedModules.find(hModule); it !=_loadedModules.end())
			{
				return it->second.BaseAddress;
			}

			MODULEINFO info;
			if (!GetModuleInformation(_hProcess, hModule, &info, sizeof(info)))
			{
				return nullptr;
			}

			CHAR filenameBuf[1024];
			if (!GetModuleFileNameA(hModule, filenameBuf, ARRAYSIZE(filenameBuf)))
			{
				return nullptr;
			}

			CHAR basenameBuf[1024];
			if (!GetModuleBaseNameA(_hProcess, hModule, basenameBuf, ARRAYSIZE(basenameBuf)))
			{
				return nullptr;
			}

			if (SymLoadModule64(_hProcess, nullptr, filenameBuf, basenameBuf, (DWORD64)info.lpBaseOfDll, info.SizeOfImage) == 0)
			{
				return nullptr;
			}

			_loadedModules.emplace(hModule, _Module_info
			{
				.ImageName = filenameBuf,
				.ModuleName = basenameBuf,
				.BaseAddress = info.lpBaseOfDll,
				.LoadSize = (size_t)info.SizeOfImage
			});

			return info.lpBaseOfDll;
		}
	};

	static inline Spinlock _lock;
	static inline std::optional<_Symbol_installer> _Symbol_inst;

	class _Symbol_info
	{
		static constexpr const size_t _Max_name_len = 1024;
		std::vector<uint8> _symbol_buf;

	public:
		inline _Symbol_info(DWORD64 inAddr) noexcept
			: _symbol_buf(sizeof(IMAGEHLP_SYMBOL64) + sizeof(CHAR) * _Max_name_len)
		{
			IMAGEHLP_SYMBOL64& symb = this->_Get_ref();
			symb.SizeOfStruct = sizeof(symb);
			symb.Address = inAddr;
			symb.MaxNameLength = (DWORD)_Max_name_len;

			DWORD64 disp;
			if (!SymGetSymFromAddr64(_Symbol_inst->GetHandle(), inAddr, &disp, &symb))
			{
				this->_symbol_buf.clear();
			}
		}

		[[nodiscard]]
		inline bool IsLoaded() const noexcept
		{
			return !this->_symbol_buf.empty();
		}

		[[nodiscard]]
		inline String GetSymbolName() const noexcept
		{
			if (this->_symbol_buf.empty())
			{
				return TEXT("<Unknown Symbol>");
			}
			else
			{
				return (LPSTR)this->_Get_ref().Name;
			}
		}

		[[nodiscard]]
		inline String GetUndecoratedName() const noexcept
		{
			if (this->_symbol_buf.empty())
			{
				return TEXT("<Unknown Symbol>");
			}

			static thread_local std::vector<char> sUndeco;
			sUndeco.clear();
			sUndeco.resize(_Max_name_len);

			DWORD len = UnDecorateSymbolName(this->_Get_ref().Name, sUndeco.data(), (DWORD)_Max_name_len, UNDNAME_COMPLETE);
			if (len == 0)
			{
				return this->_Get_ref().Name;
			}

			return String(sUndeco.data(), (size_t)len);
		}

		[[nodiscard]]
		inline size_t GetSymbolBaseAddr() const noexcept
		{
			return this->_Get_ref().Address;
		}

	public:
		inline const IMAGEHLP_SYMBOL64& _Get_ref() const noexcept
		{
			return *reinterpret_cast<const IMAGEHLP_SYMBOL64*>(this->_symbol_buf.data());
		}

		inline IMAGEHLP_SYMBOL64& _Get_ref() noexcept
		{
			return *reinterpret_cast<IMAGEHLP_SYMBOL64*>(this->_symbol_buf.data());
		}
	};

	HANDLE _hThread;
	CONTEXT _threadCtx;
	std::vector<Stackframe> _stackframes;

	inline void CaptureCurrent() noexcept
	{
		PIMAGE_NT_HEADERS image = this->_Impl_symbol_load();

		_hThread = GetCurrentThread();
		RtlCaptureContext(&_threadCtx);
		this->_Impl_stacktrace(image);
	}

	inline void Capture(HANDLE hThread) noexcept
	{
		PIMAGE_NT_HEADERS image = this->_Impl_symbol_load();

		SuspendThread(hThread);
		_hThread = hThread;
		GetThreadContext(hThread, &_threadCtx);
		this->_Impl_stacktrace(image);
		ResumeThread(hThread);
	}

	inline void CaptureException(LPEXCEPTION_POINTERS lpExceptionPointers) noexcept
	{
		PIMAGE_NT_HEADERS image = this->_Impl_symbol_load();

		_hThread = GetCurrentThread();
		_threadCtx = *lpExceptionPointers->ContextRecord;
		this->_Impl_stacktrace(image);
	}

private:
	inline PIMAGE_NT_HEADERS _Impl_symbol_load() noexcept
	{
		std::unique_lock lock(_lock);
		if (!_Symbol_inst.has_value())
		{
			_Symbol_inst.emplace();
		}
		return _Symbol_inst->ReloadSymbols();
	}

	inline void _Impl_stacktrace(PIMAGE_NT_HEADERS image) noexcept
	{
		if (image == nullptr)
		{
			return;
		}

		STACKFRAME64 S = {};
		S.AddrPC.Offset = _threadCtx.Rip;
		S.AddrPC.Mode = AddrModeFlat;
		S.AddrStack.Offset = _threadCtx.Rsp;
		S.AddrStack.Mode = AddrModeFlat;
		S.AddrFrame.Offset = _threadCtx.Rbp;
		S.AddrFrame.Mode = AddrModeFlat;
		DWORD imageType = image->FileHeader.Machine;
		HANDLE hProcess = _Symbol_inst->GetHandle();

		_stackframes.reserve(128);
		while (StackWalk64(imageType, hProcess, _hThread, &S, &_threadCtx, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr)
			&& S.AddrReturn.Offset != 0)
		{
			_Symbol_info si(S.AddrPC.Offset);
			const _Module_info* mi = _Symbol_inst->FindModule(si.GetSymbolBaseAddr());

			Stackframe& frame = _stackframes.emplace_back();
			if (mi)
			{
				frame.ModuleName = mi->ModuleName;
			}

			frame.FunctionName = si.GetUndecoratedName();

			DWORD disp;
			IMAGEHLP_LINE64 line;
			if (SymGetLineFromAddr64(hProcess, S.AddrPC.Offset, &disp, &line))
			{
				frame.FileName = line.FileName;
				frame.Line = line.LineNumber;
			}

			frame.AddressOf = S.AddrPC.Offset;
		}
	}
};

Stacktrace::Stacktrace() noexcept
	: _impl(std::make_shared<_Stacktrace_impl>())
{
}

std::span<const Stackframe> Stacktrace::GetStackframes() const noexcept
{
	return _impl->_stackframes;
}

Stacktrace Stacktrace::CaptureCurrent() noexcept
{
	Stacktrace s;
	s._impl->CaptureCurrent();
	return s;
}

Stacktrace Stacktrace::Capture(Thread* thread) noexcept
{
	Stacktrace s;
	s._impl->Capture((HANDLE)thread->GetNativeHandle());
	return s;
}

Stacktrace Stacktrace::CaptureException(void* exception_pointer) noexcept
{
	Stacktrace s;
	s._impl->CaptureException((LPEXCEPTION_POINTERS)exception_pointer);
	return s;
}

#endif