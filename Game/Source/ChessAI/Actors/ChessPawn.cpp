// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ChessPawn.h"
#include "GameEngine.h"
#include "Assets/AssetImporter.h"
#include "Actors/ChessBoard.h"

AChessPawn::AChessPawn() : Super()
{
}

bool AChessPawn::SimulateMove(const GridIndex& index)
{
	bool bMoved = Super::SimulateMove(index);
	if (bMoved)
	{
		_bFirst = false;
	}
	return bMoved;
}

bool AChessPawn::QueryMovable(MovablePointsQuery& query) const
{
	switch (query.Type)
	{
	case MovablePointsQuery::QueryType::Move:
		QueryMove(query);
		break;
	default:
		return false;
	}

	return true;
}

StaticMesh* AChessPawn::GetStaticMesh() const
{
	static StaticMesh* globalMesh = GameEngine::GetEngine()->GetAssetImporter()->ImportStaticMesh(AssetPath);
	return globalMesh;
}

inline int32 AChessPawn::GetIncrementer() const
{
	switch (GetTeam())
	{
	case EChessTeam::Black:
		return -1;
	case EChessTeam::White:
		return +1;
	}

	checkf(false, L"Invalid team.");
	return 0;
}

void AChessPawn::QueryMove(MovablePointsQuery& query) const
{
	auto checkAndEmplace = [&](auto& container, const GridIndex& loc)
	{
		AChessBoard* board = GetBoard();
		if (board->HasPiece(loc))
		{
			return false;
		}

		container.emplace_back(loc);
		return true;
	};

	int32 incrementer = GetIncrementer();
	MovablePointsArray* figure = query.BeginFigure();

	GridIndex location = GetIndex();
	location.Y += GetIncrementer();
	if (!checkAndEmplace(figure->Points, location))
	{
		return;
	}

	if (_bFirst)
	{
		location.Y += GetIncrementer();
		checkAndEmplace(figure->Points, location);
	}
}

void AChessPawn::QueryAttack(MovablePointsQuery& query) const
{
}