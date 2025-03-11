// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "App.h"
#include "Params.h"
#include "SourceCode/CppHeaderCode.h"

App App::s_App;
int32 App::s_ExitCode;

App& App::Run(CancellationToken cancellationToken)
{
	using namespace Linq;

	try
	{
		Params::Init();

		auto inputDir = Params::GetInputDirectory();
		std::vector<std::unique_ptr<SourceCode>> sourceCodes;
		auto isHeaderFile = [](const FileReference& file)
		{
			return file.GetExtensions().Equals(TEXT("h"), EStringComparison::CurrentCultureIgnoreCase);
		};

		for (auto file : inputDir.GetFiles(true) | Where(isHeaderFile))
		{
			std::unique_ptr<SourceCode> generatedCode;
			if (CppHeaderCode::TryGenerateCode(file, &generatedCode))
			{
				sourceCodes.emplace_back(std::move(generatedCode));
			}
		}

		Task<>::WhenAll(sourceCodes | Select([&](const auto& ptr) { return ptr->CompileAsync(cancellationToken); })).GetResult();

		for (auto& sourceCode : sourceCodes)
		{
			if (String errorStr; sourceCode->TryFormatError(&errorStr))
			{
				Console::Error.WriteLine(errorStr);
			}
		}
	}
	catch (const Exception& knownException)
	{
		Console::Error.WriteLine(knownException.ToString());
		s_ExitCode = -1;
	}
	return s_App;
}

int32 App::GetExitCode() const
{
	return 0;
}