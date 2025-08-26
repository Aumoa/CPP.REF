// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	class AssetImporter;

	class ENGINE_API ImportingBackend
	{
	protected:
		ImportingBackend();

	public:
		virtual ~ImportingBackend() noexcept;

		virtual std::unique_ptr<AssetImporter> CreateImporter(String filePath) = 0;
	};
}