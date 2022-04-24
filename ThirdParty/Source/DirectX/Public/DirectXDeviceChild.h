// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "DirectXFactoryChild.h"

namespace libty::inline DirectX
{
	class SDirectXDevice;

	class DIRECTX_API SDirectXDeviceChild : public SDirectXFactoryChild, implements(IRHIDeviceChild)
	{
		GENERATED_BODY(SDirectXDeviceChild);

	private:
		SPROPERTY(Owner)
		IRHIDevice* Owner = nullptr;

	public:
		SDirectXDeviceChild(IRHIDevice* Owner);
		SDirectXDeviceChild(SDirectXDevice* Owner);

		using Super::Dispose;

		virtual IRHIDevice* GetDevice() override;

	protected:
		virtual void Dispose(bool bDisposing) override;
	};
}