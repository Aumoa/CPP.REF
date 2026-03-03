// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"
#include "Console.h"
#include "DXCCommon.h"
#include "ShaderCompilationTask.h"
#include "CompileErrorException.h"
#include "DXCCompiler.h"
#include <vector>
#include <csignal>

using namespace Ayla;

Task<int> MainAsync(int argc, char** argv, std::stop_token cancellationToken)
{
	try
	{
		if (argc < 2)
		{
			Console::WriteLine(TEXT("Usage: ShaderCompileWorker <compilation-list-file>"));
			Console::WriteLine(TEXT(""));
			Console::WriteLine(TEXT("The compilation list file should contain lines in the format:"));
			Console::WriteLine(TEXT("  Source/Shader.hlsl -t library -o Intermediate/Shader -I Include/Path -e main"));
			co_return 1;
		}

		String listFilePath = String::FromLiteral(argv[1]);
		LogDXC::Info(TEXT("Shader Compile Worker"));
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
		auto compiler = DXCCompiler();

		std::vector<Task<>> compilationTasks;
		compilationTasks.reserve(tasks.size());
		for (const auto& task : tasks)
		{
			compilationTasks.emplace_back(compiler.CompileShaderAsync(task, cancellationToken));
		}

		co_await Task<>::WhenAll(std::move(compilationTasks));
		LogDXC::Info(TEXT("Compilation complete: {} succeeded"), tasks.size());
		co_return 0;
	}
	catch (const std::exception& e)
	{
		std::vector<String> shaderCompileErrors;
		if (auto ae = dynamic_cast<const AggregateException*>(&e); ae)
		{
			size_t i = 0;
			for (auto& ie : ae->GetInnerExceptions())
			{
				try
				{
					std::rethrow_exception(ie);
				}
				catch (const CompileErrorException& cee)
				{
					shaderCompileErrors.emplace_back(cee.GetMessage());
				}
				catch (const Exception& e)
				{
					Console::WriteLine(TEXT("Inner Exception #{}: {}"), i, e);
				}
				catch (const std::exception& e)
				{
					Console::WriteLine(TEXT("Inner Exception #{}: {}"), i, String::FromLiteral(e.what()));
				}

				++i;
			}
		}
		else if (auto cee = dynamic_cast<const CompileErrorException*>(&e); cee)
		{
			shaderCompileErrors.emplace_back(cee->GetMessage());
		}

		LogDXC::Critical(TEXT("Fatal error: {}"), String::FromLiteral(e.what()));
		Console::Error.WriteLine(String::Join(TEXT("> "), shaderCompileErrors));
		co_return 1;
	}
}

std::stop_source g_ss;

void sigint(int)
{
	g_ss.request_stop();
}

int main(int argc, char** argv)
{
	signal(SIGINT, sigint);

	return MainAsync(argc, argv, g_ss.get_token()).GetResult();
}