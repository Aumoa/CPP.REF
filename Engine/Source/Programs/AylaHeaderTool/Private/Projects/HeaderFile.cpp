// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Projects/HeaderFile.h"

HeaderFile::HeaderFile(String SourceFile, String SourceCode)
	: SourceCode(SourceFile, SourceCode)
{
}

bool HeaderFile::Parse(const AAssembly& InAssembly)
{
	String SourceText = GetSource();
	SourceCodeLocator Locator(GetSourcePath(), SourceText);

	while (!Locator.IsEOF())
	{
		std::unique_ptr<AType> Parsed = AType::Parse(InAssembly, Locator);
		if (Parsed == nullptr)
		{
			break;
		}

		Types.emplace_back(std::move(Parsed));
	}

	return Types.size() > 0;
}