// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Actors/ChessPawn.h"
#include "GameEngine.h"
#include "Assets/StaticMesh.h"

AChessPawn::AChessPawn() : Super()
{
}

ActionRecord AChessPawn::Move(const GridIndex& index, const ChessBoardBuilt& built)
{
	const bool bFirst = IsFirst();
	auto indexBack = GetIndex();

	ActionRecord record = Super::Move(index, built);
	if (!record)
	{
		return record;
	}

	int32 movedY = index.Y - indexBack.Y;
	_bCanEnpassant = abs(movedY) == 2;
	_bMoved = true;

	return ActionRecord(
		this,
		[&, bCanEnpassant = _bCanEnpassant, record = std::move(record)]()
		{
			_bCanEnpassant = bCanEnpassant;
			record.Undo();
		});
}

void AChessPawn::TurnChanged(EChessTeam changedTurn)
{
	if (!_bMoved)
	{
		_bCanEnpassant = false;
	}
	_bMoved = false;
}

bool AChessPawn::QueryMovable(MovablePointsQuery& query, const ChessBoardBuilt& built) const
{
	int32 incrementer = GetIncrementer();
	MovablePointsArrayPointer figure = query.BeginFigure(MovablePointsArray::FigureType::Move);

	GridIndex location = GetIndex();
	location.Y += GetIncrementer();
	const bool bBlocked = !figure->CheckAndEmplace(this, location, built);

	if (IsFirst() && !bBlocked)
	{
		figure->CheckAndEmplace(this, location + GridIndex(0, GetIncrementer()), built);
	}

	GridIndex left = location + GridIndex(-1, 0);
	GridIndex right = location + GridIndex(+1, 0);
	query.BeginFigure(MovablePointsArray::FigureType::Attack)->CheckAndEmplace(this, left, built);
	query.BeginFigure(MovablePointsArray::FigureType::Attack)->CheckAndEmplace(this, right, built);

	query.OwnerActor = this;
	return true;
}

bool AChessPawn::QueryInteractionWith(MovablePointsQuery& query, APiece* piece, const ChessBoardBuilt& built) const
{
	if (auto* isPawn = dynamic_cast<AChessPawn*>(piece); isPawn)
	{
		if (isPawn->GetTeam() != GetTeam() && isPawn->_bCanEnpassant)
		{
			GridIndex targetIdx = isPawn->GetIndex();
			GridIndex myIdx = GetIndex();
			if (targetIdx.Y == myIdx.Y)
			{
				GridIndex specialMove;
				specialMove.X = targetIdx.X;
				specialMove.Y = targetIdx.Y + GetIncrementer();

				MovablePointsArrayPointer figure = query.BeginFigure(MovablePointsArray::FigureType::Special);
				figure->CheckAndEmplace(this, specialMove, built, isPawn);
			}
		}
	}

	return true;
}

SStaticMesh* AChessPawn::GetStaticMesh() const
{
	static SStaticMesh* globalMesh = LoadObject<SStaticMesh>(AssetPath);
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