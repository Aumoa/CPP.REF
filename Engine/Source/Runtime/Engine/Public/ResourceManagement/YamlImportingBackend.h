// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if WITH_EDITOR

#pragma once

#include "CoreMinimal.h"
#include "ResourceManagement/ImportingBackend.h"

namespace Ayla
{
	class ENGINE_API YamlImportingBackend : public ImportingBackend
	{
		using Super = ImportingBackend;

	public:
		YamlImportingBackend();
		virtual ~YamlImportingBackend() noexcept override;
	};
}

#endif