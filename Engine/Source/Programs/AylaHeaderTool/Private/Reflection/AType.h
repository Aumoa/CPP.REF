// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Compiler/SourceCodeLocator.h"
#include "Compiler/SourceCodeLocation.h"

class AAssembly;

class AType
{
	const AAssembly* const RefAssembly;
	const SourceCodeLocation Location;

	String ClassKey;

protected:
	AType(const AAssembly& InAssembly, SourceCodeLocator& Locator);

public:
	virtual ~AType() noexcept;

	static std::unique_ptr<AType> Parse(const AAssembly& InAssembly, SourceCodeLocator& Locator);

	virtual SourceCodeLocation GetSourceLocation() const;
	virtual String GetClassKey() const;
	virtual String GetTypeName() const = 0;
};