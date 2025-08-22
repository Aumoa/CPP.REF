// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	class ENGINE_API AssetImporter
	{
	private:
		String m_FilePath;

	protected:
		AssetImporter(String filePath);

	public:
		virtual ~AssetImporter() noexcept;
	};
}