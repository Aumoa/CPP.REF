// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.RenderCore:RHICommandType;

/// <summary>
/// Specifies the type of a command list that written by device context.
/// </summary>
export enum class ERHICommandType
{
	/// <summary>
	/// Specifies a command buffer that the GPU can execute. A direct device context doesn't inherit any GPU state.
	/// </summary>
	Direct = 0,

	/// <summary>
	/// Specifies a command buffer that can be executed only directly via a direct device context. A bundle device context inherits all GPU state (except for the currently set pipeline state object and primitive topology).
	/// </summary>
	Bundle = 1,

	/// <summary>
	/// Specifies a command buffer for computing.
	/// </summary>
	Compute = 2,

	/// <summary>
	/// Specifies a command buffer for copying.
	/// </summary>
	Copy = 3,
};