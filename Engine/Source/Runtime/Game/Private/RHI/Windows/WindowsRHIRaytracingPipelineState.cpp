// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Windows/WindowsRHIRaytracingPipelineState.h"

#if PLATFORM_WINDOWS

#include "RHI/Windows/WindowsRHIDevice.h"
#include "RHI/Windows/WindowsRHIRootSignature.h"
#include "RTShaderLibrary.generated.h"

std::shared_ptr<RHIRaytracingPipelineState> WindowsRHIDevice::CreateRaytracingPipelineState(const RHIShaderLibraryExport& shaderExport)
{
	return MakeShared<WindowsRHIRaytracingPipelineState>(SharedFromThis(), shaderExport);
}

WindowsRHIRaytracingPipelineState::WindowsRHIRaytracingPipelineState(std::shared_ptr<WindowsRHIDevice> device, const RHIShaderLibraryExport& shaderExport)
	: RHIRaytracingPipelineState(device)
{
	auto* d3ddev = WinGetr(device);

	std::vector<D3D12_EXPORT_DESC> exports;
	exports.reserve(shaderExport.Exposes.size());

	struct RootSignatureMapInfo
	{
		std::vector<String> Exposes;
		std::vector<LPCWSTR> lpszExports;
		size_t Index;
	};
	std::map<RHIRootSignature*, RootSignatureMapInfo> localRSMaps;

	for (size_t i = 0; i < shaderExport.Exposes.size(); ++i)
	{
		auto& expose = shaderExport.Exposes[i];

		exports.emplace_back() =
		{
			.Name = expose.Name.c_str(),
			.ExportToRename = expose.Rename.has_value() ? expose.Rename->c_str() : nullptr,
			.Flags = D3D12_EXPORT_FLAG_NONE
		};

		if (expose.pLocalRS)
		{
			auto& localMap = localRSMaps[expose.pLocalRS];
			String exposeName = expose.Rename.has_value() ? *expose.Rename : expose.Name;
			localMap.Exposes.emplace_back(exposeName);
			localMap.lpszExports.emplace_back(exposeName.c_str());
		}
	}

	D3D12_DXIL_LIBRARY_DESC libraryView =
	{
		.DXILLibrary =
		{
			.pShaderBytecode = HLSL_RTShaderLibrary,
			.BytecodeLength = SE_ARRAYSIZE(HLSL_RTShaderLibrary)
		},
		.NumExports = (UINT)exports.size(),
		.pExports = exports.data()
	};

	D3D12_RAYTRACING_SHADER_CONFIG shaderConfig =
	{
		.MaxPayloadSizeInBytes = sizeof(Vector3),
		.MaxAttributeSizeInBytes = sizeof(Vector3)
	};

	D3D12_RAYTRACING_PIPELINE_CONFIG pipelineConfig =
	{
		.MaxTraceRecursionDepth = 1
	};

	std::vector<D3D12_STATE_SUBOBJECT> states =
	{
		{ D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY, &libraryView },
		{ D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG, &shaderConfig },
		{ D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG, &pipelineConfig }
	};

	D3D12_GLOBAL_ROOT_SIGNATURE globalRS;
	if (shaderExport.pGlobalRS)
	{
		globalRS =
		{
			.pGlobalRootSignature = static_cast<WindowsRHIRootSignature*>(shaderExport.pGlobalRS)->GetSignature().Get()
		};

		states.emplace_back() =
		{
			D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE,
			&globalRS
		};
	}

	size_t baseIndex = states.size();
	states.reserve(states.size() + localRSMaps.size() * 2);

	std::vector<D3D12_LOCAL_ROOT_SIGNATURE> d3dLocalRS;
	d3dLocalRS.reserve(localRSMaps.size());
	for (auto& [pRS, map] : localRSMaps)
	{
		auto& lv = d3dLocalRS.emplace_back() =
		{
			.pLocalRootSignature = static_cast<WindowsRHIRootSignature*>(pRS)->GetSignature().Get()
		};

		map.Index = states.size();

		states.emplace_back() =
		{
			D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE,
			&lv
		};
	}

	std::vector<D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION> exportAssos;
	exportAssos.reserve(localRSMaps.size());
	for (auto& [pRS, map] : localRSMaps)
	{
		size_t index = map.Index;
		auto& lv = exportAssos.emplace_back() =
		{
			.pSubobjectToAssociate = &states[index],
			.NumExports = (UINT)map.Exposes.size(),
			.pExports = map.lpszExports.data()
		};

		states.emplace_back() =
		{
			D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION,
			&lv
		};
	}

	// Do NOT append states more.

	D3D12_STATE_OBJECT_DESC pipelineSubobjects =
	{
		D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE,
		(UINT)states.size(),
		states.data()
	};

	HR(d3ddev->CreateStateObject(&pipelineSubobjects, IID_PPV_ARGS(&_pipeline)));
	HR(_pipeline->QueryInterface(IID_PPV_ARGS(&_properties)));
}

WindowsRHIRaytracingPipelineState::~WindowsRHIRaytracingPipelineState() noexcept
{
}

#endif