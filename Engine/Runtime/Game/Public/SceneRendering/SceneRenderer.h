// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHIResource;
interface IRHIDeferredCommandList;
interface IRHIDeviceBundle;
interface IRHIRenderTargetView;

class PrimitiveSceneProxy;

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

	std::vector<PrimitiveSceneProxy*> primitives;
	size_t primitives_size;

	int32 sceneSizeX;
	int32 sceneSizeY;

public:
	SceneRenderer(IRHIDeviceBundle* deviceBundle);
	~SceneRenderer() override;

	virtual void BeginRender();
	virtual void EndRender();
	virtual void PopulateRenderCommands();

	void AddPrimitives(const std::vector<PrimitiveSceneProxy*>& sceneProxies);
	PrimitiveSceneProxy* const* GetPrimitives() const;
	size_t GetPrimitiveCount() const;

	vs_property_get(IRHIDeferredCommandList*, CommandList);
	IRHIDeferredCommandList* CommandList_get() const;
	vs_property_get(IRHIResource*, FinalColor);
	IRHIResource* FinalColor_get() const;

protected:
	IRHIRenderTargetView* GetFinalColorRTV() const;
	void GetSceneSize(int32& x, int32& y);

private:
	// CALLBACK HANDLERS
	void Application_OnPostSized(int32 width, int32 height);
};