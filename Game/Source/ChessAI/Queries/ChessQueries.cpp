// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ChessQueries.h"
#include "Actors/Piece.h"
#include "Actors/ChessBoard.h"
#include "Components/PrimitiveComponent.h"

using namespace std;

bool MovablePointsArray::CheckAndEmplace(const APiece* piece, const GridIndex& location)
{
	if (!location.IsValid())
	{
		return false;
	}

	AChessBoard* board = piece->GetBoard();
	APiece* getPiece = board->GetBoardBuilt().GetPiece(location);
	if (getPiece)
	{
		if (Type == FigureType::Move)
		{
			return false;
		}
		else if (Type == FigureType::Attack && piece->GetTeam() == getPiece->GetTeam())
		{
			return false;
		}
	}

	this->Points.emplace_back(location);
	return true;
}

MovablePointsArray* MovablePointsArrayPointer::operator ->()
{
	check(Root);
	check(MyIndex != (size_t)-1);
	return &Root->Results[MyIndex];
}

MovablePointsArrayPointer MovablePointsQuery::BeginFigure(MovablePointsArray::FigureType figureType)
{
	size_t idx = Results.size();
	Results.emplace_back().Type = figureType;
	return { this, idx };
}

size_t MovablePointsQuery::GetPointsCount() const
{
	size_t n = 0;
	for (size_t i = 0; i < Results.size(); ++i)
	{
		n += Results[i].Points.size();
	}
	return n;
}

size_t MovablePointsQuery::GetPointsCount(MovablePointsArray::FigureType figureType) const
{
	size_t n = 0;
	for (size_t i = 0; i < Results.size(); ++i)
	{
		if (Results[i].Type == figureType)
		{
			n += Results[i].Points.size();
		}
	}
	return n;
}

const MovablePointsArray* MovablePointsQuery::GetHit(const GridIndex& loc) const
{
	for (auto& figure : Results)
	{
		for (auto& point : figure.Points)
		{
			if (point == loc)
			{
				return &figure;
			}
		}
	}

	return nullptr;
}

#define checkValid() checkf(_bValid, L"Record is not valid.")

ActionRecord::ActionRecord(bool bValid) : _bValid(false)
{
	checkf(!bValid, L"Invalid parameter: bValid == true. If you determine that action record is valid, see constructor function that call with APiece*, function<void()>.");
}

ActionRecord::ActionRecord(APiece* actor, function<void()> undoRecord) : _bValid(true)
	, _actor(actor)
	, _undoRecord(undoRecord)
{
}

void ActionRecord::Undo() const
{
	checkValid();

	if (_undoRecord)
	{
		_undoRecord();
	}

	if (_actorTaked != nullptr)
	{
		auto* primitive = _actorTaked->GetRootComponentAs<PrimitiveComponent>();
		primitive->SetHiddenInGame(false);
	}
}

void ActionRecord::TakeActor(APiece* actor)
{
	checkf(_actorTaked == nullptr, L"Taked actor already assigned.");
	checkf(actor != nullptr, L"Take actor is nullptr.");
	_actorTaked = actor;
	auto* primitive = actor->GetRootComponentAs<PrimitiveComponent>();
	primitive->SetHiddenInGame(true);
}

bool ActionRecord::IsValid() const
{
	return _bValid;
}

#undef checkValid