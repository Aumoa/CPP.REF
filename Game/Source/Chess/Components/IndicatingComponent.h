// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "ChessAIStructures.h"
#include "Components/ActorComponent.h"

class AChessBoardProxy;
class AGridIndicator;

class IndicatingComponent : public ActorComponent
{
public:
	using Super = ActorComponent;

private:
	AChessBoardProxy* _board = nullptr;
	AGridIndicator* _hoverIndicator = nullptr;
	GridIndex _hoverIndex;
	AGridIndicator* _selectedIndicator = nullptr;
	std::optional<GridIndex> _selectIndex;

public:
	IndicatingComponent();

	void SetupBoard(AChessBoardProxy* board);

	void UpdateHoverIndicator(const Vector3& worldLocation);
	void UpdateSelected(std::optional<GridIndex> location = std::nullopt);

	using ActionRequestDelegate = MulticastEvent<IndicatingComponent, void(const GridIndex& from, const GridIndex& to)>;
	ActionRequestDelegate ActionRequest;
};