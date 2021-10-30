// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "WindowsIncludes.h"
#include "WindowsStrokeStyle.h"

SWindowsStrokeStyle::SWindowsStrokeStyle(ID2D1StrokeStyle* InStyle) : Super()
	, Style(InStyle)
{
}

RHIStrokeStyleDesc SWindowsStrokeStyle::GetDesc()
{
	return RHIStrokeStyleDesc
	{
		.StartCap = (ERHIStrokeCapStyle)Style->GetStartCap(),
		.EndCap = (ERHIStrokeCapStyle)Style->GetEndCap(),
		.DashCap = (ERHIStrokeCapStyle)Style->GetDashCap(),
		.LineJoin = (ERHIStrokeJoin)Style->GetLineJoin(),
		.MiterLimit = Style->GetMiterLimit(),
		.DashStyle = (ERHIStrokeDashStyle)Style->GetDashStyle(),
		.DashOffset = Style->GetDashOffset()
	};
}

std::vector<float> SWindowsStrokeStyle::GetDashes()
{
	std::vector<float> Dashes((size_t)Style->GetDashesCount());
	Style->GetDashes(Dashes.data(), (UINT32)Dashes.size());
	return Dashes;
}

ID2D1StrokeStyle* SWindowsStrokeStyle::GetStyle()
{
	return Style.Get();
}