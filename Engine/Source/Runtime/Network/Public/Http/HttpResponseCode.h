// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "HttpResponseCode.gen.h"

SENUM()
enum class EHttpResponseCode
{
	Unknown = 0,

	Continue = 100,
	SwitchingProtocol = 101,

	Ok = 200,
	Created = 201,
	Accepted = 202,
	NoContent = 204,

	MultipleChoice = 300,

	BadRequest = 400,
	Unauthorized = 401,
	Forbidden = 403,
	NotFound = 404,

	InternalServerError = 500,
	NotImplemented = 501,
	ServiceUnavailable = 503
};