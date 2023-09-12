// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class APPLICATIONCORE_API NGenericImage : public std::enable_shared_from_this<NGenericImage>
{
private:
	struct Impl;
	std::unique_ptr<Impl> PlatformImpl;

private:
	NGenericImage();

public:
	~NGenericImage() noexcept;

	static Task<std::shared_ptr<NGenericImage>> LoadFromFileAsync(String InFilename);
};