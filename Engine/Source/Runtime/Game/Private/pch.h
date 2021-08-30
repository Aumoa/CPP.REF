// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"

#include <optional>
#include <chrono>
#include <string>
#include <string_view>
#include <memory>
#include <set>
#include <vector>
#include <queue>
#include <variant>
#include <map>
#include <atomic>
#include <functional>
#include <stack>
#include <queue>

#include "IWindowView.h"
#include "RHI/RHIDevice.h"
#include "RHI/RHISwapChain.h"
#include "RHI/RHIDeviceContext.h"
#include "RHI/RHICommandQueue.h"
#include "RHI/RHIRenderTargetView.h"
#include "RHI/RHITexture2D.h"
#include "RHI/RHIDepthStencilView.h"
#include "RHI/RHIShader.h"
#include "RHI/RHIVertexFactory.h"
#include "RHI/RHIResource.h"

#undef min
#undef max