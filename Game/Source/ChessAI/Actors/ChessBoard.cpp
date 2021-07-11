// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ChessBoard.h"
#include "GameEngine.h"
#include "LogChessAI.h"
#include "Level/World.h"
#include "Components/StaticMeshComponent.h"
#include "Assets/AssetImporter.h"
#include "Pawns/ChessBoardProxy.h"
#include "Shaders/ColorShader/ColorVertexFactory.h"
#include "Shaders/ColorShader/ColorShader.h"
#include "Materials/MaterialInstance.h"

#include "King.h"
#include "Queen.h"
#include "Bishop.h"
#include "Knight.h"
#include "Rook.h"
#include "ChessPawn.h"

using namespace std;
using namespace std::chrono;

using enum ELogVerbosity;

AChessBoard::AChessBoard() : Super()
	, _built(this)
{
	StaticMeshComponent* smc = CreateSubobject<StaticMeshComponent>();
	SetRootComponent(smc);

	AssetImporter* assimp = GameEngine::GetEngine()->GetAssetImporter();
	StaticMesh* sm = assimp->ImportStaticMesh(L"Contents/Chess/Arts/Board/Mesh/board.fbx");
	smc->SetStaticMesh(sm);

	ColorShader* cshader = GameEngine::GetEngine()->GetColorShader();
	MaterialInstance* black = CreateSubobject<MaterialInstance>(cshader->GetDefaultMaterial());
	black->SetVector3ParameterValueByName(L"Color", 0.2f);
	smc->SetMaterial(0, black);

	MaterialInstance* white = CreateSubobject<MaterialInstance>(cshader->GetDefaultMaterial());
	white->SetVector3ParameterValueByName(L"Color", 0.8f);
	smc->SetMaterial(1, white);
}

void AChessBoard::InitBoard(World* world)
{
	auto ForEachTeams = [](function<void(EChessTeam, int32, int32)> body)
	{
		body(EChessTeam::Black, 7, 6);
		body(EChessTeam::White, 0, 1);
	};

	ForEachTeams([&](EChessTeam team, int32 head, int32 forward)
	{
		APiece* piece = nullptr;
		
		// KING
		SpawnPiece<AKing>(team, GridIndex(4, head));
		
		// QUEEN
		SpawnPiece<AQueen>(team, GridIndex(3, head));

		auto get_index = [](int32 i, int32 j)
		{
			if (i == 0)
			{
				return j;
			}
			else
			{
				return 7 - j;
			}
		};

		for (int32 i = 0; i < 2; ++i)
		{
			// BISHOPS
			SpawnPiece<ABishop>(team, GridIndex(get_index(i, 2), head));

			// KNIGHTS
			SpawnPiece<AKnight>(team, GridIndex(get_index(i, 1), head));

			// ROOKS
			SpawnPiece<ARook>(team, GridIndex(get_index(i, 0), head));
		}

		for (int32 i = 0; i < 8; ++i)
		{
			SpawnPiece<AChessPawn>(team, GridIndex(i, forward));
		}
	});

	_built.Init();
}

AChessBoardProxy* AChessBoard::CreateProxy(EChessTeam team)
{
	AChessBoardProxy* proxy = CreateSubobject<AChessBoardProxy>(this);
	proxy->InitBoard(this, team);
	return proxy;
}

Vector3 AChessBoard::GetBoardCellPosition(const GridIndex& index) const
{
	// In relevance of mesh, the zero index of grid is top-right based, and offset will
	// increasingly to negative on cell moving.
	int32 actualX = index.X - 7;
	int32 actualY = index.Y - 7;
	return Vector3((float)actualX, 0, (float)actualY);
}

GridIndex AChessBoard::GetGridIndexFromPosition(const Vector3& location) const
{
	SceneComponent* parent = GetRootComponent();

	// Calc world to local transform.
	Transform worldToLocal;
	if (parent == nullptr)
	{
		worldToLocal = Transform::GetIdentity();
	}
	else
	{
		worldToLocal = parent->GetComponentTransform().GetInverse();
	}

	Vector3 localPosition = worldToLocal.TransformPoint(location);
	// The beginning location is start with center of each cells.
	localPosition.X() -= 0.5f;
	localPosition.Z() -= 0.5f;

	// The positive number is based on 1.
	if (localPosition.X() > 0)
	{
		localPosition.X() += 1;
	}
	if (localPosition.Z() > 0)
	{
		localPosition.Z() += 1;
	}

	// Offset is value that is in range between -0 and -8.
	// Make it to positive number that is in range between 0 ~ 8.
	int32 intX = (int32)-localPosition.X();
	int32 intY = (int32)-localPosition.Z();

	return { 7 - intX, 7 - intY };
}

ActionRecord AChessBoard::MovePiece(const GridIndex& from, const GridIndex& to)
{
	APiece*& fromPiece = _pieces[from.X][from.Y];
	if (fromPiece == nullptr)
	{
		return false;
	}

	ActionRecord record = fromPiece->Move(to, _built);
	if (!record)
	{
		return false;
	}

	const bool bMoveTurn = Internal_MoveTurn();
	checkf(bMoveTurn, L"Could not move turn.");

	swap(fromPiece, _pieces[to.X][to.Y]);
	_built.Init();

	if (fromPiece != nullptr)
	{
		record.TakeActor(fromPiece);
	}

	return ActionRecord(
		record.GetActor(),
		[&, record = move(record), fromPiece = fromPiece, from = from, to = to]()
		{
			record.Undo();
			_pieces[from.X][from.Y] = record.GetTakeActor();
			swap(_pieces[from.X][from.Y], _pieces[to.X][to.Y]);
		});
}

void AChessBoard::SimulateMoveQuery(MovablePointsQuery& query) const
{
	for (auto& result : query.Results)
	{
		const bool bAttack = result.Type == MovablePointsArray::FigureType::Attack;
		for (size_t i = 0; i < result.Points.size(); ++i)
		{
			auto it = result.Points.begin() + i;

			if (bAttack && !_built.HasPiece(*it))
			{
				result.Points.erase(it);
				i -= 1;
				continue;
			}

			ChessBoardBuilt builder(this);
			builder.SimulateMoveAndBuild(query.OwnerActor->GetIndex(), *it);
		}
	}

	for (int32 i = 0; i < 8; ++i)
	{
		for (int32 j = 0; j < 8; ++j)
		{
			if (APiece* piece = _built.GetPiece(GridIndex(i, j)); piece)
			{
				piece->QueryInteractionWith(query, piece);
			}
		}
	}
}

void AChessBoard::Internal_SpawnPiece(APiece* piece, EChessTeam team, const GridIndex& index)
{
	piece->Init(this, team, index);
	piece->GetRootComponent()->AttachToComponent(GetRootComponent());
	checkf(_pieces[index.X][index.Y] == nullptr, L"Spawn location is not empty.");
	_pieces[index.X][index.Y] = piece;
}

bool AChessBoard::Internal_MoveTurn()
{
	if (_turn == EChessTeam::Black)
	{
		_turn = EChessTeam::White;
		return true;
	}
	else if (_turn == EChessTeam::White)
	{
		_turn = EChessTeam::Black;
		return true;
	}
	LogSystem::Log(LogChessAI, Fatal, L"Current team is not valid value({}).", (int32)_turn);
	return false;
}