// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SourceCode
{
	const String Path;
	const String SourceText;

protected:
	SourceCode(String InPath, String InSourceText);
	
public:
	virtual ~SourceCode() noexcept;

	String GetSourcePath() const;
	String GetSource() const;
	virtual Task<> SaveAsync(std::stop_token SToken = {});

protected:
	static Task<> CompareAndSaveAsync(String InPath, String Text, uint32 Encoding, std::stop_token SToken = {});
};