// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class EngineCriticalException : public Exception
{
public:
	using Super = Exception;
	using This = EngineCriticalException;

private:
	TRefPtr<String> title;
	TRefPtr<String> message;

public:
	EngineCriticalException(TRefPtr<String> title, TRefPtr<String> message);
	~EngineCriticalException() override;

	TRefPtr<String> GetTitle() const;
	TRefPtr<String> GetMessage() const;
};