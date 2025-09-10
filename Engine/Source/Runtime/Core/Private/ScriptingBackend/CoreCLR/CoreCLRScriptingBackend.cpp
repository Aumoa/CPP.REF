// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "ScriptingBackend/CoreCLR/CoreCLRScriptingBackend.h"
#include "ScriptingBackend/CoreCLR/coreclrhost.h"
#include "IO/Directory.h"
#include "IO/File.h"
#include "Path.h"
#include "Version.h"
#include "InvalidOperationException.h"

namespace Ayla
{
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
	}

	void CoreCLRScriptingBackend::LoadAssembly(String assemblyBasePath, String assemblyName)
	{
		if (m_Hosting)
		{
			throw InvalidOperationException(TEXT("The assembly has been loaded."));
		}

#if PLATFORM_WINDOWS
		String coreclr = TEXT("C:\\Program Files\\dotnet\\shared\\Microsoft.NETCore.App");
		std::vector<Version> versions;
		for (auto& versionDirectory : Directory::GetDirectories(coreclr))
		{
			auto versionStr = Path::GetFileName(versionDirectory);
			Version version;
			if (Version::TryParse(versionStr, version) && version.Major == 9)
			{
				versions.emplace_back(version);
			}
		}
		if (versions.size() == 0)
		{
			throw InvalidOperationException(TEXT("No suitable CoreCLR version found."));
		}
		std::ranges::sort(versions, std::greater<>());
		coreclr = Path::Combine(coreclr, versions.front().ToString(), TEXT("coreclr"));
#else
#error TODO: Add other platform support.
#endif

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
		for (auto& file : Directory::GetFiles(assemblyBasePath, SearchOption::AllDirectories))
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
			if (filePath.StartsWith(TEXT("System.")) && filePath.EndsWith(TEXT(".dll")) && filePath.Contains(TEXT("Native")) == false)
			{
				tpaList.emplace_back(file);
			}
		}

		std::string tpaList_a = String::Join(TEXT(";"), tpaList).string();
		std::string assemblyBasePath_a = assemblyBasePath.string();
		const char* propertyKeys[] =
		{
			"TRUSTED_PLATFORM_ASSEMBLIES",
			"APP_PATHS"
		};

		const char* propertyValues[] =
		{
			tpaList_a.c_str(),
			assemblyBasePath_a.c_str()
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
			throw InvalidOperationException(TEXT("coreclr_create_delegate failed."));
		}

		return functionPtr;
	}
}
