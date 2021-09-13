// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "FirstCheckPiece.h"

class AChessPawn : public AFirstCheckPiece
{
public:
	using Super = AFirstCheckPiece;

private:
	static constexpr wchar_t AssetPath[] = L"Content/Chess/Arts/Pawn/Mesh/pawn.fbx";
	uint8 _bCanEnpassant : 1 = false;
	uint8 _bMoved : 1 = false;

public:
	AChessPawn();

	virtual ActionRecord Move(const GridIndex& index, const ChessBoardBuilt& built) override;
	virtual void TurnChanged(EChessTeam changedTurn) override;

	virtual bool QueryMovable(MovablePointsQuery& query, const ChessBoardBuilt& built) const override;
	virtual bool QueryInteractionWith(MovablePointsQuery& query, APiece* piece, const ChessBoardBuilt& built) const override;

protected:
	virtual SStaticMesh* GetStaticMesh() const override;

private:
	inline int32 GetIncrementer() const;
};