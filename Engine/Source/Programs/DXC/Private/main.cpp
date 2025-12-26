// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"
#include "Console.h"
#include "ComPtr.h"
#include "Helper.h"
#include "IO/FileReference.h"
#include "IO/DirectoryReference.h"
#include "IO/FileStream.h"
#include "IO/FileMode.h"
#include "IO/FileAccessMode.h"
#include "Threading/Tasks/Task.h"
#include <iostream>
#include <vector>
#include <sstream>

using namespace Ayla;

struct ShaderCompilationTask
{
	String SourceFile;
	String OutputFile;
	String DependencyFile;
};

std::vector<ShaderCompilationTask> ParseCompilationList(const String& listFilePath)
{
	std::vector<ShaderCompilationTask> tasks;
	
	FileReference listFile(listFilePath);
	if (!listFile.IsExists())
	{
		LogDXC::Error(TEXT("Compilation list file not found: {}"), listFilePath);
		return tasks;
	}

	String content = listFile.ReadAllText();
	// Handle both Unix (\n) and Windows (\r\n) line endings
	auto lines = content.Split(TEXT("\n"));

	for (const auto& line : lines)
	{
		// Trim whitespace including carriage returns
		String trimmedLine = line.Trim();
		if (trimmedLine.IsEmpty() || trimmedLine.StartsWith(TEXT("#")))
		{
			continue;
		}

		// Split by spaces to get tokens
		auto tokens = trimmedLine.Split(TEXT(" "));
		if (tokens.size() < 5)
		{
			LogDXC::Warning(TEXT("Invalid line format (expected: source -o output -d deps): {}"), trimmedLine);
			continue;
		}

		ShaderCompilationTask task;
		task.SourceFile = tokens[0];

		// Parse command-line arguments
		for (size_t i = 1; i < tokens.size(); ++i)
		{
			if (tokens[i] == TEXT("-o") && i + 1 < tokens.size())
			{
				task.OutputFile = tokens[i + 1];
				++i;
			}
			else if (tokens[i] == TEXT("-d") && i + 1 < tokens.size())
			{
				task.DependencyFile = tokens[i + 1];
				++i;
			}
		}

		// Validate that all required fields are present
		if (task.SourceFile.IsEmpty() || task.OutputFile.IsEmpty() || task.DependencyFile.IsEmpty())
		{
			LogDXC::Warning(TEXT("Incomplete compilation task: {}"), trimmedLine);
			continue;
		}

		tasks.emplace_back(std::move(task));
	}

	LogDXC::Log(TEXT("Parsed {} valid compilation task(s) from list"), tasks.size());
	return tasks;
}

bool CompileShader(IDxcCompiler3* compiler, IDxcUtils* utils, const ShaderCompilationTask& task)
{
	try
	{
		LogDXC::Log(TEXT("Compiling shader: {}"), task.SourceFile);

		// Check if source file exists
		FileReference sourceFile(task.SourceFile);
		if (!sourceFile.IsExists())
		{
			LogDXC::Error(TEXT("Source file not found: {}"), task.SourceFile);
			return false;
		}

		// Load source file
		ComPtr<IDxcBlobEncoding> sourceBlob;
		HR(utils->LoadFile(task.SourceFile.c_str(), nullptr, &sourceBlob));

		// Prepare arguments for compilation
		std::vector<LPCWSTR> arguments;
		
		// Entry point (will be detected from source or use main)
		arguments.push_back(L"-E");
		arguments.push_back(L"main");
		
		// Target profile - use a versatile default (pixel shader 6.0)
		// In a real implementation, this should be specified per-shader or detected
		arguments.push_back(L"-T");
		arguments.push_back(L"ps_6_0");
		
		// Optimization
		arguments.push_back(L"-O3");
		
		// Enable debug info for better error messages
		arguments.push_back(L"-Zi");

		DxcBuffer sourceBuffer = {};
		sourceBuffer.Ptr = sourceBlob->GetBufferPointer();
		sourceBuffer.Size = sourceBlob->GetBufferSize();
		sourceBuffer.Encoding = DXC_CP_ACP;

		// Compile
		ComPtr<IDxcResult> compileResult;
		HR(compiler->Compile(
			&sourceBuffer,
			arguments.data(),
			(UINT32)arguments.size(),
			nullptr,
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
				LogDXC::Error(TEXT("Compilation failed for {}: {}"), task.SourceFile, errorMsg);
			}
			else
			{
				LogDXC::Warning(TEXT("Compilation warnings for {}: {}"), task.SourceFile, errorMsg);
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
			LogDXC::Error(TEXT("Compilation produced no output for: {}"), task.SourceFile);
			return false;
		}

		// Ensure output directory exists
		FileReference outputFile(task.OutputFile);
		auto outputDir = outputFile.GetDirectory();
		if (!outputDir.IsExists())
		{
			Directory::CreateDirectory(outputDir.GetValue());
		}

		// Ensure dependency directory exists
		FileReference depFile(task.DependencyFile);
		auto depDir = depFile.GetDirectory();
		if (!depDir.IsExists())
		{
			Directory::CreateDirectory(depDir.GetValue());
		}

		// Write output file using FileStream
		{
			FileStream outputStream(task.OutputFile, FileMode::Create, FileAccessMode::Write);
			std::span<const uint8> data((const uint8*)shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize());
			outputStream.Write(data);
			outputStream.Flush();
			outputStream.Close();
		}
		
		// Write dependency file
		// For now, just write the source file itself as a dependency
		// In a real implementation, we'd parse includes from the shader
		{
			// Convert the dependency content to narrow string for writing
			std::wstring depWideStr = task.SourceFile.c_str();
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
					// Simple UTF-8 encoding for basic Unicode
					depContent.push_back('?');  // Simplified for now
				}
			}
			
			FileStream depStream(task.DependencyFile, FileMode::Create, FileAccessMode::Write);
			std::span<const uint8> depData((const uint8*)depContent.data(), depContent.size());
			depStream.Write(depData);
			depStream.Flush();
			depStream.Close();
		}

		LogDXC::Log(TEXT("Successfully compiled: {} -> {}"), task.SourceFile, task.OutputFile);
		return true;
	}
	catch (const std::exception& e)
	{
		LogDXC::Error(TEXT("Exception while compiling {}: {}"), task.SourceFile, String::FromLiteral(e.what()));
		return false;
	}
}

int main(int argc, char** argv)
{
	try
	{
		if (argc < 2)
		{
			Console::WriteLine(TEXT("Usage: DXC <compilation-list-file>"));
			Console::WriteLine(TEXT(""));
			Console::WriteLine(TEXT("The compilation list file should contain lines in the format:"));
			Console::WriteLine(TEXT("  Source/Shader.hlsl -o Intermediate/Shader.cso -d Intermediate/Shader.def"));
			return 1;
		}

		String listFilePath = String::FromLiteral(argv[1]);
		LogDXC::Log(TEXT("DXC Shader Compiler"));
		LogDXC::Log(TEXT("Reading compilation list: {}"), listFilePath);

		// Parse compilation tasks
		auto tasks = ParseCompilationList(listFilePath);
		if (tasks.empty())
		{
			LogDXC::Warning(TEXT("No valid compilation tasks found"));
			return 0;
		}

		LogDXC::Log(TEXT("Found {} shader(s) to compile"), tasks.size());

		// Initialize DXC
		ComPtr<IDxcCompiler3> compiler;
		HR(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler)));

		ComPtr<IDxcUtils> utils;
		HR(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils)));

		// Compile shaders in parallel using Task system
		std::vector<Task<bool>> compilationTasks;
		compilationTasks.reserve(tasks.size());

		IDxcCompiler3* compilerPtr = compiler.Get();
		IDxcUtils* utilsPtr = utils.Get();

		for (const auto& task : tasks)
		{
			compilationTasks.emplace_back(
				Task<bool>::Run([compilerPtr, utilsPtr, task]() {
					return CompileShader(compilerPtr, utilsPtr, task);
				})
			);
		}

		// Wait for all compilations to complete
		auto allResults = Task<bool>::WhenAll(compilationTasks).GetAwaiter().GetResult();

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

		LogDXC::Log(TEXT("Compilation complete: {} succeeded, {} failed"), successCount, failureCount);

		return failureCount > 0 ? 1 : 0;
	}
	catch (const std::exception& e)
	{
		LogDXC::Critical(TEXT("Fatal error: {}"), String::FromLiteral(e.what()));
		return 1;
	}
}