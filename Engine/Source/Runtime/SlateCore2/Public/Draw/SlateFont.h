// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline SlateCore
{
	struct SlateFont
	{
		String FamilyName;
		//IRHIFontCollection* Collection = nullptr;
		float Size = 11.0f;

		SlateFont()
		{
		}

		//SlateFont(String InFamilyName, float InSize, IRHIFontCollection* InCollection = nullptr)
		//	: FamilyName(InFamilyName)
		//	, Collection(InCollection)
		//	, Size(InSize)
		//{
		//}

		String ToString(String InFormatArgs = TEXT("")) const
		{
			return String::Format(TEXT("FamilyName={} Size={}"), FamilyName, Size);
		}

		//bool operator ==(const SlateFont& Rhs) const
		//{
		//	return FamilyName == Rhs.FamilyName
		//		&& Collection == Rhs.Collection
		//		&& Size == Rhs.Size;
		//}

		//bool operator !=(const SlateFont& Rhs) const
		//{
		//	return FamilyName != Rhs.FamilyName
		//		|| Collection != Rhs.Collection
		//		|| Size != Rhs.Size;
		//}
	};
}