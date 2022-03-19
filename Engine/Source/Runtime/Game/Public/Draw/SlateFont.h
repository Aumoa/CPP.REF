// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

struct IRHIFontCollection;

struct SlateFont
{
	std::wstring FamilyName;
	IRHIFontCollection* Collection = nullptr;
	float Size = 11.0f;

	SlateFont()
	{
	}

	SlateFont(std::wstring InFamilyName, float InSize, IRHIFontCollection* InCollection = nullptr)
		: FamilyName(InFamilyName)
		, Collection(InCollection)
		, Size(InSize)
	{
	}

	std::wstring ToString(std::wstring_view InFormatArgs = L"") const
	{
		return String::Format(L"FamilyName={} Size={}", FamilyName, Size);
	}

	bool operator ==(const SlateFont& Rhs) const
	{
		return FamilyName == Rhs.FamilyName
			&& Collection == Rhs.Collection
			&& Size == Rhs.Size;
	}

	bool operator !=(const SlateFont& Rhs) const
	{
		return FamilyName != Rhs.FamilyName
			|| Collection != Rhs.Collection
			|| Size != Rhs.Size;
	}
};