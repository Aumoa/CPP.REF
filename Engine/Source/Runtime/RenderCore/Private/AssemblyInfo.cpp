// Copyright 2020-2022 Aumoa.lib. All right reserved.

DEFINE_ASSEMBLY_INFO;

#include "RenderContext.h"
#include "RenderEngine.h"
#include "RenderThread.h"

GENERATE_BODY(libty::RenderCore::SRenderContext);
GENERATE_BODY(libty::RenderCore::SRenderEngine);
GENERATE_BODY(libty::RenderCore::SRenderThread);

#include "ShaderWorkspaces/CommonShaderWorkspace.h"
#include "ShaderWorkspaces/ColorShaderWorkspace.h"

GENERATE_BODY(libty::RenderCore::SCommonShaderWorkspace);
GENERATE_BODY(libty::RenderCore::SColorShaderWorkspace);

#include "Renderer/SceneRenderer.h"

GENERATE_BODY(libty::RenderCore::SSceneRenderer);