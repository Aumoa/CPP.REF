// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "TextureImporter.gen.h"

namespace Ayla
{
	ACLASS()
	class RENDERCORE_API TextureImporter : public Object
	{
		GENERATED_BODY()

	public:
		virtual void ImportFromFile(String fileName) = 0;
	};
}