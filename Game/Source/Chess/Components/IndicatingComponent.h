// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "ChessAIStructures.h"
#include "ChessSystemComponent.h"

class AChessBoardProxy;
class AGridIndicator;

class IndicatingComponent : public ChessSystemComponent
{
public:
	using Super = ChessSystemComponent;

private:
	AGridIndicator* _hoverIndicator = nullptr;
	GridIndex _hoverIndex;
	AGridIndicator* _selectedIndicator = nullptr;
	std::optional<GridIndex> _selectIndex;

public:
	IndicatingComponent();

	virtual void SetupBoard(AChessBoardProxy* board) override;

	void UpdateHoverIndicator(const Vector3& worldLocation);
	void UpdateSelected(std::optional<GridIndex> location = std::nullopt);

	using ActionRequestDelegate = MulticastEvent<IndicatingComponent, void(const GridIndex& from, const GridIndex& to)>;
	ActionRequestDelegate ActionRequest;
};