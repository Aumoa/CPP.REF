// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ChessBoard.h"
#include "GameEngine.h"
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

AChessBoard::AChessBoard() : Super()
{
	StaticMeshComponent* smc = CreateSubobject<StaticMeshComponent>();
	SetRootComponent(smc);

	AssetImporter* assimp = GameEngine::GetEngine()->GetAssetImporter();
	StaticMesh* sm = assimp->ImportStaticMesh(L"Contents/Chess/Arts/Board/Mesh/board.fbx");
	smc->SetStaticMesh(sm);

	ColorShader* cshader = GameEngine::GetEngine()->GetColorShader();
	MaterialInstance* black = CreateSubobject<MaterialInstance>(cshader->GetDefaultMaterial());
	black->SetScalarParameterValueByName(L"Color", 0.2f);
	smc->SetMaterial(0, black);

	MaterialInstance* white = CreateSubobject<MaterialInstance>(cshader->GetDefaultMaterial());
	white->SetScalarParameterValueByName(L"Color", 0.8f);
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
		piece = world->SpawnActor<AKing>();
		piece->Init(this, team, GridIndex(4, head));
		piece->GetRootComponent()->AttachToComponent(GetRootComponent());
		
		// QUEEN
		piece = world->SpawnActor<AQueen>();
		piece->Init(this, team, GridIndex(3, head));
		piece->GetRootComponent()->AttachToComponent(GetRootComponent());

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
			piece = world->SpawnActor<ABishop>();
			piece->Init(this, team, GridIndex(get_index(i, 2), head));
			piece->GetRootComponent()->AttachToComponent(GetRootComponent());

			// KNIGHTS
			piece = world->SpawnActor<AKnight>();
			piece->Init(this, team, GridIndex(get_index(i, 1), head));
			piece->GetRootComponent()->AttachToComponent(GetRootComponent());

			// ROOKS
			piece = world->SpawnActor<ARook>();
			piece->Init(this, team, GridIndex(get_index(i, 0), head));
			piece->GetRootComponent()->AttachToComponent(GetRootComponent());
		}

		for (int32 i = 0; i < 8; ++i)
		{
			piece = world->SpawnActor<AChessPawn>();
			piece->Init(this, team, GridIndex(i, forward));
			piece->GetRootComponent()->AttachToComponent(GetRootComponent());
		}
	});
}

AChessBoardProxy* AChessBoard::CreateProxy()
{
	AChessBoardProxy* proxy = CreateSubobject<AChessBoardProxy>(this);
	proxy->SetBoard(this);
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