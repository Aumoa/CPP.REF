// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "IndicatingComponent.h"
#include "Level/World.h"
#include "Actors/ChessBoard.h"
#include "Actors/GridIndicator.h"
#include "Actors/Piece.h"
#include "Pawns/ChessBoardProxy.h"
#include "Components/PrimitiveComponent.h"
#include "Queries/ChessQueries.h"

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
		SetIndicatorLocation(_hoverIndicator, _hoverIndex);
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
	AChessBoard* board = GetBoard();
	AChessBoardProxy* proxy = GetProxy();

	auto finally = [&]()
	{
		_selectIndex = nullopt;
		UpdateSelectIndicator(false);
	};

	if (!gridIndex.IsValid())
	{
		finally();
		return;
	}

	if (_selectIndex.has_value() && *_selectIndex == gridIndex)
	{
		finally();
		return;
	}

	if (!proxy->CanSelect(gridIndex))
	{
		if (_selectIndex.has_value())
		{
			ActionRequest.Invoke(*_selectIndex, gridIndex);
		}
		finally();
		return;
	}

	_selectIndex = gridIndex;
	UpdateSelectIndicator(true);
}

void IndicatingComponent::UpdateSelectIndicator(bool bActive)
{
	AChessBoard* board = GetBoard();
	World* world = board->GetWorld();
	auto* primitiveComponent = _selectedIndicator->GetRootComponentAs<PrimitiveComponent>();

	if (bActive)
	{
		SetIndicatorLocation(_selectedIndicator, *_selectIndex);
		primitiveComponent->SetHiddenInGame(false);

		APiece* piece = board->GetPiece(*_selectIndex);
		MovablePointsQuery query = { .Type = MovablePointsQuery::QueryType::Move };
		if (piece->QueryMovable(query))
		{
			// Make pending indicators.
			size_t count = query.GetPointsCount();
			if (_movableIndicators.size() < count)
			{
				_movableIndicators.reserve(count);
				for (size_t i = _movableIndicators.size(); i < count; ++i)
				{
					AGridIndicator*& indicator = _movableIndicators.emplace_back(world->SpawnActor<AGridIndicator>());
					indicator->GetRootComponent()->AttachToComponent(board->GetRootComponent());
					indicator->GetRootComponent()->SetScale(Vector3(1, 0.05f, 1));
					indicator->SetIndicatorColor(NamedColors::Aquamarine);
				}
			}

			// Setup locations.
			size_t idx = 0;
			for (auto& arr : query.Results)
			{
				for (auto& point : arr.Points)
				{
					AGridIndicator*& indicator = _movableIndicators[idx++];
					SetIndicatorLocation(indicator, point);
					if (auto* primitive = indicator->GetRootComponentAs<PrimitiveComponent>(); primitive != nullptr)
					{
						primitive->SetHiddenInGame(false);
					}
				}
			}
		}
	}
	else
	{
		primitiveComponent->SetHiddenInGame(true);

		for (auto& indicator : _movableIndicators)
		{
			if (auto* primitive = indicator->GetRootComponentAs<PrimitiveComponent>(); primitive != nullptr)
			{
				primitive->SetHiddenInGame(true);
			}
		}
	}
}

void IndicatingComponent::SetIndicatorLocation(AGridIndicator* indicator, const GridIndex& location)
{
	AChessBoard* board = GetBoard();
	Vector3 amendedLocation = board->GetBoardCellPosition(location);
	indicator->SetActorLocation(amendedLocation);
}