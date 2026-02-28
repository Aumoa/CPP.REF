// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "HLSLSourceCollection.h"

namespace Ayla
{
	std::mutex HLSLSourceCollection::m_Mutex;
	std::map<String, ComPtr<IDxcBlobEncoding>> HLSLSourceCollection::m_HeaderBlobs;

	ComPtr<IDxcBlobEncoding> HLSLSourceCollection::LoadOrGetSource(String fileName, IDxcUtils* utils)
	{
		auto lock = std::unique_lock(m_Mutex);
		auto it = m_HeaderBlobs.find(fileName);
		if (it != m_HeaderBlobs.end())
		{
			return it->second;
		}

		lock.unlock();
		ComPtr<IDxcBlobEncoding> sourceBlob;
		HR(utils->LoadFile(fileName.c_str(), nullptr, &sourceBlob));

		lock.lock();
		m_HeaderBlobs.emplace(fileName, sourceBlob);
		return sourceBlob;
	}
}