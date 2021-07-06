// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ChessQueries.h"
#include "Actors/Piece.h"
#include "Components/PrimitiveComponent.h"

using namespace std;

#define checkValid() checkf(_bValid, L"Record is not valid.")

ActionRecord::ActionRecord(bool bValid) : _bValid(false)
{
	checkf(!bValid, L"Invalid parameter: bValid == true. If you determine that action record is valid, see constructor function that call with APiece*, function<void()>.");
}

ActionRecord::ActionRecord(APiece* actor, function<void()> undoRecord) : _bValid(true)
	, _actor(actor)
	, _undoRecord(undoRecord)
{
}

void ActionRecord::Undo() const
{
	checkValid();

	if (_undoRecord)
	{
		_undoRecord();
	}

	if (_actorTaked != nullptr)
	{
		auto* primitive = _actorTaked->GetRootComponentAs<PrimitiveComponent>();
		primitive->SetHiddenInGame(false);
	}
}

void ActionRecord::TakeActor(APiece* actor)
{
	checkf(_actorTaked == nullptr, L"Taked actor already assigned.");
	checkf(actor != nullptr, L"Take actor is nullptr.");
	_actorTaked = actor;
	auto* primitive = actor->GetRootComponentAs<PrimitiveComponent>();
	primitive->SetHiddenInGame(true);
}

bool ActionRecord::IsValid() const
{
	return _bValid;
}

#undef checkValid