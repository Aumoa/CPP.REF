// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

enum class EMaterialDirtyMask : uint8
{
	None = 0,
	RenderState = 1,
	UpdateBuffer = RenderState << 1,
	All = 0xFF
};

class GAME_API MaterialInterface abstract : virtual public Object
{
public:
	using Super = Object;
	using This = MaterialInterface;

private:
	uint16 index;
	EMaterialDirtyMask dirtyMask;
	float ambient;
	float diffuse;
	float specular;
	float specExp;

public:
	MaterialInterface(uint16 inIndex);
	~MaterialInterface() override;

	virtual void SetMarkDirty(EMaterialDirtyMask inAddMask);
	virtual void ResolveDirtyState();
	bool HasAnyDirtyMark() const;
	bool HasDirtyMark(EMaterialDirtyMask inCompareMask) const;

	vs_property(float, Ambient);
	float Ambient_get() const;
	void Ambient_set(float value);

	vs_property(float, Diffuse);
	float Diffuse_get() const;
	void Diffuse_set(float value);

	vs_property(float, Specular);
	float Specular_get() const;
	void Specular_set(float value);

	vs_property(float, SpecExp);
	float SpecExp_get() const;
	void SpecExp_set(float value);

	vs_property_get_auto(uint16, Index, index);
};

#include "MaterialInterface.inl"