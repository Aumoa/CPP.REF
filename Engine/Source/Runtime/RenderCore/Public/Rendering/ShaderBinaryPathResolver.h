// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RenderFeatures.h"
#include "IO/DirectoryReference.h"
#include "IO/FileReference.h"

namespace Ayla
{
	class RENDERCORE_API ShaderBinaryPathResolver : public StaticClass
	{
	public:
		[[nodiscard]] static String GetExtension(RenderFeatures InRenderFeature);
		[[nodiscard]] static String GetFileName(String InShaderName, RenderFeatures InRenderFeature);
		[[nodiscard]] static FileReference GetFile(const DirectoryReference& InShaderDirectory, String InShaderName, RenderFeatures InRenderFeature);
	};
}
