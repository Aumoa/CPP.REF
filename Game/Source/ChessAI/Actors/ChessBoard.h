// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "Level/World.h"
#include "ChessAIStructures.h"
#include "Queries/ChessQueries.h"

class AChessBoardProxy;
class APiece;

class AChessBoard : public AActor
{
public:
	using Super = AActor;

private:
	struct ChessBoardBuilt
	{
		struct Mark
		{
			APiece* PlacedActor;
			uint8 bMarkAttackWhite : 1;
			uint8 bMarkAttackBlack : 1;
		};

		const AChessBoard* Owner = nullptr;
		Mark Marks[8][8] = {};

		void Init(const AChessBoard* board);
		void Build();
		void SimulateMoveAndBuild(const GridIndex& from, const GridIndex& to);

		bool HasPiece(const GridIndex& index) const;
		APiece* GetPiece(const GridIndex& index) const;
	};

private:
	Degrees _rotation;
	APiece* _pieces[8][8] = {};
	EChessTeam _turn = EChessTeam::White;
	ChessBoardBuilt _built;

public:
	AChessBoard();

	void InitBoard(World* world);
	AChessBoardProxy* CreateProxy(EChessTeam team);

	Vector3 GetBoardCellPosition(const GridIndex& index) const;
	GridIndex GetGridIndexFromPosition(const Vector3& location) const;
	inline const ChessBoardBuilt& GetBoardBuilt() const { return _built; }

	template<std::derived_from<APiece> T>
	T* SpawnPiece(EChessTeam team, const GridIndex& index)
	{
		World* const world = GetWorld();
		T* piece = world->SpawnActor<T>();
		Internal_SpawnPiece(piece, team, index);
		return piece;
	}

	ActionRecord MovePiece(const GridIndex& from, const GridIndex& to);
	inline EChessTeam GetTurn() const { return _turn; }
	void SimulateMoveQuery(MovablePointsQuery& query) const;

private:
	void Internal_SpawnPiece(APiece* piece, EChessTeam team, const GridIndex& index);
	bool Internal_MoveTurn();
};