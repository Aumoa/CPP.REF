// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ResourceManagement/AssetImporter.h"

namespace Ayla
{
	class ENGINE_API SceneImporter : public AssetImporter
	{
	public:
		SceneImporter(String filePath);
		virtual ~SceneImporter() noexcept override;
	};
}