// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Projects/SourceCode.h"
#include "Reflection/AType.h"

class AAssembly;

class HeaderFile : public SourceCode
{
	const AAssembly* AssemblyRef;
	String IntermediatePath;
	std::vector<std::unique_ptr<AType>> Types;

public:
	HeaderFile(String SourceFile, String SourceCode, String IntermediatePath);

	virtual Task<> SaveAsync(std::stop_token SToken = {}) override;
	bool Parse(const AAssembly& InAssembly);

	String GeneratedHeader() const;
	String GeneratedSource() const;

private:
	String GeneratedTypeHeader(AType* InType) const;
	String GeneratedTypeSource(AType* InType) const;
};