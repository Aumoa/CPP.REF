// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CompiledType.h"
#include "BodyAccessKey.h"
#include "CompiledSMEMBER.h"

class CompiledSCONSTRUCTOR;
class CompiledSFIELD;
class CompiledSMETHOD;

class CompiledSCLASS : public CompiledType
{
private:
	size_t _sclass = 0;
	String _className;
	String _parentClass;
	std::vector<String> _interfaceTypes;

	std::vector<std::unique_ptr<CompiledSMEMBER>> _members;

public:
	CompiledSCLASS();

	void CompileSourceTree(const String& classDeclaration, size_t& line);
	virtual void Generate(StringBuilder& sb, const String& fileKey) override;

private:
	std::coroutine_handle<> STree_StartCoro(const String& word, size_t& line);

private:
	std::coroutine_handle<> STree_ClassDeclare(const String& word, size_t& line);

private:
	std::coroutine_handle<> STree_Inheritances(const String& word, size_t& line);

private:
	size_t _generatedBody = 0;
	EBodyAccessKey _eBodyAccess = EBodyAccessKey::Private;
	std::coroutine_handle<> STree_Body(const String& word, size_t& line);

private:
	std::coroutine_handle<> STree_Member(const String& word, size_t& line);

private:
	std::coroutine_handle<> STree_Constructor(CompiledSCONSTRUCTOR* ptr, const String& word, size_t& line);

private:
	std::coroutine_handle<> STree_Field(CompiledSFIELD* ptr, const String& word, size_t& line);

private:
	std::coroutine_handle<> STree_Method(CompiledSMETHOD* ptr, const String& word, size_t& line);
};