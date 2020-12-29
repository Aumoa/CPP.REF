// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHIResource;
interface IRHIDeferredCommandList;
interface IRHIDeviceBundle;
interface IRHIRenderTargetView;

class GAME_API SceneRenderer : virtual public Object
{
public:
	using Super = Object;
	using This = SceneRenderer;

private:
	IRHIDeviceBundle* deviceBundle;
	TRefPtr<IRHIResource> finalColor;
	TRefPtr<IRHIDeferredCommandList> commandList;
	TRefPtr<IRHIRenderTargetView> rtv;

public:
	SceneRenderer(IRHIDeviceBundle* deviceBundle);
	~SceneRenderer() override;

	virtual void BeginRender();
	virtual void EndRender();

	vs_property_get(IRHIDeferredCommandList*, CommandList);
	IRHIDeferredCommandList* CommandList_get() const;
	vs_property_get(IRHIResource*, FinalColor);
	IRHIResource* FinalColor_get() const;

private:
	// CALLBACK HANDLERS
	void Application_OnPostSized(int32 width, int32 height);
};