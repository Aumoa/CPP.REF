// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "ChessAIStructures.h"
#include "Components/ActorComponent.h"

class AChessBoard;
class AGridIndicator;

class IndicatingComponent : public ActorComponent
{
public:
	using Super = ActorComponent;

private:
	AChessBoard* _board = nullptr;
	AGridIndicator* _hoverIndicator = nullptr;
	GridIndex _hoverIndex;
	AGridIndicator* _selectedIndicator = nullptr;

public:
	IndicatingComponent();

	void SetupBoard(AChessBoard* board);

	void UpdateHoverIndicator(const Vector3& worldLocation);
	void UpdateSelected(std::optional<GridIndex> location = std::nullopt);
};