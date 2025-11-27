// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Graphics.h"
#include "GenericApplication.h"
#include "CommandBuffer.h"

namespace Ayla
{
    Graphics::Graphics()
    {
    }

    Graphics::~Graphics() noexcept
    {
    }

    SharedPtr<CommandBuffer> Graphics::GetPooledCommandBuffer()
    {
        std::unique_lock lock(m_PooledCommandBufferMutex);
        if (m_PooledCommandBuffers.empty())
        {
            lock.unlock();
            return CreateCommandBuffer();
        }
        else
        {
			auto it = m_PooledCommandBuffers.begin() + (m_PooledCommandBuffers.size() - 1);
            auto cmd = std::move(*it);
            m_PooledCommandBuffers.erase(it);
            return cmd;
        }
    }
}