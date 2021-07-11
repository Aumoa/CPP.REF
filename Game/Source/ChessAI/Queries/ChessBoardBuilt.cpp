// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ChessBoardBuilt.h"
#include "Actors/ChessBoard.h"
#include "Actors/King.h"

ChessBoardBuilt::ChessBoardBuilt(const AChessBoard* board) : _board(board)
{
}

void ChessBoardBuilt::Init()
{
	for (size_t i = 0; i < 8; ++i)
	{
		for (size_t j = 0; j < 8; ++j)
		{
			_marks[i][j].PlacedActor = _board->_pieces[i][j];
		}
	}
}

void ChessBoardBuilt::Build()
{
	for (int32 i = 0; i < 8; ++i)
	{
		for (int32 j = 0; j < 8; ++j)
		{
			Mark& mark = _marks[i][j];
			if (mark.PlacedActor != nullptr)
			{
				MovablePointsQuery query;
				if (!mark.PlacedActor->QueryMovable(query, *this))
				{
					continue;
				}

				for (auto& result : query.Results)
				{
					for (auto& point : result.Points)
					{

					}
				}
			}
		}
	}
}

void ChessBoardBuilt::SimulateMoveAndBuild(const GridIndex& from, const GridIndex& to)
{
}

bool ChessBoardBuilt::HasPiece(const GridIndex& index) const
{
	return GetPiece(index);
}

APiece* ChessBoardBuilt::GetPiece(const GridIndex& index) const
{
	return _marks[index.X][index.Y].PlacedActor;
}

bool ChessBoardBuilt::IsChecked(GridIndex& index) const
{
	for (int32 i = 0; i < 8; ++i)
	{
		for (int32 j = 0; j < 8; ++j)
		{
			const Mark& mark = _marks[i][j];
			const bool bChecked = mark.bMarkAttackBlack || mark.bMarkAttackWhite;
			if (bChecked)
			{
				if (auto* isKing = dynamic_cast<AKing*>(_marks[i][j].PlacedActor); isKing)
				{
					EChessTeam team = isKing->GetTeam();
					if (team == EChessTeam::Black && mark.bMarkAttackWhite)
					{
						index = GridIndex(i, j);
						return true;
					}
					else if (team == EChessTeam::White && mark.bMarkAttackBlack)
					{
						index = GridIndex(i, j);
						return true;
					}
				}
			}
		}
	}

	return false;
}