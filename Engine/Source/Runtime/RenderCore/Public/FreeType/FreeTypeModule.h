// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class FontFace;

class RENDERCORE_API FreeTypeModule : virtual public Object
{
	GENERATED_BODY(FreeTypeModule)

public:
	struct Impl;

private:
	std::unique_ptr<Impl> _impl;

public:
	FreeTypeModule();
	virtual ~FreeTypeModule() override;

	FontFace* CreateFontFace(std::filesystem::path fontFace, int32 faceIndex = 0);
};