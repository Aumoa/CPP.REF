// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "FirstCheckPiece.h"

class AChessPawn : public AFirstCheckPiece
{
public:
	using Super = AFirstCheckPiece;

private:
	static constexpr wchar_t AssetPath[] = L"Contents/Chess/Arts/Pawn/Mesh/pawn.fbx";

public:
	AChessPawn();

	virtual bool QueryMovable(MovablePointsQuery& query, const ChessBoardBuilt& built) const override;

protected:
	virtual StaticMesh* GetStaticMesh() const override;

private:
	inline int32 GetIncrementer() const;

	void QueryMove(MovablePointsQuery& query) const;
	void QueryAttack(MovablePointsQuery& query) const;
};