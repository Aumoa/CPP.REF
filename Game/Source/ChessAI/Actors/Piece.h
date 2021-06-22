// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "ChessAIStructures.h"

class AChessBoard;
class StaticMesh;
class StaticMeshComponent;

class APiece : public AActor
{
public:
	using Super = AActor;

private:
	AChessBoard* _board = nullptr;
	StaticMeshComponent* _meshComponent = nullptr;
	EChessTeam _team = EChessTeam::Black;
	GridIndex _myIndex;

public:
	APiece();

	virtual void Init(AChessBoard* board, EChessTeam team, const GridIndex& index);

protected:
	virtual StaticMesh* GetStaticMesh() const = 0;
};