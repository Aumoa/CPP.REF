// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class StaticMesh;

class ParserBase : virtual public Object
{
public:
	using Super = Object;
	
public:
	ParserBase();
	~ParserBase() override;

	virtual bool TryParse(TRefPtr<String> filepath) = 0;

	virtual bool IsStaticMesh() const = 0;
	virtual TRefPtr<StaticMesh> GetStaticMesh() const = 0;
};