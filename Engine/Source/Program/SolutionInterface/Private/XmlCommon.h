// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "tinyxml2.h"

class XmlException : public std::exception
{
public:
	XmlException(tinyxml2::XMLError XmlErr)
	{
	}
};

class NullReferenceException : public std::exception
{
public:
	NullReferenceException()
	{
	}
};

#define XmlOp(Op) \
if (tinyxml2::XMLError E = Op; E != XMLError::XML_SUCCESS) \
{ \
	throw XmlException(E);\
}

#define AssignNullCheck(AssignVar, Op) \
auto* AssignVar = Op; \
if (AssignVar == nullptr) \
{ \
	throw NullReferenceException(); \
}