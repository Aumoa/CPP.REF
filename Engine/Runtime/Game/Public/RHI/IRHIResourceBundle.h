// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "IRHIBundle.h"

class StaticMesh;
struct RHIStaticMeshGeometryData;

interface GAME_API IRHIResourceBundle : virtual public Object, virtual public IRHIBundle
{
	virtual TRefPtr<StaticMesh> CreateStaticMesh(const RHIStaticMeshGeometryData& inGeometryData) = 0;
};