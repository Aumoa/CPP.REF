// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "IRHIBundle.h"

enum class ERHITextureFormat;
interface IRHIResource;
interface IRHIScene;
class StaticMesh;
class PlatformImage;
class APlayerController;
struct RHIStaticMeshGeometryData;

interface GAME_API IRHIResourceBundle : virtual public Object, virtual public IRHIBundle
{
	virtual TRefPtr<StaticMesh> CreateStaticMesh(const RHIStaticMeshGeometryData& inGeometryData) = 0;
	virtual TRefPtr<IRHIResource> CreateTexture2D(ERHITextureFormat format, PlatformImage* platformImage) = 0;
};