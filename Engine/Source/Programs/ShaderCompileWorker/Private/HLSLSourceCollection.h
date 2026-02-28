// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DXCCommon.h"

namespace Ayla
{
	class HLSLSourceCollection
	{
		static std::mutex m_Mutex;
		static std::map<String, ComPtr<IDxcBlobEncoding>> m_HeaderBlobs;

	public:
		static ComPtr<IDxcBlobEncoding> LoadOrGetSource(String fileName, IDxcUtils* utils);
	};

}