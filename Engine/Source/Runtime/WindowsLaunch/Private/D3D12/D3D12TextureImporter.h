// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "HAL/TextureImporter.h"
#include "D3D12/D3D12Common.h"
#include "D3D12TextureImporter.gen.h"

namespace Ayla
{
	class D3D12Graphics;

	ACLASS()
	class WINDOWSLAUNCH_API D3D12TextureImporter : public TextureImporter
	{
		GENERATED_BODY()

	private:
		PPtr<D3D12Graphics> m_Graphics;
		ComPtr<IWICFormatConverter> m_FormatConverter;

	public:
		virtual void ImportFromFile(String fileName) override;
	};
}