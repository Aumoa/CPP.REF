// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/ShaderBinaryPathResolver.h"
#include "InvalidOperationException.h"

namespace Ayla
{
	String ShaderBinaryPathResolver::GetExtension(RenderFeatures InRenderFeature)
	{
		switch (InRenderFeature)
		{
		case RenderFeatures::D3D12:
			return TEXT("cso");
		case RenderFeatures::Vulkan:
			return TEXT("spv");
		}

		throw InvalidOperationException(TEXT("Unsupported render feature for shader binary lookup."));
	}

	String ShaderBinaryPathResolver::GetFileName(String InShaderName, RenderFeatures InRenderFeature)
	{
		return String::Format(TEXT("{0}.{1}"), InShaderName, GetExtension(InRenderFeature));
	}

	FileReference ShaderBinaryPathResolver::GetFile(const DirectoryReference& InShaderDirectory, String InShaderName, RenderFeatures InRenderFeature)
	{
		return InShaderDirectory.GetFile(GetFileName(InShaderName, InRenderFeature));
	}
}
