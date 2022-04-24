// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "DirectXCommon.h"

namespace libty::inline DirectX
{
	class SDirectXFactory;

	class SDirectXFactoryChild : implements(SObject, IRHIFactoryChild)
	{
		GENERATED_BODY(SDirectXFactoryChild)

	private:
		SPROPERTY(Owner)
		IRHIFactory* Owner = nullptr;

	public:
		SDirectXFactoryChild(IRHIFactory* Owner);
		SDirectXFactoryChild(SDirectXFactory* Owner);

		virtual void Dispose() override;

		virtual IRHIFactory* GetFactory() override;

	protected:
		virtual void Dispose(bool bDisposing);
	};
}