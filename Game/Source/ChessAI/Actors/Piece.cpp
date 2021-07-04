// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Piece.h"
#include "GameEngine.h"
#include "LogChessAI.h"
#include "ChessBoard.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "Shaders/ColorShader/ColorShader.h"

using enum ELogVerbosity;

APiece::APiece() : Super()
{
	_meshComponent = CreateSubobject<StaticMeshComponent>();
	SetRootComponent(_meshComponent);
}

void APiece::Init(AChessBoard* board, EChessTeam team, const GridIndex& index)
{
	if (_board != nullptr)
	{
		LogSystem::Log(LogChessAI, Error, L"APiece::Init() function can be call only first time.");
		return;
	}

	_board = board;
	_meshComponent->SetStaticMesh(GetStaticMesh());
	_team = team;
	_myIndex = index;

	ColorShader* cshader = GameEngine::GetEngine()->GetColorShader();
	MaterialInstance* color = CreateSubobject<MaterialInstance>(cshader->GetDefaultMaterial());
	color->SetVector3ParameterValueByName(L"Color", team == EChessTeam::Black ? 0.2f : 0.8f);
	_meshComponent->SetMaterial(0, color);

	_meshComponent->SetLocation(board->GetBoardCellPosition(index));
	if (team == EChessTeam::White)
	{
		_meshComponent->SetRotation(Quaternion::FromAxisAngle(Vector3(0, 1, 0), 180.0f));
	}
}

bool APiece::SimulateMove(const GridIndex& index)
{
	_meshComponent->SetLocation(_board->GetBoardCellPosition(index));
	_myIndex = index;
	return true;
}

bool APiece::QueryMovable(MovablePointsQuery& query) const
{
	checkf(false, L"NOT IMPLEMENTED. MAKE IT TO PURE VIRTUAL FUNCTION.");
	return false;
}