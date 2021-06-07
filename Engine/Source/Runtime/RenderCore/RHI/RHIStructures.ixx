// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.RenderCore:RHIStructures;

import SC.Runtime.Core;

/// <summary>
/// Represent viewport area.
/// </summary>
export struct RHIViewport
{
	float TopLeftX = 0;
	float TopLeftY = 0;
	float Width = 0;
	float Height = 0;
	float MinDepth = 0;
	float MaxDepth = 0;
};

/// <summary>
/// Represent viewport scissor rect.
/// </summary>
export struct RHIScissorRect
{
	int32 Left = 0;
	int32 Top = 0;
	int32 Right = 0;
	int32 Bottom = 0;
};