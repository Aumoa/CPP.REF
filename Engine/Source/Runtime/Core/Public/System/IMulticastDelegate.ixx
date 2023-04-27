// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.System:IMulticastDelegate;

export import :IntegralTypes;

export class DelegateHandle;

export class IMulticastDelegate
{
public:
	virtual ~IMulticastDelegate() noexcept = default;
	virtual void Remove(DelegateHandle&) = 0;
};