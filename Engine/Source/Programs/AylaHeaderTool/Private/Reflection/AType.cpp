// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Reflection/AType.h"
#include "Reflection/AClass.h"
#include "Exceptions/TerminateException.h"

AType::AType(const AAssembly& InAssembly, SourceCodeLocator& Locator)
	: RefAssembly(&InAssembly)
	, Location(Locator.GetPosition())
{
	ClassKey = Locator.GetWord();
}

AType::~AType() noexcept
{
}

std::unique_ptr<AType> AType::Parse(const AAssembly& InAssembly, SourceCodeLocator& Locator)
{
	while (!Locator.IsEOF())
	{
		String View = Locator.GetCurrentView();

		static constexpr std::array<char_t, 1> Prefixes = { 'A' };
		size_t IndexOf = View.IndexOfAny(Prefixes, 0);
		if (IndexOf == -1)
		{
			Locator.SetEOF();
			break;
		}

		Locator.IncrementPos(IndexOf);
		View = Locator.GetCurrentView();

		if (View.StartsWith(TEXT("ACLASS()")))
		{
			Locator.IncrementPos(TEXT("ACLASS()").length());
			Locator.SkipWhitespace();
			return std::make_unique<AClass>(InAssembly, Locator);
		}
		else
		{
			Locator.IncrementPos(1);
			continue;
		}
	}

	return nullptr;
}

String AType::GetClassKey() const
{
	return ClassKey;
}

SourceCodeLocation AType::GetSourceLocation() const
{
	return Location;
}