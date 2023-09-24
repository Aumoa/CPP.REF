// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SLATE_API SSizeBox : public SCompoundWidget
{
	GENERATED_SLATE_BODY(SSizeBox)

private:
	std::optional<float> OverrideWidth;
	std::optional<float> OverrideHeight;

public:
	SSizeBox();
	virtual ~SSizeBox() noexcept override;

public:
	BEGIN_SLATE_ATTRIBUTE()
		DECLARE_SLATE_ATTRIBUTE(std::optional<float>, OverrideWidth)
		DECLARE_SLATE_ATTRIBUTE(std::optional<float>, OverrideHeight)
	END_SLATE_ATTRIBUTE();

	DECLARE_SLATE_CONSTRUCTOR();

protected:
	virtual Vector2 ComputeDesiredSize() const override;
};