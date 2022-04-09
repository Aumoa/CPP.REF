// Copyright 2020-2022 Aumoa.lib. All right reserved.

DEFINE_ASSEMBLY_INFO;

#include "RHI/RHIEnums.h"

GENERATE_BODY(libty::PlatformGeneric::ERHIPrimitiveTopology);
GENERATE_BODY(libty::PlatformGeneric::ERHICommandListType);
GENERATE_BODY(libty::PlatformGeneric::ERHIResourceStates);
GENERATE_BODY(libty::PlatformGeneric::ERHIPixelFormat);
GENERATE_BODY(libty::PlatformGeneric::ERHIVertexElementFormat);
GENERATE_BODY(libty::PlatformGeneric::ERHIInputClassification);
GENERATE_BODY(libty::PlatformGeneric::ERHIShaderParameterType);
GENERATE_BODY(libty::PlatformGeneric::ERHIResourceFlags);
GENERATE_BODY(libty::PlatformGeneric::ERHIDescriptorRangeType);
GENERATE_BODY(libty::PlatformGeneric::ERHIBufferUsage);
GENERATE_BODY(libty::PlatformGeneric::ERHIRTVDimension);
GENERATE_BODY(libty::PlatformGeneric::ERHIDSVDimension);
GENERATE_BODY(libty::PlatformGeneric::ERHISRVDimension);
GENERATE_BODY(libty::PlatformGeneric::ERHIDSVFlags);
GENERATE_BODY(libty::PlatformGeneric::ERHIBufferSRVFlags);
GENERATE_BODY(libty::PlatformGeneric::ERHIFontWeight);
GENERATE_BODY(libty::PlatformGeneric::ERHIFontStyle);
GENERATE_BODY(libty::PlatformGeneric::ERHIFontStretch);
GENERATE_BODY(libty::PlatformGeneric::EMaterialBlendMode);
GENERATE_BODY(libty::PlatformGeneric::EMaterialDomain);
GENERATE_BODY(libty::PlatformGeneric::ERHIDrawTextOptions);
GENERATE_BODY(libty::PlatformGeneric::ERHITextAlignment);
GENERATE_BODY(libty::PlatformGeneric::ERHIParagraphAlignment);
GENERATE_BODY(libty::PlatformGeneric::ERHIInterpolationMode);
GENERATE_BODY(libty::PlatformGeneric::ERHICompositeMode);
GENERATE_BODY(libty::PlatformGeneric::ERHIAttachmentLoadOp);
GENERATE_BODY(libty::PlatformGeneric::ERHIAttachmentStoreOp);
GENERATE_BODY(libty::PlatformGeneric::ERHIShaderType);
GENERATE_BODY(libty::PlatformGeneric::ERHIRootParameterType);
GENERATE_BODY(libty::PlatformGeneric::ERHIShaderVisibility);
GENERATE_BODY(libty::PlatformGeneric::ERHISamplerFilter);
GENERATE_BODY(libty::PlatformGeneric::ERHITextureAddressMode);
GENERATE_BODY(libty::PlatformGeneric::ERHIComparisonFunc);
GENERATE_BODY(libty::PlatformGeneric::ERHIStaticBorderColor);
GENERATE_BODY(libty::PlatformGeneric::ERHIRootSignatureFlags);
GENERATE_BODY(libty::PlatformGeneric::ERHIBlend);
GENERATE_BODY(libty::PlatformGeneric::ERHIBlendOp);
GENERATE_BODY(libty::PlatformGeneric::ERHILogicOp);
GENERATE_BODY(libty::PlatformGeneric::ERHIFillMode);
GENERATE_BODY(libty::PlatformGeneric::ERHICullMode);
GENERATE_BODY(libty::PlatformGeneric::ERHIConservativeRasterizationMode);
GENERATE_BODY(libty::PlatformGeneric::ERHIDepthWriteMask);
GENERATE_BODY(libty::PlatformGeneric::ERHIStencilOp);
GENERATE_BODY(libty::PlatformGeneric::ERHIIndexBufferStripCutValue);
GENERATE_BODY(libty::PlatformGeneric::ERHIPrimitiveTopologyType);
GENERATE_BODY(libty::PlatformGeneric::ERHIPipelineStateFlags);
GENERATE_BODY(libty::PlatformGeneric::ERHIColorWriteEnable);
GENERATE_BODY(libty::PlatformGeneric::ERHIResourceBarrierType);
GENERATE_BODY(libty::PlatformGeneric::ERHIResourceBarrierFlags);
GENERATE_BODY(libty::PlatformGeneric::ERHIUAVDimension);
GENERATE_BODY(libty::PlatformGeneric::ERHIBufferUAVFlags);
GENERATE_BODY(libty::PlatformGeneric::ERHIHeapType);
GENERATE_BODY(libty::PlatformGeneric::ERHICPUPageProperty);
GENERATE_BODY(libty::PlatformGeneric::ERHIMemoryPool);
GENERATE_BODY(libty::PlatformGeneric::ERHIHeapFlags);
GENERATE_BODY(libty::PlatformGeneric::ERHIResourceDimension);
GENERATE_BODY(libty::PlatformGeneric::ERHITextureLayout);

#include "RHI/RHIInterfaces.h"

GENERATE_BODY(libty::PlatformGeneric::IRHIFactoryChild);
GENERATE_BODY(libty::PlatformGeneric::IRHIAdapter);
GENERATE_BODY(libty::PlatformGeneric::IRHIDeviceChild);
GENERATE_BODY(libty::PlatformGeneric::IRHIRootSignature);
GENERATE_BODY(libty::PlatformGeneric::IRHIPipelineState);
GENERATE_BODY(libty::PlatformGeneric::IRHIResource);
GENERATE_BODY(libty::PlatformGeneric::IRHIView);
GENERATE_BODY(libty::PlatformGeneric::IRHIRenderTargetView);
GENERATE_BODY(libty::PlatformGeneric::IRHIDepthStencilView);
GENERATE_BODY(libty::PlatformGeneric::IRHIShaderResourceView);
GENERATE_BODY(libty::PlatformGeneric::IRHICommandAllocator);
GENERATE_BODY(libty::PlatformGeneric::IRHIGraphicsCommandList);
GENERATE_BODY(libty::PlatformGeneric::IRHIShaderCodeBlob);
GENERATE_BODY(libty::PlatformGeneric::IRHIShaderCodeWorkspace);
GENERATE_BODY(libty::PlatformGeneric::IRHIFence);
GENERATE_BODY(libty::PlatformGeneric::IRHISwapChain);
GENERATE_BODY(libty::PlatformGeneric::IRHICommandQueue);
GENERATE_BODY(libty::PlatformGeneric::IRHIDevice);
GENERATE_BODY(libty::PlatformGeneric::IRHIFactory);

#include "Multimedia/IPlatformImage.h"
#include "Multimedia/IPlatformImageLoader.h"

GENERATE_BODY(libty::PlatformGeneric::IPlatformImage);
GENERATE_BODY(libty::PlatformGeneric::IPlatformImageLoader);

#include "Input/IMEEvent.h"
#include "Input/IPlatformIME.h"
#include "Input/IPlatformKeyboard.h"
#include "Input/IPlatformMouse.h"
#include "Input/KeyboardTracker.h"
#include "Input/MouseState.h"
#include "Input/MouseStateTracker.h"

GENERATE_BODY(libty::PlatformGeneric::EIMEEvent);
GENERATE_BODY(libty::PlatformGeneric::IPlatformIME);
GENERATE_BODY(libty::PlatformGeneric::IPlatformKeyboard);
GENERATE_BODY(libty::PlatformGeneric::IPlatformMouse);
GENERATE_BODY(libty::PlatformGeneric::SKeyboardTracker);
GENERATE_BODY(libty::PlatformGeneric::EMousePositionMode);
GENERATE_BODY(libty::PlatformGeneric::SMouseStateTracker);

#include "IApplicationInterface.h"

GENERATE_BODY(libty::PlatformGeneric::IApplicationInterface);
GENERATE_BODY(libty::PlatformGeneric::IApplicationInterface::ETickMode);