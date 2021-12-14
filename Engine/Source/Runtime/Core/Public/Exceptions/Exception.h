// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class CORE_API SException : implements SObject
{
	GENERATED_BODY(SException)

private:
	SPROPERTY(Message)
	std::wstring Message;
	SPROPERTY(InnerException)
	SException* InnerException = nullptr;

public:
	SException();
	SException(std::wstring_view Message, SException* InnerException = nullptr);

	virtual std::wstring ToString() override;
};