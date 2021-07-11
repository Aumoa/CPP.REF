// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ChessBoardBuilt.h"
#include "Actors/ChessBoard.h"
#include "Actors/King.h"

using namespace std;

ChessBoardBuilt::ChessBoardBuilt(const AChessBoard* board) : _board(board)
{
}

void ChessBoardBuilt::Init()
{
	for (size_t i = 0; i < 8; ++i)
	{
		for (size_t j = 0; j < 8; ++j)
		{
			Mark& mark = _marks[i][j];
			mark.PlacedActor = _board->_pieces[i][j];
			mark.bMarkAttackBlack = false;
			mark.bMarkAttackWhite = false;
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

				const bool bBlack = mark.PlacedActor->GetTeam() == EChessTeam::Black;
				for (auto& result : query.Results)
				{
					for (auto& point : result.Points)
					{
						Mark& pointMark = _marks[point.X][point.Y];
						if (bBlack)
						{
							pointMark.bMarkAttackBlack = true;
						}
						else
						{
							pointMark.bMarkAttackWhite = true;
						}
					}
				}
			}
		}
	}
}

void ChessBoardBuilt::SimulateMoveAndBuild(const GridIndex& from, const GridIndex& to)
{
	Mark& mark = _marks[from.X][from.Y];
	Mark& markTo = _marks[to.X][to.Y];
	check(mark.PlacedActor);

	// Change index to destination.
	mark.PlacedActor->SetIndex(to);
	markTo.PlacedActor = mark.PlacedActor;
	mark.PlacedActor = nullptr;

	// Build.
	Build();

	// Restore indes to source.
	markTo.PlacedActor->SetIndex(from);

	// KEEP board status for checking piece's validation.
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