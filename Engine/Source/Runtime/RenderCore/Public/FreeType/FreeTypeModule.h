// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SFontFace;
class SFontCachingManager;
class SRHIDevice;

class RENDERCORE_API SFreeTypeModule : virtual public SObject
{
	GENERATED_BODY(SFreeTypeModule)

public:
	struct Impl;

private:
	std::unique_ptr<Impl> _impl;
	std::map<std::string, SFontFace*> _faces;
	SFontCachingManager* _cachingMgr = nullptr;

public:
	SFreeTypeModule();
	virtual ~SFreeTypeModule() override;

	SFontFace* CreateFontFace(std::filesystem::path fontFace, int32 faceIndex = 0);
	SFontCachingManager* CreateCachingMgr(SRHIDevice* device);
};