// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

inline void DirectXCommandQueue::ExecuteCommandLists(DirectXDeviceContext* deviceContext)
{
	ExecuteCommandLists(&deviceContext, 1);
}

inline void DirectXCommandQueue::ExecuteCommandLists(std::span<DirectXDeviceContext* const> deviceContexts)
{
	ExecuteCommandLists(deviceContexts.data(), deviceContexts.size());
}