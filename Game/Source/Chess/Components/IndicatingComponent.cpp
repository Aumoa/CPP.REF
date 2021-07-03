// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "IndicatingComponent.h"
#include "Level/World.h"
#include "Actors/ChessBoard.h"
#include "Actors/GridIndicator.h"
#include "Pawns/ChessBoardProxy.h"
#include "Components/PrimitiveComponent.h"

using namespace std;

IndicatingComponent::IndicatingComponent() : Super()
{
}

void IndicatingComponent::SetupBoard(AChessBoardProxy* board)
{
	Super::SetupBoard(board);

	World* const world = GetWorld();
	AChessBoard* const chessBoard = GetBoard();

	// READY HOVER INDICATOR
	_hoverIndicator = world->SpawnActor<AGridIndicator>();
	_hoverIndicator->GetRootComponent()->AttachToComponent(chessBoard->GetRootComponent());
	_hoverIndicator->GetRootComponent()->SetScale(Vector3(1, 0.05f, 1));
	_hoverIndicator->SetIndicatorColor(NamedColors::Gray);

	// READY SELECT INDICATOR
	_selectedIndicator = world->SpawnActor<AGridIndicator>();
	_selectedIndicator->GetRootComponent()->AttachToComponent(chessBoard->GetRootComponent());
	_selectedIndicator->GetRootComponent()->SetScale(Vector3(1, 0.05f, 1));
	_selectedIndicator->SetIndicatorColor(NamedColors::Aquamarine);
}

void IndicatingComponent::UpdateHoverIndicator(const Vector3& worldLocation)
{
	checkf(_hoverIndicator != nullptr, L"Board is not setted up. Please call SetupBoard() function to initialize system.");
	auto* primitiveComponent = dynamic_cast<PrimitiveComponent*>(_hoverIndicator->GetRootComponent());
	checkf(primitiveComponent != nullptr, L"The root component of hover indicator is not primitive component.");

	AChessBoard* board = GetBoard();

	_hoverIndex = board->GetGridIndexFromPosition(worldLocation);
	if (_hoverIndex.IsValid())
	{
		Vector3 amendedLocation = board->GetBoardCellPosition(_hoverIndex);
		primitiveComponent->SetLocation(amendedLocation);
		primitiveComponent->SetHiddenInGame(false);
	}
	else
	{
		primitiveComponent->SetHiddenInGame(true);
	}
}

void IndicatingComponent::UpdateSelected(optional<GridIndex> location)
{
#define finally() \
primitiveComponent->SetHiddenInGame(true);\
_selectIndex = nullopt;\
return;

	auto* primitiveComponent = _selectedIndicator->GetRootComponentAs<PrimitiveComponent>();
	GridIndex gridIndex = location.value_or(_hoverIndex);
	AChessBoard* board = GetBoard();
	AChessBoardProxy* proxy = GetProxy();

	if (!gridIndex.IsValid())
	{
		finally();
	}

	if (_selectIndex.has_value() && *_selectIndex == gridIndex)
	{
		finally();
	}

	if (!proxy->CanSelect(gridIndex))
	{
		if (_selectIndex.has_value())
		{
			ActionRequest.Invoke(*_selectIndex, gridIndex);
		}
		finally();
	}

	Vector3 position = board->GetBoardCellPosition(gridIndex);
	primitiveComponent->SetLocation(position);
	primitiveComponent->SetHiddenInGame(false);
	_selectIndex = gridIndex;
}