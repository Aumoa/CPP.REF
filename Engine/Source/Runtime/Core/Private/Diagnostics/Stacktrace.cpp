// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Diagnostics/Stacktrace.h"

#if PLATFORM_WINDOWS
#include "Misc/WindowsPlatformMisc.h"
#include "Threading/Spinlock.h"
#include <map>

struct Stacktrace::_stacktrace_impl
{
	struct _module_info
	{
		String ImageName;
		String ModuleName;
		void* BaseAddress;
		size_t LoadSize;
	};

	class _symbol_installer
	{
		HANDLE _hProcess = NULL;
		bool _bInitialized = false;

		std::map<HMODULE, _module_info> _loadedModules;
		std::vector<HMODULE> _loadBuf;
		void* _baseModuleAddr = nullptr;

	public:
		_symbol_installer() noexcept
			: _loadBuf(1)
		{
			_hProcess = GetCurrentProcess();
			_bInitialized = (bool)SymInitializeW(_hProcess, L"", FALSE);
		}

		~_symbol_installer() noexcept
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
				_baseModuleAddr = this->_configure_info(_loadBuf.front());
				if (_baseModuleAddr == nullptr)
				{
					return nullptr;
				}
			}

			// Getting module descriptions.
			for (auto& hModule : _loadBuf)
			{
				this->_configure_info(hModule);
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

		inline const _module_info* FindModule(size_t Addr)
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
		void* _configure_info(HMODULE hModule)
		{
			if (auto it = _loadedModules.find(hModule); it != _loadedModules.end())
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

			_loadedModules.emplace(hModule, _module_info
				{
					.ImageName = String(filenameBuf),
					.ModuleName = String(basenameBuf),
					.BaseAddress = info.lpBaseOfDll,
					.LoadSize = (size_t)info.SizeOfImage
				});

			return info.lpBaseOfDll;
		}
	};

	static inline Spinlock _lock;
	static inline std::optional<_symbol_installer> _symbol_inst;

	class _symbol_info
	{
		static constexpr const size_t _Max_name_len = 1024;
		std::vector<uint8> _symbol_buf;

	public:
		inline _symbol_info(DWORD64 inAddr) noexcept
			: _symbol_buf(sizeof(IMAGEHLP_SYMBOL64) + sizeof(CHAR) * _Max_name_len)
		{
			IMAGEHLP_SYMBOL64& symb = this->_Get_ref();
			symb.SizeOfStruct = sizeof(symb);
			symb.Address = inAddr;
			symb.MaxNameLength = (DWORD)_Max_name_len;

			DWORD64 disp;
			if (!SymGetSymFromAddr64(_symbol_inst->GetHandle(), inAddr, &disp, &symb))
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
				return String((LPSTR)this->_Get_ref().Name);
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
				return String(this->_Get_ref().Name);
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

	std::vector<Stackframe> _stackframes;

	inline _stacktrace_impl& Capture(HANDLE hThread, CONTEXT* context) noexcept
	{
		PIMAGE_NT_HEADERS image = this->_impl_symbol_load();
		this->_impl_stacktrace(image, hThread, context);
		return *this;
	}

private:
	inline PIMAGE_NT_HEADERS _impl_symbol_load() noexcept
	{
		std::unique_lock lock(_lock);
		if (!_symbol_inst.has_value())
		{
			_symbol_inst.emplace();
		}
		return _symbol_inst->ReloadSymbols();
	}

	inline void _impl_stacktrace(PIMAGE_NT_HEADERS image, HANDLE hThread, CONTEXT* context) noexcept
	{
		if (image == nullptr)
		{
			return;
		}

		CONTEXT lContext = *context;
		STACKFRAME64 S = {};
		S.AddrPC.Offset = lContext.Rip;
		S.AddrPC.Mode = AddrModeFlat;
		S.AddrStack.Offset = lContext.Rsp;
		S.AddrStack.Mode = AddrModeFlat;
		S.AddrFrame.Offset = lContext.Rbp;
		S.AddrFrame.Mode = AddrModeFlat;
		DWORD imageType = image->FileHeader.Machine;
		HANDLE hProcess = _symbol_inst->GetHandle();

		_stackframes.reserve(128);
		int32 skip = 1;
		while (StackWalk64(imageType, hProcess, hThread, &S, &lContext, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr)
			&& S.AddrReturn.Offset != 0)
		{
			if (skip-- > 0)
			{
				continue;
			}

			_symbol_info si(S.AddrPC.Offset);
			const _module_info* mi = _symbol_inst->FindModule(si.GetSymbolBaseAddr());

			Stackframe frame;
			if (mi)
			{
				frame._module = mi->ModuleName;
			}

			frame._description = si.GetUndecoratedName();

			DWORD disp;
			IMAGEHLP_LINE64 line;
			if (SymGetLineFromAddr64(hProcess, S.AddrPC.Offset, &disp, &line))
			{
				frame._location = String(line.FileName);
				frame._line = line.LineNumber;

				if (line.LineNumber > 1'000'000)
				{
					// Compiler-generated frame.
					continue;
				}
			}

			frame._address = (int64)S.AddrPC.Offset;
			_stackframes.emplace_back(frame);
		}
	}
};

Stacktrace Stacktrace::Current() noexcept
{
	HANDLE hThread = GetCurrentThread();

	CONTEXT context;
	RtlCaptureContext(&context);

	Stacktrace s;
	s._frames = std::move(_stacktrace_impl().Capture(hThread, &context)._stackframes);
	return s;
}

Stacktrace Stacktrace::FromThread(std::thread& thr) noexcept
{
	HANDLE hThread = (HANDLE)thr.native_handle();
	SuspendThread(hThread);

	CONTEXT context;
	GetThreadContext(hThread, &context);

	Stacktrace s;
	s._frames = std::move(_stacktrace_impl().Capture(hThread, &context)._stackframes);

	ResumeThread(hThread);
	return s;
}

Stacktrace Stacktrace::FromException(void* lpExceptionPointers) noexcept
{
	Stacktrace s;
	s._frames = std::move(_stacktrace_impl().Capture(GetCurrentThread(), ((LPEXCEPTION_POINTERS)lpExceptionPointers)->ContextRecord)._stackframes);
	return s;
}

#endif

Stacktrace::Stacktrace() noexcept
{
}

std::span<const Stackframe> Stacktrace::GetStackframes() const noexcept
{
	return _frames;
}

String Stacktrace::ToString() const noexcept
{
	std::vector<String> ss;
	for (auto& frame : _frames)
	{
		ss.emplace_back(String::Format(TEXT("  at {}!{} in {}({})"), frame.GetModuleName(), frame.GetDescription(), frame.GetSourceLocation(), frame.GetSourceLine()));
	}

	return String::Join(TEXT("\n"), ss);
}