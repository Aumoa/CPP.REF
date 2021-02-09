// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class MeshLoader : virtual public Object
{
public:
	using Super = Object;
	using This = MeshLoader;

public:
	MeshLoader();
	~MeshLoader() override;
};

class AssimpLoader : public MeshLoader
{
public:
	using Super = MeshLoader;
	using This = AssimpLoader;

public:
	AssimpLoader();
	~AssimpLoader() override;
};