// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "IndicatingComponent.h"
#include "Level/World.h"
#include "Actors/ChessBoard.h"
#include "Actors/GridIndicator.h"
#include "Components/PrimitiveComponent.h"

using namespace std;

IndicatingComponent::IndicatingComponent() : Super()
{
}

void IndicatingComponent::SetupBoard(AChessBoard* board)
{
	_board = board;
	World* const world = GetWorld();

	// READY HOVER INDICATOR
	_hoverIndicator = world->SpawnActor<AGridIndicator>();
	_hoverIndicator->GetRootComponent()->AttachToComponent(board->GetRootComponent());
	_hoverIndicator->GetRootComponent()->SetScale(Vector3(1, 0.05f, 1));
	_hoverIndicator->SetIndicatorColor(NamedColors::Gray);

	// READY SELECT INDICATOR
	_selectedIndicator = world->SpawnActor<AGridIndicator>();
	_selectedIndicator->GetRootComponent()->AttachToComponent(board->GetRootComponent());
	_selectedIndicator->GetRootComponent()->SetScale(Vector3(1, 0.05f, 1));
	_selectedIndicator->SetIndicatorColor(NamedColors::Aquamarine);
}

void IndicatingComponent::UpdateHoverIndicator(const Vector3& worldLocation)
{
	checkf(_hoverIndicator != nullptr, L"Board is not setted up. Please call SetupBoard() function to initialize system.");
	auto* primitiveComponent = dynamic_cast<PrimitiveComponent*>(_hoverIndicator->GetRootComponent());
	checkf(primitiveComponent != nullptr, L"The root component of hover indicator is not primitive component.");

	_hoverIndex = _board->GetGridIndexFromPosition(worldLocation);
	if (_hoverIndex.IsValid())
	{
		Vector3 amendedLocation = _board->GetBoardCellPosition(_hoverIndex);
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
	auto* primitiveComponent = _selectedIndicator->GetRootComponentAs<PrimitiveComponent>();
	GridIndex gridIndex = location.value_or(_hoverIndex);
	if (!gridIndex.IsValid())
	{
		primitiveComponent->SetHiddenInGame(true);
		return;
	}

	Vector3 position = _board->GetBoardCellPosition(gridIndex);
	primitiveComponent->SetLocation(position);
	primitiveComponent->SetHiddenInGame(false);
}