#pragma once

#pragma warning( disable: 4635 )

#include <Windows.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <d3d11.h>
#include <d3d11on12.h>
#include <d2d1_1.h>
#include <dwrite_1.h>
#include <wrl/client.h>
#include <wincodec.h>
#include <DirectXMath.h>

#include <algorithm>
#include <condition_variable>
#include <fstream>
#include <variant>
#include <set>
#include <stack>

#undef min
#undef max

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#undef LoadImage

using Microsoft::WRL::ComPtr;
using namespace DirectX;

#define _VISUAL_STUDIO_EDITOR_
#define __SC_GAME_EXPORT_SYMBOL__

namespace SC::Game::Details
{
	class IntegratedFactory;
	class CDevice;
	class CDeviceContext;
	class CCommandQueue;
	class CSwapChain;

	class ViewStorage;
	class VisibleViewStorage;
	class CShaderResourceView;
	class CUnorderedAccessView;
	class CBuffer;
	class HeapAllocator;
	class CDynamicBuffer;

	class GeometryBuffer;
	class HDRBuffer;
	class HDRComputedBuffer;
	class GlyphBuffer;
	class GlyphRenderer;
}

#include "../Include/SC.Game.h"

inline SC::String FormatMessage( HRESULT hr )
{
	using namespace std;

	switch ( hr )
	{
	case S_OK:
		return "S_OK(0x00000000): ";
	case E_FAIL:
		return "E_FAIL(0x80004005): Unspecified error.";
	case E_INVALIDARG:
		return "E_INVALIDARG(0x80070057): One or more arguments are invalid.";
	case WINCODEC_ERR_WRONGSTATE:
		return "WINCODEC_ERR_WRONGSTATE(0x88982F04): The codec is in the wrong state.";
	case __HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND ):
		return "HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND): The system cannot find the file specified.";
	case E_ACCESSDENIED:
		return "E_ACCESSDENIED(0x80070005): General access denied error.";
	case DXGI_ERROR_DEVICE_REMOVED:
		return "DXGI_ERROR_DEVICE_REMOVED(0x887A0005): The GPU device instance has been suspended. Use GetDeviceRemovedReason to determine the appropriate action.";
	case DXGI_ERROR_DEVICE_HUNG:
		return "DXGI_ERROR_DEVICE_HUNG(0x887A0006): The GPU will not respond to more commands, most likely because of an invalid command passed by the calling application.";
	case DXGI_ERROR_DEVICE_RESET:
		return "DXGI_ERROR_DEVICE_RESET(0x887A0007): The GPU will not respond to more commands, most likely because some other application submitted invalid commands. The calling application should re-create the device and continue.";
	default:
		return "Unknown HRESULT Error: "s + to_string( hr );
	}
}

#include "Details.h"

inline void HR( HRESULT hr )
{
	using namespace SC;

	if ( FAILED( hr ) )
	{
		if ( hr == DXGI_ERROR_DEVICE_REMOVED )
		{
			HRESULT hr = SC::Game::Details::GlobalVar.device->pDevice->GetDeviceRemovedReason();
			throw new Exception( FormatMessage( hr ), new Exception( FormatMessage( hr ) ) );
		}
		else
		{
			throw new Exception( FormatMessage( hr ) );
		}
	}
}

#include "GameObjectEnumerator.h"
#include "UI.Panel.Enumerator.h"