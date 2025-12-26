// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"
#include "Console.h"
#include "DXCCommon.h"
#include "IO/FileReference.h"
#include "IO/DirectoryReference.h"
#include "IO/FileStream.h"
#include "IO/FileMode.h"
#include "IO/FileAccessMode.h"
#include "Threading/Tasks/Task.h"
#include "ShaderCompilationTask.h"
#include <vector>
#include <sstream>

using namespace Ayla;

bool CompileShader(IDxcCompiler3* compiler, IDxcUtils* utils, const ShaderCompilationTask& task)
{
	try
	{
		LogDXC::Info(TEXT("Compiling shader: {}"), task.GetSourceFile());

		// Check if source file exists
		FileReference sourceFile(task.GetSourceFile());
		if (!sourceFile.IsExists())
		{
			LogDXC::Error(TEXT("Source file not found: {}"), task.GetSourceFile());
			return false;
		}

		// Load source file
		ComPtr<IDxcBlobEncoding> sourceBlob;
		HR(utils->LoadFile(task.GetSourceFile().c_str(), nullptr, &sourceBlob));

		// Create default include handler
		// This allows #include directives to work with relative paths
		ComPtr<IDxcIncludeHandler> includeHandler;
		HR(utils->CreateDefaultIncludeHandler(&includeHandler));

		// Prepare arguments for compilation
		std::vector<LPCWSTR> arguments;
		
		// Add include path for the source file's directory
		// This allows #include "Common.hlsli" to work
		auto sourceDir = sourceFile.GetDirectory();
		if (sourceDir.IsExists())
		{
			arguments.push_back(L"-I");
			arguments.push_back(sourceDir.GetAbsolutePath().c_str());
		}
		
		// Add additional include paths from task
		for (const auto& includePath : task.GetIncludePaths())
		{
			arguments.push_back(L"-I");
			arguments.push_back(includePath.c_str());
		}
		
		// Detect shader type from filename extension or naming convention
		const wchar_t* shaderProfile = nullptr;
		const wchar_t* entryPoint = L"main";
		bool isRaytracing = false;

		switch (task.GetType())
		{
			case ShaderType::Vertex:
				shaderProfile = L"vs_6_0";
				break;
			case ShaderType::Pixel:
				shaderProfile = L"ps_6_0";
				break;
			case ShaderType::Compute:
				shaderProfile = L"cs_6_0";
				break;
			case ShaderType::Library:
				shaderProfile = L"lib_6_3";
				isRaytracing = true;
				break;
			default:
				LogDXC::Error(TEXT("Unsupported shader type for file: {}"), task.GetSourceFile());
				return false;
		}
		
		// Entry point
		arguments.push_back(L"-E");
		arguments.push_back(entryPoint);
		
		// Target profile
		arguments.push_back(L"-T");
		arguments.push_back(shaderProfile);
		
		// SPIR-V output for Vulkan (required for raytracing)
		arguments.push_back(L"-spirv");
		
		// Vulkan 1.2 target environment
		arguments.push_back(L"-fspv-target-env=vulkan1.2");
		
		// For raytracing shaders, use library linkage
		if (isRaytracing)
		{
			// Enable raytracing extension
			arguments.push_back(L"-fspv-extension=SPV_KHR_ray_tracing");
		}
		
		// Optimization
		arguments.push_back(L"-O3");
		
		// Enable debug info for better error messages
		arguments.push_back(L"-Zi");
		
		// Enable 16-bit types if needed
		arguments.push_back(L"-enable-16bit-types");

		DxcBuffer sourceBuffer = {};
		sourceBuffer.Ptr = sourceBlob->GetBufferPointer();
		sourceBuffer.Size = sourceBlob->GetBufferSize();
		sourceBuffer.Encoding = DXC_CP_UTF8;

		// Compile with include handler
		ComPtr<IDxcResult> compileResult;
		HR(compiler->Compile(
			&sourceBuffer,
			arguments.data(),
			(UINT32)arguments.size(),
			includeHandler.Get(),  // Pass include handler here
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
				LogDXC::Error(TEXT("Compilation failed for {}: {}"), task.GetSourceFile(), errorMsg);
			}
			else
			{
				LogDXC::Warning(TEXT("Compilation warnings for {}: {}"), task.GetSourceFile(), errorMsg);
			}
		}

		if (FAILED(hrStatus))
		{
			return false;
		}

		// Get compiled shader bytecode
		ComPtr<IDxcBlob> shaderBlob;
		HR(compileResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr));

		if (!shaderBlob || shaderBlob->GetBufferSize() == 0)
		{
			LogDXC::Error(TEXT("Compilation produced no output for: {}"), task.GetSourceFile());
			return false;
		}

		// Ensure output directory exists
		String outputFileName = task.GetOutputBasePath() + TEXT(".spv");
		FileReference outputFile(outputFileName);
		auto outputDir = outputFile.GetDirectory();
		if (!outputDir.IsExists())
		{
			Directory::CreateDirectory(outputDir.GetValue());
		}

		// Ensure dependency directory exists
		String dependencyFileName = task.GetOutputBasePath() + TEXT(".deps");
		FileReference depFile(dependencyFileName);
		auto depDir = depFile.GetDirectory();
		if (!depDir.IsExists())
		{
			Directory::CreateDirectory(depDir.GetValue());
		}

		// Write output file using FileStream
		{
			FileStream outputStream(outputFileName, FileMode::Create, FileAccessMode::Write);
			std::span<const uint8> data((const uint8*)shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize());
			outputStream.Write(data);
			outputStream.Flush();
			outputStream.Close();
		}
		
		// Write dependency file
		{
			std::wstring depWideStr = task.GetSourceFile().c_str();
			depWideStr += L"\n";
			
			// Convert wstring to UTF-8
			std::string depContent;
			for (wchar_t wc : depWideStr)
			{
				if (wc <= 0x7F)
				{
					depContent.push_back(static_cast<char>(wc));
				}
				else
				{
					depContent.push_back('?');
				}
			}
			
			FileStream depStream(dependencyFileName, FileMode::Create, FileAccessMode::Write);
			std::span<const uint8> depData((const uint8*)depContent.data(), depContent.size());
			depStream.Write(depData);
			depStream.Flush();
			depStream.Close();
		}

		LogDXC::Info(TEXT("Successfully compiled: {} -> {}"), task.GetSourceFile(), outputFileName);
		return true;
	}
	catch (const std::exception& e)
	{
		LogDXC::Error(TEXT("Exception while compiling {}: {}"), task.GetSourceFile(), String::FromLiteral(e.what()));
		return false;
	}
}

Task<int> MainAsync(int argc, char** argv)
{
	try
	{
		if (argc < 2)
		{
			Console::WriteLine(TEXT("Usage: DXC <compilation-list-file>"));
			Console::WriteLine(TEXT(""));
			Console::WriteLine(TEXT("The compilation list file should contain lines in the format:"));
			Console::WriteLine(TEXT("  Source/Shader.hlsl -t library -o Intermediate/Shader -I Include/Path"));
			co_return 1;
		}

		String listFilePath = String::FromLiteral(argv[1]);
		LogDXC::Info(TEXT("DXC Shader Compiler"));
		LogDXC::Info(TEXT("Reading compilation list: {}"), listFilePath);

		// Parse compilation tasks using the new ShaderCompilationTask class
		auto tasks = co_await ShaderCompilationTask::ParseMakefileAsync(listFilePath);
		if (tasks.empty())
		{
			LogDXC::Warning(TEXT("No valid compilation tasks found"));
			co_return 0;
		}

		LogDXC::Info(TEXT("Found {} shader(s) to compile"), tasks.size());

		// Initialize DXC
		ComPtr<IDxcCompiler3> compiler;
		HR(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler)));

		ComPtr<IDxcUtils> utils;
		HR(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils)));

		// Compile shaders in parallel using Task system
		// Note: IDxcCompiler3 is thread-safe according to DXC documentation
		std::vector<Task<bool>> compilationTasks;
		compilationTasks.reserve(tasks.size());

		IDxcCompiler3* compilerPtr = compiler.Get();
		IDxcUtils* utilsPtr = utils.Get();

		for (const auto& task : tasks)
		{
			compilationTasks.emplace_back(
				Task<>::Run([compilerPtr, utilsPtr, task]() {
					return CompileShader(compilerPtr, utilsPtr, task);
				})
			);
		}

		// Wait for all compilations to complete
		auto allResults = co_await Task<>::WhenAll(compilationTasks);

		// Check results
		size_t successCount = 0;
		size_t failureCount = 0;
		for (bool result : allResults)
		{
			if (result)
			{
				++successCount;
			}
			else
			{
				++failureCount;
			}
		}

		LogDXC::Info(TEXT("Compilation complete: {} succeeded, {} failed"), successCount, failureCount);

		co_return failureCount > 0 ? 1 : 0;
	}
	catch (const std::exception& e)
	{
		LogDXC::Critical(TEXT("Fatal error: {}"), String::FromLiteral(e.what()));
		co_return 1;
	}
}

int main(int argc, char** argv)
{
	return MainAsync(argc, argv).GetResult();
}