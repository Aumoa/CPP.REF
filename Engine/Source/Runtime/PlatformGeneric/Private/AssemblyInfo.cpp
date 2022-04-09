// Copyright 2020-2022 Aumoa.lib. All right reserved.

DEFINE_ASSEMBLY_INFO;

#include "RHI/RHIEnums.h"

GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIPrimitiveTopology);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHICommandListType);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIResourceStates);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIPixelFormat);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIVertexElementFormat);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIInputClassification);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIShaderParameterType);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIResourceFlags);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIDescriptorRangeType);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIBufferUsage);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIRTVDimension);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIDSVDimension);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHISRVDimension);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIDSVFlags);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIBufferSRVFlags);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIFontWeight);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIFontStyle);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIFontStretch);
GENERATE_BODY(libty::PlatformGeneric::RHI::EMaterialBlendMode);
GENERATE_BODY(libty::PlatformGeneric::RHI::EMaterialDomain);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIDrawTextOptions);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHITextAlignment);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIParagraphAlignment);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIInterpolationMode);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHICompositeMode);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIAttachmentLoadOp);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIAttachmentStoreOp);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIShaderType);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIRootParameterType);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIShaderVisibility);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHISamplerFilter);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHITextureAddressMode);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIComparisonFunc);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIStaticBorderColor);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIRootSignatureFlags);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIBlend);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIBlendOp);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHILogicOp);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIFillMode);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHICullMode);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIConservativeRasterizationMode);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIDepthWriteMask);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIStencilOp);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIIndexBufferStripCutValue);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIPrimitiveTopologyType);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIPipelineStateFlags);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIColorWriteEnable);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIResourceBarrierType);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIResourceBarrierFlags);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIUAVDimension);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIBufferUAVFlags);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIHeapType);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHICPUPageProperty);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIMemoryPool);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIHeapFlags);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHIResourceDimension);
GENERATE_BODY(libty::PlatformGeneric::RHI::ERHITextureLayout);

#include "RHI/RHIInterfaces.h"

GENERATE_BODY(libty::PlatformGeneric::RHI::IRHIFactoryChild);
GENERATE_BODY(libty::PlatformGeneric::RHI::IRHIAdapter);
GENERATE_BODY(libty::PlatformGeneric::RHI::IRHIDeviceChild);
GENERATE_BODY(libty::PlatformGeneric::RHI::IRHIRootSignature);
GENERATE_BODY(libty::PlatformGeneric::RHI::IRHIPipelineState);
GENERATE_BODY(libty::PlatformGeneric::RHI::IRHIResource);
GENERATE_BODY(libty::PlatformGeneric::RHI::IRHIView);
GENERATE_BODY(libty::PlatformGeneric::RHI::IRHIRenderTargetView);
GENERATE_BODY(libty::PlatformGeneric::RHI::IRHIDepthStencilView);
GENERATE_BODY(libty::PlatformGeneric::RHI::IRHIShaderResourceView);
GENERATE_BODY(libty::PlatformGeneric::RHI::IRHICommandAllocator);
GENERATE_BODY(libty::PlatformGeneric::RHI::IRHIGraphicsCommandList);
GENERATE_BODY(libty::PlatformGeneric::RHI::IRHIShaderCodeBlob);
GENERATE_BODY(libty::PlatformGeneric::RHI::IRHIShaderCodeWorkspace);
GENERATE_BODY(libty::PlatformGeneric::RHI::IRHIFence);
GENERATE_BODY(libty::PlatformGeneric::RHI::IRHISwapChain);
GENERATE_BODY(libty::PlatformGeneric::RHI::IRHICommandQueue);
GENERATE_BODY(libty::PlatformGeneric::RHI::IRHIDevice);
GENERATE_BODY(libty::PlatformGeneric::RHI::IRHIFactory);

#include "Multimedia/IPlatformImage.h"
#include "Multimedia/IPlatformImageLoader.h"

GENERATE_BODY(libty::PlatformGeneric::Multimedia::IPlatformImage);
GENERATE_BODY(libty::PlatformGeneric::Multimedia::IPlatformImageLoader);

#include "Input/IMEEvent.h"
#include "Input/IPlatformIME.h"
#include "Input/IPlatformKeyboard.h"
#include "Input/IPlatformMouse.h"
#include "Input/KeyboardTracker.h"
#include "Input/MouseState.h"
#include "Input/MouseStateTracker.h"

GENERATE_BODY(libty::PlatformGeneric::Input::EIMEEvent);
GENERATE_BODY(libty::PlatformGeneric::Input::IPlatformIME);
GENERATE_BODY(libty::PlatformGeneric::Input::IPlatformKeyboard);
GENERATE_BODY(libty::PlatformGeneric::Input::IPlatformMouse);
GENERATE_BODY(libty::PlatformGeneric::Input::SKeyboardTracker);
GENERATE_BODY(libty::PlatformGeneric::Input::EMousePositionMode);
GENERATE_BODY(libty::PlatformGeneric::Input::SMouseStateTracker);

#include "IApplicationInterface.h"

GENERATE_BODY(libty::PlatformGeneric::IApplicationInterface);
GENERATE_BODY(libty::PlatformGeneric::IApplicationInterface::ETickMode);