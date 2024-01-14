// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "SCWApp.h"
#include "Platform/ShaderCompiler.h"
#include "Exceptions/UsageException.h"
#include "Exceptions/TerminateException.h"
#include "Threading/CancellationToken.h"

NSCWApp::NSCWApp()
{
}

Task<int32> NSCWApp::RunAsync(std::stop_token InCancellationToken)
{
	String Input, Output;
	if (CommandLine::TryGetValue(TEXT("Input"), Input) == false)
	{
		throw UsageException();
	}
	if (CommandLine::TryGetValue(TEXT("Output"), Output) == false)
	{
		throw UsageException();
	}

	FileReference InputFile = Input;
	DirectoryReference OutputDirectory = Output;

	if (InputFile.IsExists() == false)
	{
		throw TerminateException(TerminateException::EKnownErrorCodes::InputFileNotFound);
	}

	if (OutputDirectory.IsExists() == false)
	{
		OutputDirectory.Create();
	}

	auto Compiler = NShaderCompiler::GeneratePlatformCompiler();

	static constexpr String HeaderFormatBase = TEXT(R"(
// This header file is generated by ShaderCompileWorker.
// Do NOT modify this file manually.

constexpr byte {}[] =
{{
	{}
}};
)");

	static constexpr size_t BinaryEachLine = 16;

	String Name = InputFile.GetName();
	FileReference OutputFile = OutputDirectory.GetFile(Name).WithExtensions(TEXT("fx.h"));
	std::vector<byte> Binary;

	auto Timer = PerformanceTimer::StartNew();
	if (Name.EndsWith(TEXT("VertexShader")))
	{
		Binary = co_await Compiler->CompileVertexShaderAsync(InputFile, InCancellationToken);
	}
	else if (Name.EndsWith(TEXT("PixelShader")))
	{
		Binary = co_await Compiler->CompilePixelShaderAsync(InputFile, InCancellationToken);
	}
	else
	{
		throw TerminateException(TerminateException::EKnownErrorCodes::NotSupportedShaderType);
	}

	std::vector<String> Lines;
	size_t Iterate = (size_t)Math::Max((int32)((Binary.size() - 1) / BinaryEachLine + 1), (int32)0);
	Lines.reserve(Iterate);

	for (size_t i = 0; i < Iterate; ++i)
	{
		size_t Start = i * BinaryEachLine;
		size_t End = Math::Min(Start + BinaryEachLine, Binary.size());
		auto Span = std::span(Binary.data() + Start, Binary.data() + End);
		Lines.emplace_back(String::Join(TEXT(", "), Span | Linq::Select(LP1R(p, String::Format(TEXT("{:>3}"), p)))));
	}

	String Body = String::Join(TEXT(",\n\t"), Lines);
	Body = String::Format(HeaderFormatBase, Name, Body);
	co_await File::CompareAndWriteAllTextAsync(OutputFile, Body, InCancellationToken);

	Timer.Stop();
	Console::WriteLine(TEXT("{} ({:.2f}s)"), InputFile.GetFileName(), Timer.GetElapsed().GetTotalSeconds<double>());

	co_return 0;
}

void NSCWApp::PrintUsage(TextWriter& Writer)
{
	String Usage = TEXT(R"(Usage: 
-Input    : The source shader code directory.
-Output   : The binary code directory.
)");
	Writer.Write(Usage);
}