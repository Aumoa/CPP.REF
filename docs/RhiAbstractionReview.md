# RHI Abstraction Review

## Context

This note captures the RHI abstraction concerns found while closing the Vulkan
raytracing triangle branch. The immediate runtime issue was a resize crash after
the triangle rendered successfully, but the investigation exposed broader
abstraction problems that should be addressed on a separate branch before the
rendering layer grows more responsibilities.

The renderer is designed around raytracing as a core requirement. Vulkan and
Direct3D 12 backends should implement the required raytracing path or fail
clearly when the platform cannot support it. The goal is not to add non-
raytracing fallbacks.

## Current Pain Points

### Presentable image lifetime is not modeled explicitly

`RenderTexture::Acquire(CommandBuffer*)` currently behaves like a swapchain
backbuffer acquire operation, even though `RenderTexture` sounds like a general
render target abstraction.

- `RenderCore/Public/Rendering/RenderTexture.h`
- `Direct3D12/Private/DXGISwapchainRenderTexture.cpp`
- `VulkanAPI/Private/VkSwapchainRenderTexture.cpp`

This creates a weak contract. Direct3D 12 only reads the current backbuffer
index, while Vulkan also registers semaphores, initializes layouts, and manages
swapchain image caches. Those responsibilities belong to a presentable target or
swapchain frame abstraction rather than a generic texture.

### Window swapchain extension crosses rendering layers

`GenericWindowSwapchainExtension` lives in `ApplicationCore`, but it exposes
`RenderTexture` and takes `CommandBuffer` in `Present`.

- `ApplicationCore/Public/GenericWindowSwapchainExtension.h`

This couples the window extension layer to rendering command submission. That is
acceptable for a prototype, but it becomes restrictive for multi-window support,
offscreen rendering, frame graph work, and backend-specific present handling.

### CommandBuffer mixes high-level render operations

`CommandBuffer` exposes graphics render pass commands, pipeline binding, draw
commands, and ray dispatch commands through one interface.

- `RenderCore/Public/CommandBuffer.h`
- `Direct3D12/Private/D3D12CommandBuffer.cpp`
- `VulkanAPI/Private/VkCommandBuffer.cpp`

Both backends use backend-specific casts to determine whether a pipeline or
render texture is supported. This suggests that the abstraction is missing
typed command contexts such as graphics and raytracing encoders, or typed
pipeline interfaces such as `GraphicsPipeline` and `RaytracingPipeline`.

### RenderPipeline contains sample-specific binding

`RenderPipeline::SetCameraBufferView` is not a general pipeline contract. It is
a current sample binding rule that happened to fit the triangle and camera path.

- `RenderCore/Public/Rendering/RenderPipeline.h`
- `Direct3D12/Private/D3D12GeometryRenderPipeline.cpp`
- `VulkanAPI/Private/VkGeometryRenderPipeline.cpp`
- `VulkanAPI/Private/VkRaytracingRenderPipeline.cpp`

This should eventually become a more general resource binding model, such as a
shader parameter set, descriptor set, root parameter layout, or bind group
abstraction.

### Test triangle resources live inside command buffers

The current triangle sample creates vertex and index buffers inside backend
command buffer implementations.

- `Direct3D12/Private/D3D12CommandBuffer.cpp`
- `VulkanAPI/Private/VkCommandBuffer.cpp`

This is useful for the prototype, but it is not a durable RHI responsibility.
Sample geometry should move into scene, mesh, or test-renderer code once the
RHI surface is cleaned up.

## Resize Crash Connection

The observed resize crash logged `VK_ERROR_DEVICE_LOST` from `vkQueueSubmit` in
`VulkanAPI/Private/VkCommandBuffer.cpp`.

The likely direct cause is:

1. `VkSwapchainExt::DoResize` destroys and recreates the Vulkan swapchain.
2. `VkSwapchainRenderTexture` keeps cached swapchain images, image views, and
   framebuffers from the old swapchain.
3. `Acquire` skips reallocation because the old cache is still populated.
4. Raytracing updates descriptors and barriers against stale swapchain image
   views or images.
5. The invalid command buffer reaches `vkQueueSubmit`.

The small fix is to invalidate the render texture cache when the swapchain is
recreated. The architectural fix is to make presentable image acquisition,
resize, invalidation, and presentation explicit in the RHI.

## Implementation Status

The first implementation pass introduced `PresentableRenderTarget` and
`PresentableFrame` in RenderCore. Swapchain-backed render textures now own
frame acquisition, presentation, invalidation, and current size reporting
through that interface, while generic `RenderTexture` no longer exposes
swapchain acquire semantics.

Direct3D 12 and Vulkan swapchain render textures both implement the new
presentable target contract. Vulkan resize now invalidates cached swapchain
images, image views, framebuffers, and first-render state before recreating the
swapchain, so stale swapchain resources cannot survive a resize.

Vulkan resize edge cases are now explicit. Zero-size surface extents suspend
frame acquisition instead of creating an invalid swapchain, and
`VK_ERROR_OUT_OF_DATE_KHR` or `VK_SUBOPTIMAL_KHR` from acquire/present requests
a swapchain recreation on the next frame.

## Recommended Direction

### Introduce a presentable target abstraction

Create a separate concept for swapchain-backed render targets. Possible names:

- `Swapchain`
- `PresentableRenderTarget`
- `PresentableSurface`
- `SwapchainRenderTarget`

This abstraction should own:

- acquiring the current presentable image
- tracking image availability and render completion synchronization
- invalidating backend image caches after resize
- presenting the acquired image
- exposing the current output size

Generic `RenderTexture` should not need to know about swapchain acquire or
present semantics.

### Return a frame-scoped presentable image

Prefer a frame object over loose `Acquire` and `Present` calls. For example:

```cpp
PresentableFrame frame = swapchain->AcquireFrame(commandContext);
RenderTargetView output = frame.GetOutputTarget();
// render into output
frame.Present(commandContext);
```

The exact type names can change, but the important contract is that acquired
presentable images are frame-scoped and cannot silently survive resize.

### Split command recording by capability

Consider separating command recording into clearer capability surfaces:

- graphics render pass encoding
- raytracing dispatch encoding
- resource copy and upload commands
- synchronization and barriers

This does not need to become a large framework immediately. The short-term goal
is to avoid a single `CommandBuffer` API that requires every backend to accept
every operation and throw at runtime for unsupported combinations.

### Split pipeline types

`RenderPipeline` should not represent both raster graphics and raytracing
pipelines without type information. Prefer separate abstractions such as:

- `GraphicsPipeline`
- `RaytracingPipeline`
- shared lower-level `PipelineState` only if useful

This would remove several backend `dynamic_cast` checks and make render passes
express their actual requirements.

### Replace sample-specific resource binding

Move away from `SetCameraBufferView` as the common pipeline API. A better long-
term model is to describe shader-visible resources through a binding object:

- descriptor set
- bind group
- shader parameter set
- root parameter set

This will matter more once camera data, scene data, acceleration structures,
materials, textures, and per-pass outputs are no longer hardcoded.

## Suggested Follow-Up Order

1. Done: add a narrow presentable target abstraction and move swapchain
   acquire, present, resize invalidation, and current size handling into it.
2. Done: fix the Vulkan resize bug through that abstraction, including cache
   invalidation after swapchain recreation.
3. Done: handle Vulkan resize edge cases explicitly, including zero-size windows,
   `VK_ERROR_OUT_OF_DATE_KHR`, and `VK_SUBOPTIMAL_KHR`.
4. Next: revisit wait semaphore stage masks for raytracing output. The current
   Vulkan command submission uses color attachment output as the wait stage,
   which does not naturally describe raytracing writes.
5. Split graphics and raytracing pipeline abstractions before adding more
   render features.
6. Move sample triangle geometry out of command buffer implementations.

## Design Guardrails

- Do not make RHI a lowest-common-denominator wrapper that hides important
  Direct3D 12 or Vulkan concepts.
- Prefer abstractions that describe engine-level intent: presentable output,
  graphics pass, raytracing pass, resource binding, and synchronization.
- Keep backend-specific details private when possible, but expose enough
  structure that backends do not need to fake unsupported concepts.
- Treat raytracing as required engine functionality, not an optional fallback.
- Keep fixes small enough to review, but avoid adding more behavior to the
  current weak abstractions when a narrow new concept would be clearer.
