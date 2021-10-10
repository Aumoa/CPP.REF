// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ChessAIStructures.h"
#include "ChessSystemComponent.h"
#include "Queries/ChessQueries.h"

class AChessBoardProxy;
class AGridIndicator;

class CHESS_API IndicatingComponent : public ChessSystemComponent
{
	GENERATED_BODY(IndicatingComponent)

private:
	AGridIndicator* _hoverIndicator = nullptr;
	GridIndex _hoverIndex;
	AGridIndicator* _selectedIndicator = nullptr;
	std::optional<GridIndex> _selectIndex;
	std::vector<AGridIndicator*> _movableIndicators;
	std::vector<AGridIndicator*> _attackIndicators;
	std::vector<AGridIndicator*> _specialIndicators;

public:
	IndicatingComponent();

	virtual void SetupBoard(AChessBoardProxy* board) override;

	void UpdateHoverIndicator(const Vector3& worldLocation);
	void UpdateSelected(std::optional<GridIndex> location = std::nullopt);

	using ActionRequestDelegate = MulticastEvent<IndicatingComponent, void(const GridIndex& from, const GridIndex& to)>;
	ActionRequestDelegate ActionRequest;

private:
	void UpdateSelectIndicator(bool bActive);
	void SetIndicatorLocation(AGridIndicator* indicator, const GridIndex& location);
	void UpdateMovableIndicators(const MovablePointsQuery& results);
	void UpdateAttackIndicators(const MovablePointsQuery& results);
	void UpdateSpecialIndicators(const MovablePointsQuery& results);
	void UpdateIndicatorsImpl(std::vector<AGridIndicator*>& container, MovablePointsArray::FigureType type, const MovablePointsQuery& results, std::function<AGridIndicator*()> ctor);
};