// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "ScriptingBackend/CoreCLR/CoreCLRScriptingBackend.h"
#include "ScriptingBackend/CoreCLR/coreclrhost.h"
#include "IO/Directory.h"
#include "IO/File.h"
#include "Linq/Concat.h"
#include "Path.h"
#include "Version.h"
#include "InvalidOperationException.h"
#include "Environment.h"

namespace Ayla
{
	namespace
	{
		constexpr int32 RequiredCoreCLRMajorVersion = 9;

		void AddCoreCLRRuntimeDirectoryCandidate(std::vector<String>& candidates, String dotnetRoot)
		{
			if (dotnetRoot.IsEmpty())
			{
				return;
			}

			if (dotnetRoot.EndsWith(TEXT("Microsoft.NETCore.App"), StringComparison::CurrentCultureIgnoreCase))
			{
				candidates.emplace_back(dotnetRoot);
			}
			else
			{
				candidates.emplace_back(Path::Combine(dotnetRoot, TEXT("shared"), TEXT("Microsoft.NETCore.App")));
			}
		}

		std::vector<String> GetCoreCLRRuntimeDirectoryCandidates()
		{
			std::vector<String> candidates;
			AddCoreCLRRuntimeDirectoryCandidate(candidates, Environment::GetEnvironmentVariable(TEXT("DOTNET_ROOT")));

#if PLATFORM_WINDOWS
			AddCoreCLRRuntimeDirectoryCandidate(candidates, TEXT("C:\\Program Files\\dotnet"));
#elif PLATFORM_LINUX
			AddCoreCLRRuntimeDirectoryCandidate(candidates, TEXT("/usr/share/dotnet"));
			AddCoreCLRRuntimeDirectoryCandidate(candidates, TEXT("/usr/lib/dotnet"));
#elif PLATFORM_OSX
			AddCoreCLRRuntimeDirectoryCandidate(candidates, TEXT("/usr/local/share/dotnet"));
			AddCoreCLRRuntimeDirectoryCandidate(candidates, TEXT("/usr/share/dotnet"));
			AddCoreCLRRuntimeDirectoryCandidate(candidates, TEXT("/usr/lib/dotnet"));
#else
#error TODO: Add other platform support.
#endif

			return candidates;
		}

		String ResolveCoreCLRPath()
		{
			bool bFoundRuntime = false;
			Version bestVersion;
			String bestRuntimeDirectory;

			for (const String& runtimeRoot : GetCoreCLRRuntimeDirectoryCandidates())
			{
				if (Directory::Exists(runtimeRoot) == false)
				{
					continue;
				}

				for (const String& versionDirectory : Directory::GetDirectories(runtimeRoot))
				{
					auto versionStr = Path::GetFileName(versionDirectory);
					Version version;
					if (Version::TryParse(versionStr, version) && version.Major == RequiredCoreCLRMajorVersion)
					{
						if (bFoundRuntime == false || version > bestVersion)
						{
							bestVersion = version;
							bestRuntimeDirectory = versionDirectory;
							bFoundRuntime = true;
						}
					}
				}
			}

			if (bFoundRuntime == false)
			{
				throw InvalidOperationException(TEXT("No suitable CoreCLR version found."));
			}

			return Path::Combine(bestRuntimeDirectory, TEXT("coreclr"));
		}
	}

	struct CoreCLRScriptingBackend::Functions
	{
		coreclr_initialize_ptr coreclr_initialize = nullptr;
		coreclr_shutdown_ptr coreclr_shutdown = nullptr;
		coreclr_create_delegate_ptr coreclr_create_delegate = nullptr;
	};

	CoreCLRScriptingBackend::CoreCLRScriptingBackend()
		: ScriptingBackend()
	{
	}

	CoreCLRScriptingBackend::~CoreCLRScriptingBackend() noexcept
	{
		if (m_Hosting)
		{
			m_Hosting->Detach();
		}
	}

	void CoreCLRScriptingBackend::LoadAssembly(String assemblyBasePath, String assemblyName)
	{
		if (m_Hosting)
		{
			throw InvalidOperationException(TEXT("The assembly has been loaded."));
		}

		String coreclr = ResolveCoreCLRPath();

		m_Hosting = std::make_unique<DynamicLibrary>(coreclr);
		if (m_Hosting->IsValid() == false)
		{
			throw InvalidOperationException(TEXT("Failed to load CoreCLR library."));
		}

		m_Functions = std::make_unique<Functions>();
		m_Functions->coreclr_initialize = m_Hosting->LoadDelegate<coreclr_initialize_ptr>(TEXT("coreclr_initialize"));
		m_Functions->coreclr_shutdown = m_Hosting->LoadDelegate<coreclr_shutdown_ptr>(TEXT("coreclr_shutdown"));
		m_Functions->coreclr_create_delegate = m_Hosting->LoadDelegate<coreclr_create_delegate_ptr>(TEXT("coreclr_create_delegate"));
		if (m_Functions->coreclr_initialize == nullptr || m_Functions->coreclr_shutdown == nullptr || m_Functions->coreclr_create_delegate == nullptr)
		{
			throw InvalidOperationException(TEXT("Failed to load CoreCLR hosting functions."));
		}

		std::vector<String> tpaList;
		auto files = Directory::GetFiles(assemblyBasePath, SearchOption::TopDirectoryOnly)
			| Linq::Concat(Directory::GetFiles(Environment::GetCurrentDirectory(), SearchOption::TopDirectoryOnly));
		for (const auto& file : files)
		{
			if (file.EndsWith(TEXT("Script.dll"), StringComparison::CurrentCultureIgnoreCase))
			{
				tpaList.emplace_back(file);
			}
		}

		String coreclrDir = Path::GetDirectoryName(coreclr);
		for (auto& file : Directory::GetFiles(coreclrDir))
		{
			auto filePath = Path::GetFileName(file);
			if ((filePath.StartsWith(TEXT("System.")) || filePath.StartsWith(TEXT("Microsoft."))) && filePath.EndsWith(TEXT(".dll")) && filePath.Contains(TEXT("Native")) == false)
			{
				tpaList.emplace_back(file);
			}
		}

#if PLATFORM_WINDOWS
		String tpaList_s = String::Join(TEXT(";"), tpaList);
#elif PLATFORM_LINUX || PLATFORM_OSX
		String tpaList_s = String::Join(TEXT(":"), tpaList);
#else
#error TODO: Add other platform support.
#endif
		std::string tpaList_a = tpaList_s.string();
		std::string assemblyBasePath_a = assemblyBasePath.string();
		const char* propertyKeys[] =
		{
			"TRUSTED_PLATFORM_ASSEMBLIES",
			"APP_PATHS",
			"APP_NI_PATHS",
			"NATIVE_DLL_SEARCH_DIRECTORIES",
			"System.Globalization.Invariant",
			"System.Diagnostics.Debugger.IsSupported"
		};

		const char* propertyValues[] =
		{
			tpaList_a.c_str(),
			assemblyBasePath_a.c_str(),
			assemblyBasePath_a.c_str(),
			assemblyBasePath_a.c_str(),
			"true",
			"true"
		};

		auto assemblyName_a = assemblyName.string();
		int hr = m_Functions->coreclr_initialize(
			"GameAssembly.dll",
			"DefaultDomain",
			2,
			propertyKeys,
			propertyValues,
			&m_HostHandle,
			&m_DomainId);
		if (hr < 0)
		{
			throw InvalidOperationException(TEXT("coreclr_initialize failed."));
		}
	}

	void CoreCLRScriptingBackend::UnloadAssembly()
	{
		if (m_HostHandle != nullptr)
		{
			m_Functions->coreclr_shutdown(m_HostHandle, m_DomainId);
			m_HostHandle = nullptr;
			m_DomainId = 0;
		}

		m_Functions.reset();
		m_Hosting.reset();
	}

	void* CoreCLRScriptingBackend::GetFunctionPointer(std::string_view assemblyName, std::string_view className, std::string_view methodName)
	{
		if (m_HostHandle == nullptr)
		{
			throw InvalidOperationException(TEXT("The assembly has not been loaded."));
		}

		void* functionPtr = nullptr;
		int hr = m_Functions->coreclr_create_delegate(
			m_HostHandle,
			m_DomainId,
			assemblyName.data(),
			className.data(),
			methodName.data(),
			&functionPtr);
		if (hr < 0 || functionPtr == nullptr)
		{
			String message = String::Format(TEXT("Failed to coreclr_create_delegate: {0}: {1}: {2}"), String::FromLiteral(assemblyName), String::FromLiteral(className), String::FromLiteral(methodName));
			throw InvalidOperationException(message);
		}

		return functionPtr;
	}
}
