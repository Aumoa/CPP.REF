// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "DXCCompiler.h"
#include "ShaderCompilationTask.h"
#include "DXCIncludeHandler.h"
#include "HLSLSourceCollection.h"
#include "CompileErrorException.h"
#include "IO/Directory.h"
#include "IO/File.h"

namespace Ayla
{
	DXCCompiler::DXCCompiler()
	{
		// Initialize DXC
		HR(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&m_Compiler)));
		HR(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&m_Utils)));
	}

	Task<> DXCCompiler::CompileShaderAsync(const ShaderCompilationTask& sct, std::stop_token cancellationToken)
	{
		ComPtr<IDxcBlobEncoding> sourceBlob = HLSLSourceCollection::LoadOrGetSource(sct.GetSourceFile(), m_Utils.Get());
		ComPtr<DXCIncludeHandler> includeHandler = new DXCIncludeHandler(m_Utils, sct);

		std::vector<String> arguments_s = ParseCompilerArguments(sct);
		std::vector<LPCWSTR> arguments;
		arguments.reserve(arguments_s.size());
		for (const auto& s : arguments_s)
		{
			arguments.emplace_back(s.c_str());
		}

		BOOL known = FALSE;
		UINT32 codePage = 0;
		HR(sourceBlob->GetEncoding(&known, &codePage));

		DxcBuffer sourceBuffer;
		sourceBuffer.Ptr = sourceBlob->GetBufferPointer();
		sourceBuffer.Size = sourceBlob->GetBufferSize();
		sourceBuffer.Encoding = known ? codePage : DXC_CP_UTF8;

		ComPtr<IDxcResult> compileResult;
		HR(m_Compiler->Compile(
			&sourceBuffer,
			arguments.data(),
			(UINT32)arguments.size(),
			includeHandler.Get(),
			IID_PPV_ARGS(&compileResult)
		));

		// Check compilation status
		HRESULT hrStatus;
		HR(compileResult->GetStatus(&hrStatus));

		// Get errors/warnings
		ComPtr<IDxcBlobUtf8> errors;
		compileResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr);
		if (errors && errors->GetStringLength() > 0)
		{
			String errorMsg = String::FromLiteral((const char*)errors->GetBufferPointer());
			if (FAILED(hrStatus))
			{
				LogDXC::Error(TEXT("Compilation failed for {}: {}"), sct.GetSourceFile(), errorMsg);
			}
			else
			{
				LogDXC::Warning(TEXT("Compilation warnings for {}: {}"), sct.GetSourceFile(), errorMsg);
			}
		}

		if (FAILED(hrStatus))
		{
			throw CompileErrorException();
		}

		// Get compiled shader bytecode
		ComPtr<IDxcBlob> shaderBlob;
		HR(compileResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr));

		if (!shaderBlob || shaderBlob->GetBufferSize() == 0)
		{
			LogDXC::Error(TEXT("Compilation produced no output for: {}"), sct.GetSourceFile());
			throw InvalidOperationException();
		}

		// Ensure output directory exists
		String outputFileName = sct.GetOutputBasePath() + (sct.IsVulkan() ? TEXT(".spv") : TEXT(".cso"));
		auto outputDir = Path::GetDirectoryName(outputFileName);
		if (!Directory::Exists(outputDir))
		{
			Directory::CreateDirectory(outputDir);
		}

		// Ensure dependency directory exists
		String dependencyFileName = sct.GetOutputBasePath() + TEXT(".deps");
		auto depDir = Path::GetDirectoryName(dependencyFileName);
		if (!Directory::Exists(depDir))
		{
			Directory::CreateDirectory(depDir);
		}

		// Write output file using FileStream
		co_await File::WriteAllBytesAsync(outputFileName, std::span{ (const uint8*)shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize() }, cancellationToken);

		// Write dependency file
		{
			StringBuilder depContent;
			depContent.AppendFormat(TEXT("{}:\n"), outputFileName);
			// Add source file as dependency
			depContent.AppendFormat(TEXT("  {}\n"), Path::GetFullPath(sct.GetSourceFile()));
			// Add include files as dependencies
			for (const auto& includedFile : includeHandler->GetDependentFiles())
			{
				depContent.AppendFormat(TEXT("  {}\n"), Path::GetFullPath(includedFile));
			}
			co_await File::WriteAllTextAsync(dependencyFileName, depContent.ToString(), cancellationToken);
		}
	}

	std::vector<String> DXCCompiler::ParseCompilerArguments(const ShaderCompilationTask& sct)
	{
		std::vector<String> arguments;

		// Add include path for the source file's directory
		String sourceFile = sct.GetSourceFile();
		String sourceDir = Path::GetDirectoryName(sourceFile);
		if (Directory::Exists(sourceDir))
		{
			arguments.emplace_back(TEXT("-I"));
			arguments.emplace_back(Path::GetFullPath(sourceDir));
		}

		// Add additional include paths from task
		for (auto& includePath : sct.GetIncludePaths())
		{
			arguments.emplace_back(TEXT("-I"));
			arguments.emplace_back(Path::GetFullPath(includePath));
		}

		String entryPoint = sct.GetEntryPoint();
		if (!entryPoint.IsEmpty())
		{
			arguments.emplace_back(TEXT("-E'"));
			arguments.emplace_back(entryPoint);
		}

		String shaderProfile;
		bool isRaytracing = false;

		switch (sct.GetType())
		{
		case ShaderType::Vertex:
			shaderProfile = TEXT("vs_6_0");
			entryPoint = TEXT("main");
			isRaytracing = false;
			break;
		case ShaderType::Pixel:
			shaderProfile = TEXT("ps_6_0");
			entryPoint = TEXT("main");
			isRaytracing = false;
			break;
		case ShaderType::Compute:
			shaderProfile = TEXT("cs_6_0");
			entryPoint = TEXT("main");
			isRaytracing = false;
			break;
		case ShaderType::Library:
			shaderProfile = TEXT("lib_6_3");
			entryPoint = TEXT("main");
			isRaytracing = true;
			break;
		}

		// Target profile
		arguments.emplace_back(TEXT("-T"));
		arguments.emplace_back(shaderProfile);

		if (sct.IsVulkan())
		{
			// SPIR-V output for Vulkan (required for raytracing)
			arguments.emplace_back(TEXT("-spirv"));

			// Vulkan 1.2 target environment
			arguments.emplace_back(TEXT("-fspv-target-env=vulkan1.2"));

			if (isRaytracing)
			{
				// Enable raytracing extension
				arguments.emplace_back(TEXT("-fspv-extension=SPV_KHR_ray_tracing"));
			}
		}

		// Optimization
		arguments.emplace_back(TEXT("-O3"));

		// Enable debug info for better error messages
		arguments.emplace_back(TEXT("-Zi"));

		// Enable 16-bit types if needed
		arguments.emplace_back(TEXT("-enable-16bit-types"));

		return arguments;
	}
}