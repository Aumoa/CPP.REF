// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/IndicatingComponent.h"
#include "Level/World.h"
#include "Actors/ChessBoard.h"
#include "Actors/GridIndicator.h"
#include "Actors/Piece.h"
#include "Pawns/ChessBoardProxy.h"
#include "Components/PrimitiveComponent.h"
#include "Queries/ChessQueries.h"

IndicatingComponent::IndicatingComponent() : Super()
{
}

void IndicatingComponent::SetupBoard(AChessBoardProxy* board)
{
	Super::SetupBoard(board);

	SWorld* const world = GetWorld();
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
	auto* primitiveComponent = dynamic_cast<SPrimitiveComponent*>(_hoverIndicator->GetRootComponent());
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

void IndicatingComponent::UpdateSelected(std::optional<GridIndex> location)
{
	auto* primitiveComponent = _selectedIndicator->GetRootComponentAs<SPrimitiveComponent>();
	GridIndex gridIndex = location.value_or(_hoverIndex);
	AChessBoard* board = GetBoard();
	AChessBoardProxy* proxy = GetProxy();

	auto finally = [&]()
	{
		_selectIndex = std::nullopt;
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
	SWorld* world = board->GetWorld();
	auto* primitiveComponent = _selectedIndicator->GetRootComponentAs<SPrimitiveComponent>();

	if (bActive)
	{
		SetIndicatorLocation(_selectedIndicator, *_selectIndex);
		primitiveComponent->SetHiddenInGame(false);

		const ChessBoardBuilt& built = board->GetBoardBuilt();
		APiece* piece = built.GetPiece(*_selectIndex);
		MovablePointsQuery query;
		if (piece->QueryMovable(query, built))
		{
			board->SimulateMoveQuery(query);
			UpdateMovableIndicators(query);
			UpdateAttackIndicators(query);
			UpdateSpecialIndicators(query);
		}
	}
	else
	{
		primitiveComponent->SetHiddenInGame(true);
		UpdateMovableIndicators({});
		UpdateAttackIndicators({});
		UpdateSpecialIndicators({});
	}
}

void IndicatingComponent::SetIndicatorLocation(AGridIndicator* indicator, const GridIndex& location)
{
	AChessBoard* board = GetBoard();
	Vector3 amendedLocation = board->GetBoardCellPosition(location);
	indicator->SetActorLocation(amendedLocation);
}

void IndicatingComponent::UpdateMovableIndicators(const MovablePointsQuery& results)
{
	SWorld* const world = GetWorld();
	AChessBoard* const board = GetBoard();
	UpdateIndicatorsImpl(_movableIndicators, MovablePointsArray::FigureType::Move, results, [&]()
	{
		AGridIndicator* indicator = world->SpawnActor<AGridIndicator>();
		indicator->GetRootComponent()->AttachToComponent(board->GetRootComponent());
		indicator->GetRootComponent()->SetScale(Vector3(1, 0.05f, 1));
		indicator->SetIndicatorColor(NamedColors::Aquamarine);
		return indicator;
	});
}

void IndicatingComponent::UpdateAttackIndicators(const MovablePointsQuery& results)
{
	SWorld* const world = GetWorld();
	AChessBoard* const board = GetBoard();
	UpdateIndicatorsImpl(_attackIndicators, MovablePointsArray::FigureType::Attack, results, [&]()
	{
		AGridIndicator* indicator = world->SpawnActor<AGridIndicator>();
		indicator->GetRootComponent()->AttachToComponent(board->GetRootComponent());
		indicator->GetRootComponent()->SetScale(Vector3(1, 0.05f, 1));
		indicator->SetIndicatorColor(NamedColors::Red);
		return indicator;
	});
}

void IndicatingComponent::UpdateSpecialIndicators(const MovablePointsQuery& results)
{
	SWorld* const world = GetWorld();
	AChessBoard* const board = GetBoard();
	UpdateIndicatorsImpl(_attackIndicators, MovablePointsArray::FigureType::Special, results, [&]()
	{
		AGridIndicator* indicator = world->SpawnActor<AGridIndicator>();
		indicator->GetRootComponent()->AttachToComponent(board->GetRootComponent());
		indicator->GetRootComponent()->SetScale(Vector3(1, 0.05f, 1));
		indicator->SetIndicatorColor(NamedColors::Purple);
		return indicator;
	});
}

void IndicatingComponent::UpdateIndicatorsImpl(std::vector<AGridIndicator*>& container, MovablePointsArray::FigureType type, const MovablePointsQuery& results, std::function<AGridIndicator*()> ctor)
{
	SWorld* const world = GetWorld();
	AChessBoard* const board = GetBoard();

	// Make pending indicators.
	size_t count = results.GetPointsCount(type);
	if (container.size() < count)
	{
		container.reserve(count);
		for (size_t i = container.size(); i < count; ++i)
		{
			AGridIndicator*& indicator = container.emplace_back(ctor());
		}
	}
	else
	{
		for (size_t i = count; i < container.size(); ++i)
		{
			SPrimitiveComponent* primitive = container[i]->GetRootComponentAs<SPrimitiveComponent>();
			if (primitive != nullptr)
			{
				primitive->SetHiddenInGame(true);
			}
		}
	}

	// Setup locations.
	size_t idx = 0;
	for (auto& arr : results.Results)
	{
		if (arr.Type == type)
		{
			for (auto& point : arr.Points)
			{
				AGridIndicator*& indicator = container[idx++];
				SetIndicatorLocation(indicator, point);
				if (auto* primitive = indicator->GetRootComponentAs<SPrimitiveComponent>(); primitive != nullptr)
				{
					primitive->SetHiddenInGame(false);
				}
			}
		}
	}
}