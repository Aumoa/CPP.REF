// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "FirstCheckPiece.h"

using namespace std;

AFirstCheckPiece::AFirstCheckPiece() : Super()
{
}

ActionRecord AFirstCheckPiece::Move(const GridIndex& index, const ChessBoardBuilt& built)
{
	ActionRecord record = Super::Move(index, built);
	if (!record)
	{
		return record;
	}

	const bool bFirstRecord = _bFirstMove;
	_bFirstMove = false;

	return ActionRecord(
		this,
		[&, bFirstRecord, record = move(record)]()
	{
		_bFirstMove = bFirstRecord;
		record.Undo();
	});
}